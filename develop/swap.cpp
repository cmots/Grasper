#include "swap.hpp"
#include "storage/vkvstore.hpp"
#include "storage/ekvstore.hpp"

#include <cstdlib>
/**
 * @brief  get the difference between current time and last check time
 * @note   if now < time, it turns to next round, like carrying
 * @param  time: last check time
 * @retval time diff between last time and current
 */
static uint16_t swap::getIdleTime(uint16_t time)
{
    uint16_t now = getCurrentTime();
    if (now >= time)
        return now - time;
    return 65535 - time + now;
}

/**
 * @brief  increase the counter of a gobj
 * @note   Use lfu_incr_factor to control the increase ratio, larger factor means smaller ratio
 *         Also, larger counter causes smaller increase ratio
 * @param  &gobj: 
 * @retval 
 */
static uint8_t swap::LFUIncreaseAndReturn(gobj &gobj)
{
    uint8_t counter = gobj.counter;
    if (counter == 255)
        return 255;
    double r = (double)rand() / RAND_MAX;
    double baseval = counter - init_counter;
    if (baseval < 0)
        baseval = 0;
    double p = 1.0 / (baseval * lfu_incr_factor + 1);
    if (r < p)
    {
        counter++;
        gobj.counter = counter;
    }
    return counter;
}

/**
 * @brief  decrease the counter of a gobj
 * @note   
 * @param  &gobj: the gobj to be operated
 * @retval counter after decrease
 */
static uint8_t swap::LFUDecreaseAndReturn(gobj &gobj)
{
    uint16_t last_time = gobj.time;
    uint8_t counter = gobj.counter;
    uint16_t num_periods = getIdleTime(last_time) / ((_interval << 16) & 65535);
    if (num_periods)
    {
        counter = num_periods > counter ? 0 : counter - num_periods;
        gobj.counter = counter;
    }
    return counter;
}

/**
 * @brief  Create a new grasper object
 * @note   set its counter to init_counter, or it may be swapped after init
 * @param  key: the key of grasper object
 * @retval the newly create grasper object
 */
static gobj *swap::createGrasperObj(ikey_t key)
{
    gobj *gobj = malloc(sizeof(gobj));
    gobj->key = key;
    gobj->counter = init_counter;
    gobj->time = getCurrentTime();
    _gobjs.push_back(*gobj);
    return gobj;
}

/**
 * @brief  get 16 high bits of current unix time => minute
 * @note   
 * @retval the current time in unit of minute
 */
static uint16_t swap::getCurrentTime()
{
    return (std::time(0) / 60) & 65535;
}

static bool swap::swapOut(int pos)
{
    return true;

    // gobj obj = _gobjs.at(pos);
    // ikey_t key = obj.key;
    // value_t &val;

    // VKVStore::get_property_local(key.pid, val);
    // string fname = key.pid;
    // ofstream out_f(fname, ios::binary);

    // if (!out_f.is_open())
    // {
    //     return false;
    // }

    // out_f.write((char *)val.content);

    // out_f.close();

    // return true;
}

static bool swap::swapIn(int pos)
{
    return true;
    // string fn="";
    // ifstream in_f(fn, ios::binary);

    // if (!in_f.is_open())
    // {
    //     return false;
    // }

    // char *mem = get<2>(data);

    // uint64_t last_entry, mem_sz;

    // in_f.read((char *)&last_entry, sizeof(uint64_t));
    // in_f.read((char *)&mem_sz, sizeof(uint64_t));
    // in_f.read(mem, mem_sz);
    // in_f.close();

    // data = make_tuple(last_entry, mem_sz, mem);

    // return true;
}


inline bool comp(pair<gobj, int> & a, pair<gobj, int> & b)
{
    if (a.first.counter <= b.first.counter)
        return true;
    else
        return false;
}

static void swap::LFUSwap()
{
    while (true)
    {
        srand((unsigned)time(NULL));
        vector<pair<gobj, int>> tmp_gobjs;
        for (int i =0;i< _pool_size;i++)
        {
            int r = rand() % _gobjs.size();
            LFUDecreaseAndReturn(_gobjs.at(r));
            tmp_gobjs.push_back(pair<gobj, int>(_gobjs.at(r), r));
        }
        std::sort(tmp_gobjs.begin(), tmp_gobjs.end(), comp);
        swapOut(tmp_gobjs.at(0).second);
        std::this_thread::sleep_for(std::chrono::milliseconds(_interval * 100));
    }
}

