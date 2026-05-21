#pragma once
#include "NetworkManager.hpp"
#include "LinkingContext.hpp"
#include "Object.hpp"

class NetworkManagerServer: public NetworkManager
{
public:
    static NetworkManagerServer* sInstance;
    ~NetworkManagerServer()override{}
    virtual void Init()override;
public:
    virtual void ProcessPacket(ClientSession* _cs,InputMemoryStream& _stream);
private:
    void HandleHello_Packet(ClientSession* _session,InputMemoryStream& _instream);
    void SendHello_Packet(ClientSession* _session);
private:
    // 접속한 클라이언트들을 관리하는 명부 (ID -> 세션 )
    std::unordered_map<uint32_t,ClientSession*> m_SessionMap;
    static uint32_t nextSessionID;    
private:
    LinkingContext* m_LinkingContext; 
public:
    void RegisterObject(ObjectPtr _obj);   
    LinkingContext* GetLinkingContext()const{return m_LinkingContext;}

    void SendOutgoingReplicationPackets();
};