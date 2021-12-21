#pragma once
#include "base/type.hpp"
#include <ctime>
#include <thread>
#include <vector>
#include <thread>
#include <string>
#include <stdio.h>

typedef struct grasperObject {    // Grasper Object
    uint16_t time; // last check time (relative to server.lruclock) :16bit
    int counter;   // reference counter:8 bits
    ikey_t key;
} gobj;

class swap
{
private:
    unsigned int _interval;
    unsigned int _pool_size;
    int lfu_incr_factor;
    uint8_t init_counter;
    std::time_t currentTime;    //useless
    bool swapOut(int pos);
    bool swapIn(int pos);
    vector<gobj> _gobjs;


public:
    swap(int interval, int pool_size, int factor, uint8_t counter);
    ~swap();
    uint16_t getIdleTime(uint16_t time);
    uint8_t LFUIncreaseAndReturn(gobj &gobj);
    uint8_t LFUDecreaseAndReturn(gobj &gobj);
    gobj* createGrasperObj(ikey_t key);
    uint16_t getCurrentTime();
    void LFUSwap(unsigned int interval, int pool_size);
    
};

swap::swap(unsigned int interval,unsigned int pool_size, int factor, uint8_t counter)
{
    this->_interval = internal;
    this->_pool_size = pool_size;
    this->lfu_incr_factor = factor;
    this->init_counter = counter;
    this->currentTime = std::time(0);
    std::thread swap_t(LFUSwap, interval, pool_size);
}

swap::~swap()
{
    vector<gobj>().swap(_gobjs);
}
