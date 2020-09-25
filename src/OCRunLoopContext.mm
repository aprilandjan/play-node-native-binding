#include <future>
#include <vector>
#include <memory> // provide 'std::share_ptr'
#include <iostream>
#include <thread>
#include <chrono> // provide time things
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include "./OCRunLoopContext.h"
#include "./V8TaskService.h"

// define runloop reaction delay time
#define DEFAULT_OC_RUNLOOP_DELAY_LOW 100
#define DEFAULT_OC_RUNLOOP_DELAY_MEDIUM 300
#define DEFAULT_OC_RUNLOOP_DELAY_HIGH 500

// class static properties
std::shared_ptr<V8TaskService> OCRunLoopContext::service = std::make_shared<V8TaskService>();
std::vector<int> OCRunLoopContext::delays = { DEFAULT_OC_RUNLOOP_DELAY_HIGH, DEFAULT_OC_RUNLOOP_DELAY_MEDIUM, DEFAULT_OC_RUNLOOP_DELAY_LOW};

//  constructor, init instance properties
OCRunLoopContext::OCRunLoopContext(): _finish(false), _level(RunLoopFrequencyLevelHigh) {
  this->initRunLoopContext();
  [NSApplication sharedApplication];
}

//  private member function
void OCRunLoopContext::initRunLoopContext() {
  //  lambda capture 'this'
  //  https://en.cppreference.com/w/cpp/thread/async
  _future = std::async(std::launch::async, [this](){
    //  this function will be executed in seperate thread
    while(true) {
      int loopDelay = delays[_level.load(std::memory_order_relaxed)];
      std::this_thread::sleep_for(std::chrono::milliseconds(loopDelay));
      //  if '_finish' is set to true, exit this thread
      if (this->_finish.load(std::memory_order_relaxed)) {
        break;
      }
      //  insert tasks in libuv default run loops
      service->defaultLoopAsync();
    }
    return true;
  });
}

//  public member function
void OCRunLoopContext::updateRunLoopFrequencyLevel(RunLoopFrequencyLevel level) {
  //  save the frequency level into atomic variable
  this->_level.store(level, std::memory_order_relaxed);
}

//  destructor
OCRunLoopContext::~OCRunLoopContext() {
  //  when destruct, mark '_finish' as true to exit async thread
  this->_finish.store(true, std::memory_order_relaxed);
  //  will block until data is available
  _future.get();
}
