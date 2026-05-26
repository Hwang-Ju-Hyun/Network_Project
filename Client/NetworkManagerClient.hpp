#pragma once
#include "NetworkManager.hpp"
#include "TCPSession.hpp"


class LinkingContext;
class InputMemoryStream;

class NetworkManagerClient: public NetworkManager
{
public:
    static NetworkManagerClient* sInstance;
    ~NetworkManagerClient()override{}
    virtual void Init()override;
public:    
    void ProcessPacket(InputMemoryStream& _inStream);
private:
    void HandleHello_Packet(InputMemoryStream& _inStream);
    void HandleReplication_Packet(InputMemoryStream& _inStream);
private:
    LinkingContext* m_LinkingContext;
public:
    LinkingContext* GetLinkingContext(){return m_LinkingContext;}
};