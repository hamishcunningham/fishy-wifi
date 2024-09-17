#ifndef ElfLora_h
#define ElfLora_h

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <ACROBOTIC_SSD1306.h>

// Pins for the Heltec board
#define SS 18
#define RST 14
#define DI0 26
#define FREQ 433E6

class ElfNode
{
  public:
    ElfNode(byte localAddr, byte gateway);
    void initialize();
    void initOLED(); // Create common static class for OLED
    void oledPrint(String text, int line);
    void sendData(String data);
    void sendRequested(); // Function to reply if data requested by gateway
    void receiveData();
    void setAddr(byte address);
    void setGatewayAddr(byte address);
    void setSF(int sf); // Set spread factor
    byte getAddr();
    byte getGatewayAddr();
    int getSF();  // Get spread factor
  private:
    int num_sent;        // Number of packets sent
    int num_received;    // Number of packets received
    int spreadFactor;    // The radios spread factor
    byte localAddress;   // The address of the node e.g. 0xBB
    byte gatewayAddress; // The address of the gateway
};

class ElfGateway
{
  public:
    ElfGateway(byte addr);
    void initialize();
    void initOLED();
    void oledPrint(String text, int line);
    void sendData(String data, byte NodeAddr);
    void receiveData();
    void uploadToSever(String data, byte NodeAddr); // Upload data to server
    void requestData(byte nodeAddr); // Send a date request to a node
    void setAddr(byte addr);
    void setSF(int sf);
    byte getAddr();
    int getSF();
  private:
    byte address;     // The address of the gateway
    int num_sent;     // Number of packets sent
    int num_received; // Number of packets received
    int spreadFactor; // The radios spread factor
};

#endif
