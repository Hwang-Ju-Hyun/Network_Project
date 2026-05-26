#include "NetworkManagerServer.hpp"
#include "MemoryStream.hpp"
#include "ObjectRegistry.hpp"
#include "ReplicationManagerServer.hpp"
#include "ClientProxy.hpp"
#include "TCPSession.hpp"
#include <cassert>
#include "RoboServer.hpp"

NetworkManagerServer* NetworkManagerServer::sInstance=nullptr;

uint32_t NetworkManagerServer::nextSessionID=1;

void  NetworkManagerServer::Init()
{
    m_LinkingContext=new LinkingContext;
    ObjectRegistry::sInstance->RegisterCreationFunction('ROBO',RoboServer::StaticCreate);
}

void NetworkManagerServer::ProcessPacket(ClientProxy* _session,InputMemoryStream& _inStream) 
{
    uint8_t packet_type;
    _inStream.Read(packet_type);
    switch (packet_type)
    {
    case PT_Hello:        
        HandleHello_Packet(_session,_inStream);
        break;
    case PT_Replication:   
        break;
    case PT_MAZE_DATA:
        /* code */
        break;
    case PT_Disconnected:
        /* code */
        break;        
    case PT_INPUT:
    {
        HandleInput_Packet(_session,_inStream);       
    }        
        break;
    default:  
        printf("Inavalid PacketData\n\a");
        break;
    }
}

void NetworkManagerServer::HandleHello_Packet(ClientProxy* _proxy,InputMemoryStream& _instream)
{                
    uint32_t newClientSessionID=nextSessionID;
    
    m_SessionIdToProxyMap[newClientSessionID]=_proxy;    
    _proxy->SetSessionID(newClientSessionID);


    std::cout<<"[서버] 새 클라이언트 접속 승인! 부여된 ID : "<<newClientSessionID<<std::endl;            

    ObjectPtr newRobo = ObjectRegistry::sInstance->CreateObject('ROBO');

    RegisterObject(newRobo);    
    _proxy->SetPossessedNetworkID( newRobo->GetNetworkID() );
    
    
    SendHello_Packet(_proxy,newRobo);

    nextSessionID++;
}

void NetworkManagerServer::SendHello_Packet(ClientProxy* _proxy,ObjectPtr _obj)
{
    OutputMemoryStream outStream;
    uint8_t packetType=PacketType::PT_Hello;    

    outStream.Write(packetType);
    uint32_t a = m_LinkingContext->GetNetworkID(_obj);
    outStream.Write(a);
    
    outStream.Write(_proxy->GetSessionID());

    _proxy->SendPacket(outStream);    
}


void NetworkManagerServer::HandleInput_Packet(ClientProxy* _session, InputMemoryStream& _inStream)
{
    int moveX = 0, moveY = 0;
    _inStream.Read(moveX);
    _inStream.Read(moveY);

        // 핑이 오는지 확인용 대형 로그!
        std::cout << "\n===================================" << std::endl;
        std::cout << "[서버 수신] 클라이언트 이동 명령 도착!" << std::endl;
        std::cout << "moveX : " << moveX << " / moveY : " << moveY << std::endl;
        std::cout << "===================================\n" << std::endl;

    // 1. "누구 파이프에서 온 거지? 아, _session(2번 손님)이 보냈네!"
    // 2. "2번 손님이 조종하는 로봇 번호가 뭐였지?"
    uint32_t myRoboID = _session->GetPossessedNetworkID();

    // 3. 호적부에서 내 로봇 찾기
    ObjectPtr myRobo = m_LinkingContext->GetObject(myRoboID);
    
    if (myRobo != nullptr)
    {
        // 4. 찾았다! 위치를 이동시키자!
        int currentX = myRobo->GetPosX();
        int currentY = myRobo->GetPosY();
        
        myRobo->SetPos(currentX + moveX, currentY + moveY);

        // 5. 서버 매니저한테 상태 변했다고 장부에 적어두라고 지시 (다음 틱에 뿌려짐)
        _session->GetReplicationManagerServer().SetStateDirty(myRoboID);        
    }
}

void NetworkManagerServer::OnClientAccepted(TCPSocketPtr _tcpSocket)
{
    TCPSessionPtr newClientSession = std::make_shared<TCPSession>();
    newClientSession->SetSocket(_tcpSocket);
    ClientProxyPtr newClientProxy=std::make_shared<ClientProxy>(newClientSession.get(),0);

    m_PendingProxies.push_back(newClientProxy);    
}


void NetworkManagerServer::RegisterObject(ObjectPtr _obj)
{
    uint32_t networkID = m_LinkingContext->GenerateNewNextNeworkID();
    _obj->SetNetworkID(networkID);
    m_LinkingContext->AddObject(_obj,networkID);    

    //이제 접속한 모든 손님들의 '개인 장부'에 "야, 이거 새로 만들어라"라고 적어둡니다.
    for(auto iter=m_SessionIdToProxyMap.begin();iter!=m_SessionIdToProxyMap.end();iter++)
    {    
        ClientProxy* session=iter->second;        
        session->GetReplicationManagerServer().ReplicateCreate(networkID);
    }
}


void NetworkManagerServer::SendOutgoingReplicationPackets()
{
    for(auto iter = m_SessionIdToProxyMap.begin();iter!=m_SessionIdToProxyMap.end();iter++)
    {
        ClientProxy* proxy = iter->second;
        OutputMemoryStream replicateStream;
        PacketType packetType = PacketType::PT_Replication;
        uint8_t packetTypeByte=static_cast<uint8_t>(packetType);

        replicateStream.Write(packetTypeByte);  
              
        proxy->GetReplicationManagerServer().Write(replicateStream);

        //리플리케이션 장부가 비워져있지 않을때만 보내겠다.
        if(replicateStream.GetLength()>sizeof(uint8_t))
        {
            proxy->SendPacket(replicateStream);
        }
    }
}