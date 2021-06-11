/* WiFi Example
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include "mbed.h"
#include "TCPSocket.h"
#include <cstdio>
#include <queue>
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"



#define WIFI_IDW0XX1    2

#include "ISM43362Interface.h"
ISM43362Interface wifi(false);
InterruptIn button(BUTTON1);
bool pressed = false;
bool queue_is_init = false;
queue<int> Q;
void queue_init(){
    int size = Q.size();
    for (int i=0; i< size; ++i){
        Q.pop();
    }
    for (int i=0; i<5; ++i){
        Q.push(0);
    }
    queue_is_init = true;
}
void button_pressed(){
    pressed = true;
}
void button_released(){
    pressed = false;
    if(!queue_is_init){
      queue_init();  
    }
}

void acc_server(NetworkInterface *net)
{
    TCPSocket socket;
    SocketAddress addr("192.168.0.145", 10024);
    nsapi_error_t response;
    int16_t pDataXYZ[3] = {0};
    queue_init();
    int avg_sum = 0;
    int oldest;
    int up;
    float avg;
    char recv_buffer[9];
    char acc_json[64];
    int sample_num = 0;
    // Open a socket on the network interface, and create a TCP connection to addr
    response = socket.open(net);
    if (0 != response){
        printf("Error opening: %d\n", response);
    }
    response = socket.connect(addr);
    if (0 != response){
        printf("Error connecting: %d\n", response);
    }
    socket.set_blocking(1);
    while (1){
        button.fall(&button_pressed);
        button.rise(&button_released);
        ++sample_num;
        if (pressed) {
            BSP_ACCELERO_AccGetXYZ(pDataXYZ);
            int x = pDataXYZ[0], y = pDataXYZ[1], z = pDataXYZ[2];
            oldest = Q.front();
            Q.pop();
            Q.push(y);
            queue_is_init = false;
            avg_sum -= oldest;
            avg_sum += y;
            avg = avg_sum / 5;
            if (avg > 0){
                up = 0;
            }
            else{
                up = 2;
            }
        }
        else{
            up = 1;
        }
        printf("%d\n", up);
        // int len = sprintf(acc_json, "%d", up);
        // response = socket.send(acc_json, len);
        // if (0 >= response){
        //     printf("Error sending: %d\n", response);
        // }
        rtos::ThisThread::sleep_for(10ms);
    }
    socket.close();
}

int main()
{
    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    //wifi.set_network("192.168.130.105","255.255.255.0","192.168.130.254");
    int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error\n");
        return -1;
    }
    printf("Success\n\n");
    printf("MAC: %s\n", wifi.get_mac_address());
    printf("IP: %s\n", wifi.get_ip_address());
    printf("Netmask: %s\n", wifi.get_netmask());
    printf("Gateway: %s\n", wifi.get_gateway());
    printf("RSSI: %d\n\n", wifi.get_rssi());
    BSP_ACCELERO_Init();
    acc_server(&wifi);
}
