/**
 * @file ArtNetController.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-12-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <ArtNet.hpp>



class ArtNetController : private ArtNet{
private:
    
    /**
     * @brief function to handle incoming artPollReplyPacket
     * 
     * @param packet pointer to the incoming packet
     * @param packetLen length of the incoming packet in bytes
     */
    void handleArtPollReply(void *packet, uint16_t packetLen);

    bool (*callback_broadcast)(uint8_t *packet, uint8_t packetLen, uint16_t port);

public:
    ArtNetController();
    ArtNetController(ArtNetController &other) = default;
    ArtNetController(ArtNetController &&other) = default;
    ~ArtNetController();
};
