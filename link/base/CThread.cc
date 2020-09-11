#include "CThread.h"

using namespace Link::Base;

void* CThread::cheatRun(void *vthis){
    CThread* t = (CThread*) vthis;
    //编译器是否还会为其传入this指针？？？？？
    t->beginRun();
    return NULL;
}

CThread::CThread()
:started(false),
mutex(PTHREAD_MUTEX_INITIALIZER),
condition(PTHREAD_COND_INITIALIZER)
{
    //使用默认配置创建互斥量
    pthread_mutex_init(&mutex,0);
    //使用默认配置创建条件变量
    pthread_cond_init(&condition,0);
    //默认配置来创建线程
    pthread_create(&pid,0,CThread::cheatRun,(void*)this);
}

CThread::~CThread()
{

}

void CThread::start()
{
    pthread_mutex_lock(&mutex);
    started = true;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
}

void CThread::beginRun()
{
    pthread_mutex_lock(&mutex);
    while(!started){
        pthread_cond_wait(&condition,&mutex);
    }
    pthread_mutex_unlock(&mutex);
    run();
}

bool CThread::isStarted()
{
    return started;
}

void CThread::sleepSeconds(const int seconds)
{
    ::sleep(seconds);
}

pthread_t CThread::getPid()
{
    return pthread_self();
}