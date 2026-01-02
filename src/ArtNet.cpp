#include <ArtNet.hpp>
#include <stdexcept>

ArtNet::ArtNet(uint16_t oemCode, uint8_t *MAC, uint8_t MACLen):oemCode(oemCode){

    if (MACLen < macAddressLen){
        throw std::runtime_error("invalid MAC");
    }

    for (uint8_t i = 0; i < macAddressLen; i++) {
        sysConf.macAddress[i] = MAC[i];
    }

    setDefaultIp();
};

void ArtNet::updateIp(uint8_t *newAddress, uint8_t newAddressLen){
    
    if (newAddressLen < ipAddressLen) {
        throw std::runtime_error("new address too short");
    }

    for (uint8_t i = 0; i < ipAddressLen; i++) {
        sysConf.ipAddress[i] = newAddress[i];
    }
}

void ArtNet::setDefaultIp() {

    if(callback_readNetSwitch()){
        sysConf.ipAddress[0] = 10;
    }
    else {
        sysConf.ipAddress[0] = 2;
    }

    sysConf.ipAddress[1] = static_cast <uint8_t> (sysConf.macAddress[3] + (oemCode && 0x00FF) + (oemCode >> 8));

    sysConf.ipAddress[2] = sysConf.macAddress[4];
    sysConf.ipAddress[3] = sysConf.macAddress[5];
};


void ArtNet::handleArtPoll(void *packet, uint16_t packetLen, uint8_t *senderIp, uint8_t senderIpLen) {

    if(packetLen < minArtPollLen){
        throw std::runtime_error("packet is too small");
    }

    artPollPacket *_packet_ptr = reinterpret_cast <artPollPacket*> (packet);

    if(_packet_ptr->protVer < minProtVersion || _packet_ptr->protVer > protVersion){
        throw std::runtime_error("protocol version is not supported");
    }

    sendArtPollReply(senderIp, senderIpLen);
}


void ArtNet::sendArtPollReply(uint8_t *targetIp, uint8_t targetIpLen) {

    ArtPollReplyPacket _packet;

    for (uint8_t i = 0; i < artNetIdentLen; i++) {
        _packet.artHeader.ident[i] = artNetIdent[i];
    }
    _packet.artHeader.opCode = opPollReply;
    
    for (uint8_t i = 0; i < ipAddressLen; i++) {
         _packet.ipAddress[i] = sysConf.ipAddress[i];
    }
    _packet.port = artNetPort;

    _packet.versionInfo = libraryVersion;
    
   _packet.subSwitch = sysConf.subSwitch;
   _packet.netSwitch = sysConf.netSwitch;

   _packet.oemCode = oemCode;
    
   /**
    * @TODO: finish packing of data -> need to implement other logic first
    */

    if(!callback_transmitUdp(reinterpret_cast<uint8_t*>(&_packet), sizeof(_packet), targetIp, targetIpLen, artNetPort)){
        throw std::runtime_error("failed to transmit packet");
    }
}

bool ArtNet::isConfigured() {
    
    if(callback_readNetSwitch == nullptr){
        return false;
    }

    if(callback_transmitUdp == nullptr){

    }



    return true;
}