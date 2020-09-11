#include "./EventLoop.h"
#include <iostream>

using namespace Link::Net;

EventLoop::EventLoop()
:eventCtrl(new IOEventCtrl)
{
}

EventLoop::~EventLoop(){
    delete eventCtrl;
}

void EventLoop::addEvent(IOEvent::ptr event){
    eventCtrl->addEvent(event);
}

void EventLoop::removeEvent(IOEvent::ptr event){
    eventCtrl->deleteEvent(event);
}

void EventLoop::modifyEvent(IOEvent::ptr event){
    eventCtrl->modifyEvent(event);
}

void EventLoop::run(){
    while(1){
        eventCtrl->waitAndRunHandle(PollTimeMs);
    }

}