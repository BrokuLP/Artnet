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
    static constexpr uint16_t libraryVersion = 0;
    static constexpr uint16_t maxUrlLen = 64;
    static constexpr uint8_t defaultSupportUrl[34] = "https://github.com/BrokuLP/Artnet";

    static constexpr uint8_t artPollPacketLen   = 22;
    static constexpr uint16_t artIpProgPacketPacketLen   = 239;
    static constexpr uint8_t artIpProgPacketLen       = 33;
    static constexpr uint8_t artIpProgReplyLen  = 34;
    static constexpr uint8_t artAddressPacketLen      = 107;
    static constexpr uint8_t artDataRequestPacketLen = 40;
    static constexpr uint8_t artDataReplyPacketLen = 20;

    static constexpr uint16_t maxArtDataReplyPayloadLen = 512;

    static constexpr uint8_t ipAddressLen       = 4;
    static constexpr uint8_t macAddressLen      = 6;
    static constexpr uint16_t artNetPort        = 0x1936;
    static constexpr uint8_t artPollTimeOut     = 3; //seconds
    static constexpr uint8_t minArtPollLen      = 14;
    static constexpr uint16_t minProtVersion    = 14;
    static constexpr uint16_t protVersion       = 14;
    static constexpr uint8_t artNetIdentLen     = 8;
    static constexpr uint8_t minArtIpProgPacketLen = 207;

    static constexpr uint8_t artNetIdent[artNetIdentLen] = {'A', 'r', 't','-','N','e', 't', 0x00};

    const uint16_t oemCode;
    
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
        opPoll              = 0x2000,
        opPollReply         = 0x2100,
        opDiagData          = 0x2300,
        opCommand           = 0x2400,
        opDataRequest       = 0x2700,
        opDataReply         = 0x2800,
        opDmx               = 0x5000,
        opNzs               = 0x5100,
        opSync              = 0x5200,
        opAddress           = 0x6000,
        opInput             = 0x7000,
        opTodRequest        = 0x8000,
        opTodData           = 0x8100,
        opTodControl        = 0x8200,
        opRdm               = 0x8300,
        opRdmSub            = 0x8400,
        opVideoSetup        = 0xa010,
        opVideoPalette      = 0xa020,
        opVideoData         = 0xa040,
        opMacMaster         = 0xf000, //deprecated
        opMacSlave          = 0xf100, //deprecated
        opFirmwareMaster    = 0xf200,
        opFirmwareReply     = 0xf300,
        opFileTnMaster      = 0xf400,
        opFileFnMaster      = 0xf500,
        opFileFnReply       = 0xf600,
        opIpProg            = 0xf800,
        opIpProgReply       = 0xf900,
        opMedia             = 0x9000,
        opMediaPatch        = 0x9100,
        opMediaControl      = 0x9200,
        opMediaControlReply = 0x9300,
        opTimeCode          = 0x9700,
        opTimeSync          = 0x9800,
        opTrigger           = 0x9900,
        opDirectory         = 0x9a00,
        opDirectoryReply    = 0x9b00,
    };

    /**
     * @brief enum of possible node report codes
     * 
     */
    enum nodeReportCodes {
        rcDebug         = 0x0000,
        rcPowerOk       = 0x0001,
        rcPowerFail     = 0x0002,
        rcSocketWr1     = 0x0003,
        rcParseFail     = 0x0004,
        rcUdpFail       = 0x0005,
        rcShNameOk      = 0x0006,
        rcLoNameOk      = 0x0007,
        rcDmxError      = 0x0008,
        rcDmxUdpFull    = 0x0009,
        rcDmxRxFull     = 0x000a,
        rcSwitchErr     = 0x000b,
        rcConfigErr     = 0x000c,
        rcDmxShort      = 0x000d,
        rcFirmwareFail  = 0x000e,
        rcUserFail      = 0x000f,
        rcFactoryRes    = 0x0010,
    };

    /**
     * @brief enum with possible indicator states for artNetPollReply
     * 
     */
    enum indicatorStates {
        isUnkown    = 0b00,
        isLocate    = 0b01,
        isMute      = 0b10,
        isNormal    = 0b11,
    };

    /**
     * @brief enum with possible port programming authority codes for artNetPollReply
     * 
     */
    enum portProgAuthorityCodes {
        ppacUnknown     = 0b00,
        ppacFrontPanel  = 0b01,
        ppacNetwork     = 0b10,
    };

    enum portTypeCodes {
        ptcDMX512    = 0b000000,
        ptcMIDI      = 0b000001,
        ptcAvab      = 0b000010,
        ptcColortran = 0b000011,
        ptcADB625    = 0b000100,
        ptcArtNet    = 0b000101,
        ptcDali      = 0b000110,
    };

    enum failSafeStates {
        fssLastState    = 0b00,
        fssOutputZero   = 0b01,
        fssOutputFull   = 0b10,
        fssPlayScene    = 0b11,
    };

    enum backgroundQueuePolicys {
        bqpStatusNone       = 0,
        bqpStatusAdvisory   = 1,
        bqpStatusWarning    = 2,
        bqpStatusError      = 3,
        bqpDisabled         = 4,
    };

    /**
     * @brief possible data requests to be send to the device
     */
    enum dataRequestCodes {
        drPoll = 0x0000,
        drUrlProduct = 0x0001,
        drUrlUserGuide = 0x0002,
        drUrlSupport = 0x0003,
        drUrlPersUdr = 0x0004,
        drUrlPersGdtf = 0x0005,
        drStartManSpec = 0x8000,
    };

    /**
     * @brief possible commands to be used in ArtAddressPackets
     */
    enum artAddressCommands {
        acNone          = 0x00,
        acCancelMerge   = 0x01,
        acLedNormal     = 0x02,
        acLedMute       = 0x03,
        acLedLocate     = 0x04,
        acResetRxFlags  = 0x05,
        acAnalysisOn    = 0x06, 
        acAnalysesOff   = 0x07, 
        acFailHold      = 0x08,
        acFailZero      = 0x09,
        acFailFull      = 0x0a,
        acFailScene     = 0x0b,
        acFailRecord    = 0x0c,
        acMergeLtp0     = 0x10,    
        acMergeLtp1     = 0x11,
        acMergeLtp2     = 0x12,
        acMergeLtp3     = 0x13,
        acDirectionTx0  = 0x20,
        acDirectionTx1  = 0x21,
        acDirectionTx2  = 0x22,
        acDirectionTx3  = 0x23,
        acDirectionRx0  = 0x30,
        acDirectionRx1  = 0x31,
        acDirectionRx2  = 0x32,
        acDirectionRx3  = 0x33,
        acMergeHtp0     = 0x50,
        acMergeHtp1     = 0x51,
        acMergeHtp2     = 0x52,
        acMergeHtp3     = 0x53,
        acArtNetSel0    = 0x60,
        acArtNetSel1    = 0x61,
        acArtNetSel2    = 0x62,
        acArtNetSel3    = 0x63,
        acAcnSel0       = 0x70,
        acAcnSel1       = 0x71,
        acAcnSel2       = 0x72,
        acAcnSel3       = 0x73,
        acClearOp0      = 0x90,
        acClearOp1      = 0x91,
        acClearOp2      = 0x92,
        acClearOp3      = 0x93,
        acStyleDelta0   = 0xa0,
        acStyleDelta1   = 0xa1,
        acStyleDelta2   = 0xa2,
        acStyleDelta3   = 0xa3,
        acStyleConst0   = 0xb0,
        acStyleConst1   = 0xb1,
        acStyleConst2   = 0xb2,
        acStyleConst3   = 0xb3,
        acRdmEnable0    = 0xc0,
        acRdmEnable1    = 0xc1,
        acRdmEnable2    = 0xc2,
        acRdmEnable3    = 0xc3,
        acRdmDisable0   = 0xd0,
        acRdmDisable1   = 0xd1,
        acRdmDisable2   = 0xd2,
        acRdmDisable3   = 0xd3,
        acBqp0          = 0xe0,
        acBqp1          = 0xe1,
        acBqp2          = 0xe2,
        acBqp3          = 0xe3,
        acBqp4          = 0xe4,
        acBqp5          = 0xe5,
        acBqp6          = 0xe6,
        acBqp7          = 0xe7,
        acBqp8          = 0xe8,
        acBqp9          = 0xe9,
        acBqp10         = 0xea,
        acBqp11         = 0xeb,
        acBqp12         = 0xec,
        acBqp13         = 0xed,
        acBqp14         = 0xee,
        acBqp15         = 0xef,
    };

    /**
     * @brief configuration of a specific port on the node
     */
    struct portConfig {
        uint8_t universe;
        bool isInput;
        bool isOutput;
    };

    /**
     * @brief struct to hold the configuration of the device
     */
    struct configuration {
        uint8_t urlProduct[maxUrlLen];
        uint8_t urlUserGuide[maxUrlLen];
        uint8_t urlSupport[maxUrlLen];
        uint8_t urlPersGdtf[maxUrlLen];
        uint8_t urlPersUdr[maxUrlLen];
        portConfig ports[4];
        uint8_t ipAddress[ipAddressLen];
        uint8_t macAddress[macAddressLen];
        styleCodes deviceStyle;
        uint16_t oemCode;
        uint8_t subSwitch;
        uint8_t netSwitch;
        uint8_t bindIndex;
        uint8_t diagnosticPriority;
        bool targetModeActive;
        bool VLCActive;
        bool sendDiagAsUnicast;
        bool sendDiagostic;
        bool sendReplyOnChange;
    };

    struct commonHeader{
        uint8_t ident[artNetIdentLen];
        uint16_t opCode;
    }__attribute__((__packed__));
    static_assert(sizeof(commonHeader) == 10, "common header has invalid size");

    /**
     * @brief packet structure of an artPoll packet
     * 
     */
    struct artPollPacket{
        commonHeader artHeader;
        uint16_t protVer;
        struct{
            unsigned int padding            : 2;
            unsigned int targetModeEnable   : 1;
            unsigned int VLCEnable          : 1;
            unsigned int diagMsgIsUnicast   : 1;
            unsigned int sendDiagMsg        : 1;
            unsigned int sendReplyOnChange  : 1;
            unsigned int deprecated         : 1;
        }__attribute__((__packed__)) flags;
        uint8_t diagPriority;
        uint16_t targetPortAddressTop;
        uint16_t targetPortAddressBottom;
        uint16_t estaMan;
        uint16_t oem;
    }__attribute__((__packed__));
    static_assert(sizeof(artPollPacket) == artPollPacketLen, "artPollPacket has incorrect size");

    struct ArtPollReplyPacket{
        commonHeader artHeader;
        uint8_t ipAddress[4];
        uint16_t port;
        uint16_t versionInfo;
        uint8_t netSwitch;
        uint8_t subSwitch;
        uint16_t oemCode;
        uint8_t ubeaVersion;
        struct {
            unsigned int indicatorState     : 2;
            unsigned int portProgAuthority  : 2;
            unsigned int padding            : 1;
            unsigned int romBootEnable      : 1;
            unsigned int rdmAvailable       : 1;
            unsigned int ubeaPresent        : 1;
        }__attribute__((__packed__)) status1;
        uint16_t estaMan;
        uint8_t portName[18];
        uint8_t longName[64];
        uint8_t nodeReport[64];
        uint16_t numPorts;
        struct {
            unsigned int isInput    : 1;
            unsigned int isOutput   : 1;
            unsigned int type       : 5;
        }__attribute__((__packed__)) portTypes[4];
        struct {
            unsigned int dataReceived       : 1;
            unsigned int dmx512HasTestPkg   : 1;
            unsigned int dmx512HasSIP       : 1;
            unsigned int dmx512HasTextPkg   : 1;
            unsigned int inputDisabled      : 1;
            unsigned int receiveErrors      : 1;
            unsigned int padding            : 1;
            unsigned int convertToSacn      : 1;
        }__attribute__((__packed__)) goodInput[4];
        struct {
            unsigned int isActive           : 1;
            unsigned int dmx512HasTestPkg   : 1;
            unsigned int dmx512HasSIP       : 1;
            unsigned int dmx512HasTextPkg   : 1;
            unsigned int isMergingArtNet    : 1;
            unsigned int shortCircuitDetect : 1;
            unsigned int ltpIsMergeMode     : 1;
            unsigned int convertFromSacn    : 1;
        }__attribute__((__packed__)) goodOutputA[4];
        uint8_t swIn[4];
        uint8_t swOut[4];
        uint8_t acnPriority;
        struct {
            unsigned int macro8Active   : 1;
            unsigned int macro7Active   : 1;
            unsigned int macro6Active   : 1;
            unsigned int macro5Active   : 1;
            unsigned int macro4Active   : 1;
            unsigned int macro3Active   : 1;
            unsigned int macro2Active   : 1;
            unsigned int macro1Active   : 1;
        }__attribute__((__packed__)) swMacro;
        struct {
            unsigned int remote8Active  : 1;
            unsigned int remote7Active  : 1;
            unsigned int remote6Active  : 1;
            unsigned int remote5Active  : 1;
            unsigned int remote4Active  : 1;
            unsigned int remote3Active  : 1;
            unsigned int remote2Active  : 1;
            unsigned int remote1Active  : 1;
        }__attribute__((__packed__)) swRemote;
        uint8_t spare[3];
        uint8_t style;
        uint8_t MAC[6];
        uint8_t bindIp[4];
        uint8_t bindIndex;
        struct {
            unsigned int supportsRDM        : 1;
            unsigned int supportsSwitching  : 1;
            unsigned int isSquawking        : 1;
            unsigned int sACNSupported      : 1;
            unsigned int longAddressSupport : 1;
            unsigned int capableOfDHCP      : 1;
            unsigned int isDHCPConfigured   : 1;
            unsigned int supportsWebConf    : 1;
        }__attribute__((__packed__)) status2;
        struct {
            unsigned int rdmDisabled            : 1;
            unsigned int outputStyleContinuous  : 1;
            unsigned int discoveryNotRunning    : 1;
            unsigned int bckgDiscoveryDisabled  : 1;
            unsigned int padding                : 4;
        }__attribute__((__packed__)) goodOutputB[4];
        struct {
            unsigned int failSafeState          : 2;
            unsigned int progFailSafeSupported  : 1;
            unsigned int llrpSupported          : 1;
            unsigned int portDirSwSupported     : 1;
            unsigned int rdmNetSupported        : 1;
            unsigned int bckgQueueSupported     : 1;
            unsigned int bckgDisConfSupport     : 1;
        }__attribute__((__packed__)) status3;
        uint8_t defaultRespUid[6];
        uint16_t user;
        uint16_t refreshRate;
        uint8_t backgroundQueuePolicy;
        uint8_t Filler[10];
    }__attribute__((__packed__));
    static_assert(sizeof(ArtPollReplyPacket) == artIpProgPacketPacketLen, "ArtPollReplyPacket has invalid size");

    struct ArtIpProgPacket {
        commonHeader artHeader;
        uint16_t protVersion;
        uint8_t filler[2];
        struct {
            unsigned int programmingEnable  : 1;
            unsigned int dhcpEnable         : 1;
            unsigned int padding            : 1;
            unsigned int progDefaultGateWay : 1;
            unsigned int resetToDefault     : 1;
            unsigned int programIpAddress   : 1;
            unsigned int programSubNetMask  : 1;
            unsigned int programPort        : 1;
        }__attribute__((__packed__)) command;
        uint8_t filler4;
        uint8_t progIp[4];
        uint8_t progSm[4];
        uint8_t progDg[4];
        uint8_t spare[5];
    }__attribute__((__packed__));
    static_assert(sizeof(ArtIpProgPacket) == artIpProgPacketLen, "ArtIpProgPacket has invalid size");
    
    struct ArtIpProgReply{
        commonHeader artHeader;
        uint16_t protVersion;
        uint8_t filler[4];
        uint8_t progIp[4];
        uint8_t progSm[4];
        uint16_t progPort; //deprecated
        struct {
            unsigned int padding        : 1;
            unsigned int dhcpEnabled    : 1;
            unsigned int padding1       : 6;
        }__attribute__((__packed__)) status;
        uint8_t spare2;
        uint8_t progDg[4];
        uint8_t spare3[2];
    }__attribute__((__packed__));
    static_assert(sizeof(ArtIpProgReply) == artIpProgReplyLen, "ArtIpProgReply has invalid size");

    struct ArtAddressPacket {
        commonHeader artHeader;
        uint16_t protVersion;
        uint8_t netSwitch;
        uint8_t bindIndex;
        uint8_t portName[18];
        uint8_t longName[64];
        uint8_t swIn[4];
        uint8_t swOut[4];
        uint8_t subSwitch;
        uint8_t acnPriority;
        uint8_t command;
    }__attribute__((__packed__));
    static_assert(sizeof(ArtAddressPacket) == artAddressPacketLen, "ArtAddressPacket has invalid size");
    
    struct ArtDataRequestPacket {
        commonHeader artHeader;
        uint16_t protVersion;
        uint16_t estaMan;
        uint16_t oemCode;
        uint16_t request;
        uint8_t spare[22];
    }__attribute__((__packed__));
    static_assert(sizeof(ArtDataRequestPacket) == artDataRequestPacketLen, "ArtDataRequestPacket has invalid size");

    struct ArtDataReplyPacket {
        commonHeader artHeader;
        uint16_t protVersion;
        uint16_t estaMan;
        uint16_t oemCode;
        uint16_t request;
        uint16_t payloadLen;
    }__attribute__((__packed__));
    static_assert(sizeof(ArtDataReplyPacket) == artDataReplyPacketLen, "artDataReplyPacket has invalid size");

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
    void handleArtPoll(void *packet, uint16_t packetLen, uint8_t *senderIp, uint8_t senderIpLen);

    /**
     * @brief function to transmit an ArtNetPollReply packet
     * 
     * @param targetIp the IPv4 of the controller to respond to
     * @param targetIpLen number of bytes in the controller IP
     * 
     * @exception <failed to transmit packet> -> udp transmission callback returned an error
     */
    void sendArtPollReply(uint8_t *targetIp, uint8_t targetIpLen);

    bool (*callback_readNetSwitch)(void) = nullptr;
    bool (*callback_transmitUdp)(uint8_t *packet, uint16_t packetLen, uint8_t *targetIp, uint8_t targetIpLen, uint16_t targetPort) = nullptr;

    /**
     * @brief function to check if all required parameters are configured
     */
    bool isConfigured();

public:
    //constructors
    ArtNet(ArtNet &other) = delete;
    ArtNet(ArtNet &&other) = delete;
    ArtNet(uint16_t oemCode, uint8_t *MAC, uint8_t MACLen);
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

    /**
     * @brief function to handle packets, has to implemented by device type classes
     * 
     * @param packet        pointer to the incoming packet
     * @param packetLen     length of the incomin packet
     * @param senderIp      pointer to the ip of the sender
     * @param senderIpLen   number of bytes in the ip of the sender
     * @param port          port the packet was received on
     */
    void virtual handlePacket(void *packet, uint16_t packetLen, uint8_t *senderIp, uint8_t senderIpLen, uint16_t port);
};
