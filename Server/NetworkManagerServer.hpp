#pragma once
#include "NetworkManager.hpp"
#include "Object.hpp"
#include "TCPSocket.hpp"
#include "ClientProxy.hpp"


class LinkingContext;

class NetworkManagerServer: public NetworkManager
{
public:
    static NetworkManagerServer* sInstance;
    ~NetworkManagerServer()override{}
    virtual void Init()override;
public:
    void ProcessPacket(ClientProxy* _cs,InputMemoryStream& _stream);
private:
    void HandleHello_Packet(ClientProxy* _proxy,InputMemoryStream& _instream);
    void SendHello_Packet(ClientProxy* _proxy);
private:
    std::vector<ClientProxyPtr> m_PendingProxies;
    // 접속한 클라이언트들을 관리하는 명부 (ID -> 세션 )    
    std::unordered_map<uint32_t,ClientProxy*> m_SessionIdToProxyMap;
    static uint32_t nextSessionID;    
private:
    LinkingContext* m_LinkingContext; 
public: 
    void OnClientAccepted(TCPSocketPtr _tcpSocket);
    std::vector<ClientProxyPtr> GetPendingProxies()const{return m_PendingProxies;}
public:
    void RegisterObject(ObjectPtr _obj);
    LinkingContext* GetLinkingContext()const{return m_LinkingContext;}
    void SendOutgoingReplicationPackets();
};