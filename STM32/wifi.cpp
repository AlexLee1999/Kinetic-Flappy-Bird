#include "wifi.h"


void WIFI::connect(){
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
    _event_queue.call_every(500, this, &WIFI::blink);
}

void WIFI::send_data() {
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
