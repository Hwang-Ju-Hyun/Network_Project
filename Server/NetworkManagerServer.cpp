#include "NetworkManagerServer.hpp"
#include "MemoryStream.hpp"
#include "ClientSession.hpp"
#include "ObjectRegistry.hpp"
#include <cassert>

NetworkManagerServer* NetworkManagerServer::sInstance=nullptr;

uint32_t NetworkManagerServer::nextSessionID=1;

void NetworkManagerServer::ProcessPacket(ClientSession* _session,InputMemoryStream& _inStream) 
{
    uint8_t packet_type;
    _inStream.Read(packet_type);
    switch (packet_type)
    {
    case PT_Hello:        
        HandleHello_Packet(_session,_inStream);
        break;
    case PT_Replication:
        /* code */
        break;
    case PT_MAZE_DATA:
        /* code */
        break;
    case PT_Disconnected:
        /* code */
        break;        
    default:  
        printf("Inavalid PacketData\n\a");
        break;
    }
}

void NetworkManagerServer::HandleHello_Packet(ClientSession* _session,InputMemoryStream& _instream)
{                
    uint32_t newClientSessionID=nextSessionID;
    
    _session->SetSessionID(newClientSessionID);

    m_SessionMap[newClientSessionID]=_session;    

    std::cout<<"[서버] 새 클라이언트 접속 승인! 부여된 ID : "<<newClientSessionID<<std::endl;
    //printf("[서버] 새 클라이언트 접속 승인! 부여된 ID : %d",newClientSessionID);       
    
    nextSessionID++;

    SendHello_Packet(_session);
}

void NetworkManagerServer::SendHello_Packet(ClientSession* _session)
{
    OutputMemoryStream outStream;
    uint8_t packetType=PacketType::PT_Hello;
    outStream.Write(packetType);
    outStream.Write(_session->GetSessionID());
    _session->SendPacket(outStream);
}