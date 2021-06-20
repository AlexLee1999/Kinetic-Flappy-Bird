#include "sensor.h"

void Sensor::getAction(uint8_t& up){
    if (!button) {
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        Q.pop_front();
        Q.push_back(_pAccDataXYZ[1]);
        avg = calculate_queue();
        queue_is_init = false;
        if (avg > 500){
            up = 0;
        }
        else if (avg <= 500 && avg > 50) {
            up = 1;
        }
        else if (avg < -500){
            up = 4;
        }
        else if (avg >= -500 && avg < -50) {
            up = 3;
        }
        else {
            up = 2;
        }
    }
    else{
        queue_init();
        up = 2;
    }
}
