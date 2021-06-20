#include "mbed.h"
#include "TCPSocket.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include <cstdint>
#include <string>
#include <queue>

#define WIFI_IDW0XX1    2
#include "ISM43362Interface.h"
ISM43362Interface wifi(false);
static EventQueue event_queue(16 * EVENTS_EVENT_SIZE);
#define IP_address  "192.168.50.173"
#define Port_number 54087
#define SEND_INT    5

DigitalIn button(BUTTON1);
// InterruptIn user_button(BUTTON1);
SocketAddress addr(IP_address, Port_number);

class Sensor{
public:
    Sensor(events::EventQueue &event_queue):_event_queue(event_queue){
        BSP_ACCELERO_Init();    
    }

    void getAction(uint8_t& up){
        if (!button) {
            BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
            Q.pop_front();
            Q.push_back(_pAccDataXYZ[1]);
            printf("ACCELERO_Y = %d\n", _pAccDataXYZ[1]);
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
        float parameter[2] = { 0.2, 0.8};
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

class WIFI{
public:
    WIFI( NetworkInterface *wifi ,Sensor * sensor, events::EventQueue &event_queue, TCPSocket* socket ): _wifi(wifi),_sensor(sensor), _event_queue(event_queue), _socket(socket) {
        connect();
    }
    void connect(){
        printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
        int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
        if (ret != 0) {
            printf("\nConnection error\n");
        }
        printf("Success\n\n");
        printf("MAC: %s\n", wifi.get_mac_address());
        printf("IP: %s\n", wifi.get_ip_address());
        printf("Netmask: %s\n", wifi.get_netmask());
        printf("Gateway: %s\n", wifi.get_gateway());
        printf("RSSI: %d\n\n", wifi.get_rssi());
        printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
        nsapi_error_t response;
        response = _socket->open(_wifi);
        if (0 != response){
            printf("Error opening: %d\n", response);
        }
        
        response = _socket->connect(addr);
    
        if (0 != response){
            printf("Error connecting: %d\n", response);
        }


        _socket->set_blocking(1);
        _event_queue.call_every(SEND_INT, this, &WIFI::send_data);
    }

    ~WIFI() {
        _socket->close();
        _wifi->disconnect();
    }
    void send_data() {
        char data[64];
        nsapi_error_t response;
        uint8_t up;
        _sensor->getAction(up);
        int len = sprintf(data,"%d",up);
        // printf("%d\n", up);
        response = _socket->send(data, len);
        if (0 >= response){
            printf("Error sending: %d\n", response);
        }
    }
private:
    NetworkInterface *    _wifi;
    Sensor *              _sensor;
    events::EventQueue    &_event_queue;
    TCPSocket*            _socket;
};


TCPSocket socket;
Sensor _sensor(event_queue);
WIFI   _wifi(&wifi, &_sensor, event_queue, &socket);

int main()
{
   event_queue.dispatch_forever();
   printf("\nDone\n");
}
