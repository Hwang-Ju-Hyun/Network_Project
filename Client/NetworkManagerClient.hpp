#pragma once
#include "NetworkManager.hpp"

class LinkingContext;

class NetworkManagerClient: public NetworkManager
{
public:
    static NetworkManagerClient* sInstance;
    ~NetworkManagerClient()override{}
    virtual void Init()override;
public:    
    virtual void ProcessPacket(ClientSession* _session,InputMemoryStream& _inStream)override;
private:
    void HandleHello_Packet(ClientSession* _session,InputMemoryStream& _inStream);
    void HandleReplication_Packet(ClientSession* _session,InputMemoryStream& _inStream);
private:
    LinkingContext* m_LinkingContext;
};