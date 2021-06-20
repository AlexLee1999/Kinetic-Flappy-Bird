#ifndef SENSOR_H
#define SENSOR_H
#include "mbed.h"
#include "TCPSocket.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include <cstdint>
#include <string>
#include <queue>


class Sensor{
public:
    Sensor(events::EventQueue &event_queue):_event_queue(event_queue){
        BSP_ACCELERO_Init();
    }
    void getAction(uint8_t&);
private:
    events::EventQueue &_event_queue;
    int16_t _pAccDataXYZ[3] = {0};
    bool queue_is_init = false;
    deque<int> Q;
    float avg;
    void queue_init(){
        if(!Q.empty()){
            Q.clear();
        }
        for (int i=0; i<2; ++i){
            Q.push_back(0);
        }
        queue_is_init = true;
    }

    float calculate_queue(){
        float parameter[2] = {0.2, 0.8};
        int count = 0;
        float result = 0;
        for(auto it = std::begin(Q); it!=std::end(Q); ++it){
            float res = (*it) * parameter[count];
            result += res;
            count++;
        }
        return result;
    }
};

#endif
