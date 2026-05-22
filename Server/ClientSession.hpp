#pragma once
#include "TCPSocket.hpp"
#include <vector>

class ClientSession;
class ReplicationManagerServer;
class SocketAddress;
class InputMemoryStream;
class OutputMemoryStream;

// 클라이언트 연결과 그 클라이언트 전용 수신 버퍼를 관리하는 클래스
class ClientSession
{
private:
    TCPSocketPtr m_Socket;
    uint32_t m_SessionID;    
    // 수신 버퍼: TCP 특성상 쪼개져서 오는 데이터를 임시로 모아두는 공간    
    std::vector<char> m_ReceiveBuffer;    
public:    
    ClientSession(TCPSocketPtr _socket,uint32_t _sessionID);
    bool ProcessIncomingData();    
    void SendPacket(OutputMemoryStream& _payLoadStream);
public:
    TCPSocketPtr GetSocket()const{return m_Socket;}
    uint32_t GetSessionID()const{return m_SessionID;}          

    void SetSessionID(uint32_t _sessionID){m_SessionID=_sessionID;} 
private:
    ReplicationManagerServer* m_ReplicationManager;
public:
    void SetReplicationManager(ReplicationManagerServer* _rms){m_ReplicationManager=_rms;}
    ReplicationManagerServer* GetReplicaionManager() const{return m_ReplicationManager;}
};

using ClientSessionPtr=std::shared_ptr<ClientSession>;