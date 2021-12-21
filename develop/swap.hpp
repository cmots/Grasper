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
    static unsigned int _interval;
    static unsigned int _pool_size;
    static int lfu_incr_factor;
    static uint8_t init_counter;
    static bool swapOut(int pos);
    static bool swapIn(int pos);
    static vector<gobj> _gobjs;


public:
    swap(unsigned int interval, int pool_size, int factor, uint8_t counter);
    ~swap();
    static uint16_t getIdleTime(uint16_t time);
    static uint8_t LFUIncreaseAndReturn(gobj &gobj);
    static uint8_t LFUDecreaseAndReturn(gobj &gobj);
    static gobj* createGrasperObj(ikey_t key);
    static uint16_t getCurrentTime();
    static void LFUSwap();
    
};

swap::swap(unsigned int interval, int pool_size, int factor, uint8_t counter)
{
    this->_interval = internal;
    this->_pool_size = pool_size;
    this->lfu_incr_factor = factor;
    this->init_counter = counter;
    this->currentTime = std::time(0);
    std::thread swap_t(LFUSwap,NULL);
}

swap::~swap()
{
    vector<gobj>().swap(_gobjs);
}
