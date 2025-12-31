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


void ArtNet::handleArtPoll(void *packet, uint16_t packetLen) {

    if(packetLen < minArtPollLen){
        throw std::runtime_error("packet is too small");
    }

    artPollPacket *_packet_ptr = reinterpret_cast <artPollPacket*> (packet);

    if(_packet_ptr->protVer < minProtVersion || _packet_ptr->protVer > protVersion){
        throw std::runtime_error("protocol version is not supported");
    }

    /**
     * @TODO: write transmission of reply
     * 
     */
}