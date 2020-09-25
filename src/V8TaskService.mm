#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <node.h>
#include <uv.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include "./V8TaskService.h"
#include "./OCRunLoopContext.h"

V8TaskService::V8TaskService() {
    _async.data = new AsyncHandleData;
    uv_async_init(uv_default_loop(), &_async, [](uv_async_t *handle){
        AsyncHandleData *handleData = (AsyncHandleData *)handle->data;
        std::list<std::function<void()>> excuteList;
        {
            std::unique_lock<std::recursive_mutex> lock(handleData->mtx);
            while (handleData->taskList.size() > 0) {
                excuteList.push_back(handleData->taskList.front());
                handleData->taskList.pop_front();
            }
        }
        while (excuteList.size() > 0) {
            excuteList.front()();
            excuteList.pop_front();
        }
    });
}

V8TaskService::~V8TaskService() {
    uv_close((uv_handle_t *)&_async, nullptr);
    delete (AsyncHandleData *)_async.data;
}

void V8TaskService::defaultLoopAsync() {
    std::function<void()> task = [=]() {
        //  insert nsapp event into nodejs event loop
        //  https://stackoverflow.com/questions/6732400/cocoa-integrate-nsapplication-into-an-existing-c-mainloop
        NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
        //  Convert the cocoa events to something useful here and add them to your own event queue
        [NSApp sendEvent: event];
    };
    AsyncHandleData *handleData = (AsyncHandleData *)_async.data;
    {
        std::unique_lock<std::recursive_mutex> lock(handleData->mtx);
        handleData->taskList.push_back(task);
    }
    uv_async_send(&_async);
}
