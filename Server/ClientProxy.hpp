#pragma once
#include "ReplicationManagerServer.hpp"
#include <memory>

//네트워크와 완전히 분리된, 오직 서버에만 존재하는 'Object 아바타'입니다.

class TCPSession;
class OutputMemoryStream;

class ClientProxy
{
private:
    TCPSession* m_TCPSession;
    uint32_t m_SessionID;
    ReplicationManagerServer m_ReplicationManagerServer;
public:
    ClientProxy(TCPSession* _session,uint32_t _sessionID);
    ~ClientProxy();
    ReplicationManagerServer& GetReplicationManagerServer(){return m_ReplicationManagerServer;}
public:
    void SetSessionID(uint32_t _sessionID){m_SessionID=_sessionID;}
    const uint32_t GetSessionID()const{return m_SessionID;}
    void SendPacket(OutputMemoryStream& _inStream);  
    TCPSession* GetSession(){return m_TCPSession;}

    friend class TCPSession;
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;