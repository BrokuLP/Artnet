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



class ArtNetController : public ArtNet{
private:
    
    /**
     * @brief function to handle incoming artPollReplyPacket
     * 
     * @param packet pointer to the incoming packet
     * @param packetLen length of the incoming packet in bytes
     */
    void handleArtPollReply(void *packet, uint16_t packetLen);
    
public:
    ArtNetController(/* args */);
    ~ArtNetController();
};
