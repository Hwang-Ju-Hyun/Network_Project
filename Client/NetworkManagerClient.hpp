#pragma once
#include "NetworkManager.hpp"

class NetworkManagerClient: public NetworkManager
{
public:
    static NetworkManagerClient* sInstance;
public:
    virtual void ProcessPacket(ClientSession* _session,InputMemoryStream& _inStream)override;
private:
    void HandleHello_Packet(ClientSession* _session,InputMemoryStream& _inStream);
};