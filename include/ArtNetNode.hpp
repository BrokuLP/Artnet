/**
 * @file ArtNetNode.hpp
 * @author your name (you@domain.com)
 * @brief extends ArtNet by the functionality required for an ArtNetNode
 * @version 0.1
 * @date 2026-01-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

 #include <ArtNet.hpp>
 #include <stdint.h>


class ArtNetNode : public ArtNet{
private:

    /**
     * @brief function pointer to output dmx data to the corresponding port
     * @param dmxData dmx data to output
     * @param dmxDataSize number of channels of dmx data
     * @param portIdx port to output data on, valid range 0:3
     * @retval true -> succeeded to output data
     * @retval false -> failed to output data
     */
    bool (*callback_outputDmx)(uint8_t *dmxData, uint16_t dmxDataSize, uint8_t portIdx);
public:
    ArtNetNode();
    ArtNetNode(ArtNetNode &other) = delete;
    ArtNetNode(ArtNetNode &&other) = delete;
    ~ArtNetNode();
};
 