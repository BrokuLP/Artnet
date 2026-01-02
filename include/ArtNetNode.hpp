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

public:
    ArtNetNode();
    ArtNetNode(ArtNetNode &other) = delete;
    ArtNetNode(ArtNetNode &&other) = delete;
    ~ArtNetNode();
};
 