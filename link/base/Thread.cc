#include "Thread.h"

#include <functional>
#include <chrono>

using namespace Link::Base;

Thread::Thread()
:started(false),
thread(new std::thread(std::bind(&Thread::beginRun,this)))
{
}

Thread::~Thread()
{

}

void Thread::start()
{
    std::unique_lock<std::mutex> lock(mutex);
    started = true;
    condition.notify_one();
}

void Thread::beginRun()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        if(!started){
            condition.wait(lock);
        }
    }
    run();
}

bool Thread::isStarted()
{
    return started;
}

void Thread::sleepSeconds(const int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

std::thread::id getNowThreadId()
{
    return std::this_thread::get_id();
}

