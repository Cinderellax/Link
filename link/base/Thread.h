#ifndef __LINK_BASE_THREAD_H__
#define __LINK_BASE_THREAD_H__

#include <thread>
#include <memory>
#include <condition_variable>

namespace Link{
namespace Base{

class Thread{
public:
    Thread();
    virtual ~Thread();
    void start();
    static std::thread::id getNowThreadId();
    bool isStarted();
    virtual void run() = 0;

protected:
    void sleepSeconds(const int seconds);
private:
    void beginRun();
    bool started;
    std::mutex mutex;
    std::condition_variable condition;
    std::shared_ptr<std::thread> thread;
    
};

}
}
#endif