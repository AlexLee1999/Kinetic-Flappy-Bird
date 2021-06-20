#ifndef WIFI_H
#define WIFI_H
#include "mbed.h"
#include "TCPSocket.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include <cstdint>
#include <string>
#include <queue>

class WIFI{
public:
    WIFI( NetworkInterface *wifi ,Sensor * sensor, events::EventQueue &event_queue, TCPSocket* socket, SocketAddress* addr, DigitalIn* button): _wifi(wifi),_sensor(sensor), _event_queue(event_queue), _socket(socket), _addr(addr), _button(button) {
        connect();
    }
    ~WIFI() {
        _socket->close();
        _wifi->disconnect();
    }
    void connect();
    void send_data();

private:
    NetworkInterface *    _wifi;
    Sensor *              _sensor;
    events::EventQueue    &_event_queue;
    TCPSocket*            _socket;
    SocketAddress*        _addr;
    DigitalIn*            _button;
};
