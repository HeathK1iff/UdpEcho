#include "UdpEcho.h"
#include <stdio.h>

void UdpEcho::setLog(Print *pLog)
{
  this->pLog = pLog;
}

UdpEcho::UdpEcho(int port){
  echoServer = new WiFiUDP();
  echoServer->begin(port);
}

UdpEcho::~UdpEcho(){
  delete echoServer;  
}

int UdpEcho::scan(EchoRespond ret[], IPAddress target, int size, int port)
{
  int count = 0;
  if (!WiFi.isConnected())
    return count;
 
  WiFiUDP *udp = new WiFiUDP();
  udp->begin(port + 1);
  udp->beginPacket(target, port);
  udp->write(WiFi.hostname().c_str(), strlen(WiFi.hostname().c_str()));
  udp->endPacket();

  if (pLog != NULL)
    pLog->printf("Send echo %s:%d\n", target.toString().c_str(), port);
  
  for (int i = 0; i < size; i++)
  {
    delay(100);
    int packetSize = udp->parsePacket();
    if (packetSize > 0)
    {
      char packet[packetSize + 1];
      memset(packet, '\0', packetSize);
      memset(ret[count].host, '\0', HOST_RESPOND_SIZE);
      udp->read(packet, packetSize);
      
      ret[count].address = udp->remoteIP();      
      strncpy(ret[count].host, packet, packetSize);
      ret[count].host[packetSize] = '\0';
      
      if (pLog != NULL)
        pLog->printf("Echo(%s) was received from %s:%d\n", ret[count].host, udp->remoteIP().toString().c_str(), udp->remotePort());
      count++;
    }
  }
  delete udp;
    
  return count;
}

int UdpEcho::scan(EchoRespond ret[], int size, int port)
{
  return scan(ret, IPAddress(255,255,255,255), size, port);
}

bool UdpEcho::isExist(char *host, int port){
  EchoRespond ret[RESPOND_CAPACITY];
  int count = scan(ret, RESPOND_CAPACITY, port);
  for (int i = 0; i < count; i++){
    if (strcmp(ret[i].host, host) == 0)
      return true;
  }
  return false;
}

bool UdpEcho::getHostByIp(char *host, IPAddress target, int port){
  EchoRespond ret[RESPOND_CAPACITY];
  int count = scan(ret, RESPOND_CAPACITY, port);
  for (int i = 0; i < count; i++){
    if (ret[i].address == target){
      strncpy(host, ret[i].host, 30);
      host[29] = '\0';
      return true;      
    }
  }
  return false;
}

bool UdpEcho::getIpByHost(IPAddress &ip, char *host, int port){
  EchoRespond ret[RESPOND_CAPACITY];
  int count = scan(ret, RESPOND_CAPACITY, port);
  for (int i = 0; i < count; i++){
    if (strcmp(host, ret[i].host) == 0){
      ip = ret[i].address;
      return true;      
    }
  }
  return false;
}

bool UdpEcho::isExist(IPAddress target, int port){
  EchoRespond ret[RESPOND_CAPACITY];
  return (scan(ret, target, RESPOND_CAPACITY, port) > 0);
}

void UdpEcho::maintenance()
{
  if (WiFi.isConnected())
  {
    int respond_bytes = echoServer->parsePacket();
    if ((respond_bytes > 0) && (respond_bytes < UDP_PACKET_SIZE))
    {       
      char inPacket[respond_bytes];
      memset(inPacket,'\0', respond_bytes);
      echoServer->read(inPacket, ((respond_bytes > UDP_PACKET_SIZE) ? UDP_PACKET_SIZE : respond_bytes));

      if (pLog != NULL)
        pLog->printf("Echo(%s) was received from %s:%d\n", inPacket, echoServer->remoteIP().toString().c_str(), echoServer->remotePort());
        
      echoServer->beginPacket(echoServer->remoteIP(), echoServer->remotePort());
      echoServer->write(WiFi.hostname().c_str(), strlen(WiFi.hostname().c_str()));
      echoServer->endPacket();
      if (pLog != NULL)
        pLog->printf("Respond(%s) was send\n", WiFi.hostname().c_str());
    }
  }
}

