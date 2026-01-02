#include <ArtNetController.hpp>

void ArtNetController::handleArtPollReply(void *packet, uint16_t packetLen) {

    ArtPollReplyPacket *_packet_ptr = reinterpret_cast<ArtPollReplyPacket*> (packet);
}