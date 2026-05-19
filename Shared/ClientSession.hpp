#include "header.hpp"
#include "TCPSocket.hpp"
#include <vector>

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
    void HandlePacket(PacketType _pt,InputMemoryStream& _stream);
    void SendPacket(OutputMemoryStream& _payLoadStream);
public:
    TCPSocketPtr GetSocket()const{return m_Socket;}
    uint32_t GetSessionID()const{return m_SessionID;}    
};

using ClientSessionPtr=std::shared_ptr<ClientSession>;