#include "NetworkManagerServer.hpp"
#include "MemoryStream.hpp"
#include "ClientSession.hpp"
#include "ObjectRegistry.hpp"
#include "ReplicationManagerServer.hpp"
#include "ClientSession.hpp"

#include <cassert>

NetworkManagerServer* NetworkManagerServer::sInstance=nullptr;

uint32_t NetworkManagerServer::nextSessionID=1;

void  NetworkManagerServer::Init()
{
    m_LinkingContext=new LinkingContext;
}

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

void NetworkManagerServer::RegisterObject(ObjectPtr _obj)
{
    int networkID = m_LinkingContext->GenerateNewNextNeworkID();
    _obj->SetNetworkID(networkID);
    m_LinkingContext->AddObject(_obj.get(),networkID);
    

    //이제 접속한 모든 손님들의 '개인 장부'에 "야, 이거 새로 만들어라"라고 적어둡니다.
    for(auto iter=m_SessionMap.begin();iter!=m_SessionMap.end();iter++)
    {    
        ClientSession* session=iter->second;
        session->m_ReplicationManager.ReplicateCreate(networkID);
    }
}


void NetworkManagerServer::SendOutgoingReplicationPackets()
{
    for(auto iter = m_SessionMap.begin();iter!=m_SessionMap.end();iter++)
    {
        ClientSession* session = iter->second;
        OutputMemoryStream replicateStream;
        PacketType packetType = PacketType::PT_Replication;
        uint8_t packetTypeByte=static_cast<uint8_t>(packetType);

        replicateStream.Write(packetTypeByte);
        session->m_ReplicationManager.Write(replicateStream);

        //리플리케이션 장부가 비워져있지 않을때만 보내겠다.
        if(replicateStream.GetLength()>sizeof(uint8_t))
        {
            session->SendPacket(replicateStream);
        }    
    }
}