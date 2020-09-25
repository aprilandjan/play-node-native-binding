#ifndef V8_TASK_SERVICE_H
#define V8_TASK_SERVICE_H

#include <list>
#include <mutex>
#include <node.h>
#include <uv.h>

struct AsyncHandleData {
    std::list<std::function<void()>> taskList;
    std::recursive_mutex mtx;
};

class V8TaskService {
public:
    V8TaskService();
    ~V8TaskService();
    void defaultLoopAsync();
private:
    uv_async_t _async;
};

#endif //V8_TASK_SERVICE_H
