#include <future>
#include <vector>
#include <memory> // provide 'std::share_ptr'
//#include <node.h> // provide 'v8' namespace

/** RunLoop 执行频率 */
enum RunLoopFrequencyLevel {
  RunLoopFrequencyLevelLow = 0,
  RunLoopFrequencyLevelMedium,
  RunLoopFrequencyLevelHigh,
};

class OCRunLoopContext {
  public:
    OCRunLoopContext();
    ~OCRunLoopContext();
    /** 设置 RunLoop 执行的频率 */
    void updateRunLoopFrequencyLevel(RunLoopFrequencyLevel level);
  private:
    void initRunLoopContext();
    int delayForFrequencyLevel(RunLoopFrequencyLevel level);

    // c++11 feature, https://en.cppreference.com/w/cpp/thread/future
    std::future<bool> _future;
    // c++11 feature, https://en.cppreference.com/w/cpp/atomic/atomic
    std::atomic<bool> _finish;
    // change frequency level
    std::atomic<RunLoopFrequencyLevel> _level;
    // static configuration & instance
    // static std::shared_ptr<V8TaskService> _service;
    static std::vector<int> delays;
};

