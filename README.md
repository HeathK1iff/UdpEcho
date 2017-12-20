# UdpEcho
Library provide easy detect device that use this library in network for esp8266

## Usage
```c++
#include <UdpEcho.h>
#define RESPOND_MAX 5
#define SEC_MS 1000l
...
UdpEcho echo(8313);
unsigned long tsScan;
...
void loop(){
    echo.maintenance();
    if (tsScan < millis()){
        EchoRespond buf[5];
        deviceCount = echo.scan(buf, 5, 8313);
        if (deviceCount > 0)
        {
            for (int i = 0; i < deviceCount; i++) {
                Serial.printf("IP = %s; Host = %s", buf[i].address.toString().c_str(), buf[i].host);
            }
        }
        tsScan = millis() + SEC_MS*5;
    }
}
```


