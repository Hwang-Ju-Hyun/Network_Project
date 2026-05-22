#include "ClientProxy.hpp"
#include "TCPSession.hpp"
#include "MemoryStream.hpp"
#include "NetworkManagerServer.hpp"

ClientProxy::ClientProxy(TCPSession* _session,uint32_t _sessionID)
    :m_TCPSession(_session)
    ,m_SessionID(_sessionID)
{            
    m_TCPSession->OnPacketReceived=[this](InputMemoryStream& inStream){NetworkManagerServer::sInstance->ProcessPacket(this,inStream);};
}

ClientProxy::~ClientProxy()
{
    delete m_TCPSession;
    m_TCPSession=nullptr;
}

void ClientProxy::SendPacket(OutputMemoryStream& _inStream)
{
    m_TCPSession->SendPacket(_inStream);
}