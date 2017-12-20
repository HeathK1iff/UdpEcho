#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <Print.h>

#ifndef UDPECHO_H
#define UDPECHO_H

#define UDP_PACKET_SIZE 200
#define HOST_RESPOND_SIZE 30
#define RESPOND_CAPACITY 4

struct EchoRespond{
  IPAddress address;
  char host[HOST_RESPOND_SIZE];
};

class UdpEcho
{
private:
  Print *pLog;
  WiFiUDP *echoServer;
  int scan(EchoRespond ret[], IPAddress target, int size, int port);
public:
  UdpEcho(int port);
  ~UdpEcho();

  bool isExist(char *host, int port);
  bool isExist(IPAddress target, int port);
  bool getHostByIp(char *host, IPAddress target, int port);
  bool getIpByHost(IPAddress &target, char *host, int port);

  int scan(EchoRespond ret[], int size, int port);
  void maintenance();
  void setLog(Print *pLog);
};

#endif