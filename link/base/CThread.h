#ifndef __LINK_BASE_CTHREAD_H__
#define __LINK_BASE_CTHREAD_H__

#include <pthread.h>
#include <unistd.h>

namespace Link{
namespace Base{

class CThread{
public:
    CThread();
    ~CThread();
    void start();
    static pthread_t getPid();
    
    bool isStarted();
    virtual void run() = 0;
protected:
    void sleepSeconds(const int seconds);
private:
    static void* cheatRun(void *vthis);
    void beginRun();
    bool started;
    pthread_t pid;
    pthread_mutex_t mutex;
    pthread_cond_t condition;

};

}
}
#endif