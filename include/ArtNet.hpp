/**
 * @file ArtNet.hpp
 * @author your name (you@domain.com)
 * @brief implementation of ArtNet-4 base function (all function note device specific)
 * @version 0.1
 * @date 2025-12-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdint.h>


class ArtNet {
private:
    static constexpr uint8_t artPollPacketLen = 22;

    //some constants
    static constexpr uint8_t ipAddressLen = 4;
    static constexpr uint8_t macAddressLen = 6;
    static constexpr uint16_t artNetPort = 0x1936;
    static constexpr uint8_t artPollTimeOut = 3; //seconds
    static constexpr uint8_t minArtPollLen = 14;
    static constexpr uint16_t minProtVersion = 14;
    static constexpr uint16_t protVersion = 14;
    static constexpr uint8_t artNetIdentLen = 8;

    static constexpr uint8_t artNetIdent[artNetIdentLen] = {'A', 'r', 't','-','N','e', 't', 0x00};
    
    /**
     * @brief enum of all possible style codes
     */
    enum styleCodes{
        StNode          = 0x00,
        StController    = 0x01,
        StMedia         = 0x02,
        StRoute         = 0x03,
        StBackup        = 0x04,
        StConfig        = 0x05,
        StVisual        = 0x06,
    };

    /**
     * @brief enum of possible opCodes
     */
    enum opCodes {
        opPoll          = 0x2000,
        opPollReply     = 0x2100,
        opDiagData      = 0x2300,
        opCommand       = 0x2400,
        opDataRequest   = 0x2700,
        opDataReply     = 0x2800,
        opDmx = 0x5000,
        opNzs = 0x5100,
        opSync = 0x5200,
        opAddress = 0x6000,
        opInput = 0x7000,
        opTodRequest = 0x8000,
        opTodData = 0x8100,
        opTodControl = 0x8200,
        opRdm = 0x8300,
        opRdmSub = 0x8400,
        opVideoSetup = 0xa010,
        opVideoPalette = 0xa020,
        opVideoData = 0xa040,
        opMacMaster = 0xf000, //deprecated
        opMacSlave = 0xf100, //deprecated
        opFirmwareMaster = 0xf200,
        opFirmwareReply = 0xf300,
        opFileTnMaster = 0xf400,
        opFileFnMaster = 0xf500,
        opFileFnReply = 0xf600,
        opIpProg = 0xf800,
        opIpProgReply = 0xf900,
        opMedia = 0x9000,
        opMediaPatch = 0x9100,
        opMediaControl = 0x9200,
        opMediaControlReply = 0x9300,
        opTimeCode = 0x9700,
        opTimeSync = 0x9800,
        opTrigger = 0x9900,
        opDirectory = 0x9a00,
        opDirectoryReply = 0x9b00,
    };

    /**
     * @brief struct to hold the configuration of the device
     */
    struct configuration {
        uint8_t ipAddress[ipAddressLen];
        uint8_t macAddress[macAddressLen];
        styleCodes deviceStyle;
        uint8_t bindIndex;
        uint8_t diagnosticPriority;
        uint16_t oemCode;
        bool targetModeActive;
        bool VLCActive;
        bool sendDiagAsUnicast;
        bool sendDiagostic;
        bool sendReplyOnChange;
    };

    /**
     * @brief struct representing the full address of a port
     */
    struct portAddress{
        unsigned int net        : 7;
        unsigned int subNet     : 4;
        unsigned int universe   : 4;
    };

    struct commonHeader{

    }__attribute__((__packed__));


    /**
     * @brief packet structure of an artPoll packet
     * 
     */
    struct artPollPacket{
        uint8_t ident[artNetIdentLen];
        uint16_t opCode;
        uint16_t protVer;
        struct{
            unsigned int padding : 2;
            unsigned int targetModeEnable : 1;
            unsigned int VLCEnable : 1;
            unsigned int diagMsgIsUnicast : 1;
            unsigned int sendDiagMsg : 1;
            unsigned int sendReplyOnChange : 1;
            unsigned int deprecated : 1;
        }__attribute__((__packed__)) flags;
        uint8_t diagPriority;
        uint16_t targetPortAddressTop;
        uint16_t targetPortAddressBottom;
        uint16_t estaMan;
        uint16_t oem;
    }__attribute__((__packed__));
    static_assert(sizeof(artPollPacket) == artPollPacketLen, "artPollPacket has incorrect size");




    //private storage stuff
    struct configuration sysConf;

    /**
     * @brief calculate the default ip as outlined in the ArtNet spec
     */
    void setDefaultIp();

    /**
     * @brief function to handle artPoll packets
     * 
     * @param packet pointer to the incoming packet
     * @param packetLen length of the incoming packet in bytes
     * 
     * @exception <packet is too small>
     * @exception <protocol version not supported>
     */
    void handleArtPoll(void *packet, uint16_t packetLen);

    bool (*callback_readNetSwitch)(void);

public:
    //constructors
    ArtNet(ArtNet &other) = delete;
    ArtNet(ArtNet &&other) = delete;
    ArtNet();
    ~ArtNet();

    /**
     * @brief function to update the ip of the device
     * 
     * @param newAddress new ip address
     * @param newAdressLen number of bytes in the new address
     * 
     * @exception <new address too short> new address too short, address not updated
     */
    void updateIp(uint8_t *newAddress, uint8_t newAdressLen);
};
