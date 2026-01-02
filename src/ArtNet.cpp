#include <ArtNet.hpp>
#include <stdexcept>

void ArtNet::updateIp(uint8_t *newAddress, uint8_t newAddressLen){
    
    if (newAddressLen < ipAddressLen) {
        throw std::runtime_error("new address too short");
    }

    for (uint8_t i = 0; i < ipAddressLen; i++) {
        sysConf.ipAddress[i] = newAddress[i];
    }
}


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