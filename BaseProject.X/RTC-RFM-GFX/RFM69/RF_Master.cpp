

#include "RFM69.h"
#include "RF_Master.h"

//Test of all functions

/*RFM69(uint8_t slaveSelectPin, uint8_t interruptPin, bool isRFM69HW) //interruptNum is now deprecated
                : _slaveSelectPin(slaveSelectPin), _interruptPin(interruptPin), _isRFM69HW(isRFM69HW) {};
    bool initialize(uint8_t freqBand, uint8_t ID, uint8_t networkID=1);
    void setAddress(uint8_t addr);
    void setNetwork(uint8_t networkID);
    bool canSend();
    virtual void send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK=false);
    virtual bool sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries=2, uint8_t retryWaitTime=40); // 40ms roundtrip req for 61byte packets
    virtual bool receiveDone();
    bool ACKReceived(uint8_t fromNodeID);
    bool ACKRequested();
    virtual void sendACK(const void* buffer = "", uint8_t bufferSize=0);
    uint32_t getFrequency();
    void setFrequency(uint32_t freqHz);
    void encrypt(const char* key);
    int16_t readRSSI(bool forceTrigger=false); // *current* signal strength indicator; e.g. < -90dBm says the frequency channel is free + ready to transmit
    void promiscuous(bool onOff=true);
    virtual void setHighPower(bool onOFF=true); // has to be called after initialize() for RFM69HW
    virtual void setPowerLevel(uint8_t level); // reduce/increase transmit power level
    void sleep();
    uint8_t readTemperature(uint8_t calFactor=0); // get CMOS temperature (8bit)
    void rcCalibration(); // calibrate the internal RC oscillator for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]

    // allow hacking registers by making these public
    uint8_t readReg(uint8_t addr);
    void writeReg(uint8_t addr, uint8_t val);
    void readAllRegs();*/

void testAll()
{
    uint8_t slaveSelectPin = 0, interruptPin = 0;
    bool isRFM69HW = 1;
    uint8_t freqBand = 91, ID = 5, networkID = 5;
    
    
    
    RFM69 radio(slaveSelectPin, interruptPin, isRFM69HW);
    radio.initialize(freqBand, ID, networkID);
    radio.canSend();
    //radio.send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK=false);
    //radio.sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries=2, uint8_t retryWaitTime=40); // 40ms roundtrip req for 61byte packets
    //radio.receiveDone();
    //radio.ACKReceived(uint8_t fromNodeID);
    radio.ACKRequested();
    //radio.sendACK(const void* buffer = "", uint8_t bufferSize=0);
    radio.getFrequency();
    //radio.setFrequency(uint32_t freqHz);
    //radio.encrypt(const char* key);
    //radio.readRSSI(bool forceTrigger=false); // *current* signal strength indicator; e.g. < -90dBm says the frequency channel is free + ready to transmit
    //radio.promiscuous(bool onOff=true);
    //radio.setHighPower(bool onOFF=true); // has to be called after initialize() for RFM69HW
    //radio.setPowerLevel(uint8_t level); // reduce/increase transmit power level
    radio.sleep();
    //radio.readTemperature(uint8_t calFactor=0); // get CMOS temperature (8bit)
    radio.rcCalibration(); // calibrate the internal RC oscillator for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]

}
