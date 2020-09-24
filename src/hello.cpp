#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <list>
#include "./OCRunLoopContext.h"

//===== run without mutex =====
void func1(int i) {
    std::cout << "entry func thread id = " << i << "\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    std::cout << "leave func thread id = " << i << "\n" << std::endl;
}

void run_without_mutex() {
    int size = 5;
    std::thread tList[size];
    for(int i = 0; i < size; i++) {
        tList[i] = std::thread(func1, i);
    }
    for(int i = 0; i < size; i++) {
        tList[i].join();
    }
    std::cout << "finish!" << std::endl;
    return;
}

//====== run with mutex =====
//  global mutex
std::mutex g_mutex;

void func2(int i) {
    // g_mutex.lock();
    // func1(i);
    // g_mutex.unlock();

    //  add mutex, ensure that even if error occurred, the lock is freed automatically when exiting current scope
    std::lock_guard<std::mutex> lock(g_mutex);
    func1(i);
}

void run_with_mutex() {
    int size = 5;
    std::thread tList[size];
    for(int i = 0; i < size; i++) {
        tList[i] = std::thread(func2, i);
    }
    for(int i = 0; i < size; i++) {
        tList[i].join();
    }
    std::cout << "finish!" << std::endl;
}

//===== condition variable =====
class CTask{
public:
    CTask(int taskId) {
        this->_taskId = taskId;
    }
    void doTask() {
        std::cout << "consumer a task id is " << _taskId << std::endl;
    }
private:
    int _taskId;
};

std::list<std::shared_ptr<CTask>> g_task;
//  the condition that threads can hold and wait
std::condition_variable g_conv;

//  the producer thread, create task and put it into task list
void producer_do() {
    int taskId = 0;
    std::shared_ptr<CTask> ptask = nullptr;
    while(true) {
        //  create task
        ptask = std::make_shared<CTask>(taskId);
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            g_task.push_back(ptask);
            std::cout << "produce a task id is " << taskId << std::endl;
        }
        //  wake up thread
        g_conv.notify_one();
        taskId++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

//  the consumer thread,
void consumer_do() {
    std::shared_ptr<CTask> ptask = nullptr;
    while(true) {
        std::unique_lock<std::mutex> lock(g_mutex);
        while(g_task.empty()) {
            //  wait until notification
            g_conv.wait(lock);
        }

        //  handle the first task
        ptask = g_task.front();
        g_task.pop_front();

        if (ptask == nullptr) {
            continue;
        }
        ptask->doTask();
    }
}

void run_producer_and_consumer() {
    std::thread t1(consumer_do);
    std::thread t2(consumer_do);
    std::thread t3(consumer_do);
    std::thread t4(producer_do);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

//===== OCRunLoop =====

void run_oc_runloop() {
    std::cout << "main thread start...\n";
    OCRunLoopContext loop;
    // loop.updateRunLoopFrequencyLevel(RunLoopFrequencyLevel::RunLoopFrequencyLevelLow);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "main thread done!\n";
}

std::string hello(std::string name)
{
    // run_without_mutex();
    // run_with_mutex();
    // run_producer_and_consumer();
    run_oc_runloop();

    return "Hello, " + name + "!";
}
