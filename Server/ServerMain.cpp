#include <iostream>
#include "TCPSocket.hpp"
#include "SocketUtil.hpp"
#include <cassert>
#include "SocketAddressFactory.hpp"
#include "ClientSession.hpp"
#include "NetworkManager.hpp"
#include "NetworkManagerServer.hpp"
#include "Server.hpp"
#include "ObjectRegistry.hpp"
#include "ReplicationManagerServer.hpp"

bool g_LOOP=true;

int main()
{        
    SocketAddressPtr serverAddr = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:9999");    
    TCPSocketPtr sockServerTcp=SocketUtil::CreateTCPSocket(AF_INET);

    assert(sockServerTcp->Bind(*serverAddr)!=ERROR);

    assert(sockServerTcp->Listen()!=ERROR);

    std::cout<<"Server: Checking for data"<<std::endl<<std::endl;
    std::vector<TCPSocketPtr> readBlockSockets;
    std::vector<TCPSocketPtr> readAbleSockets;
    std::vector<ClientSessionPtr> clientSessions;

    readBlockSockets.push_back(sockServerTcp);

    uint32_t nextClientSessionID=1;

    NetworkManager::sInstance = new NetworkManagerServer();    
    NetworkManager::GetInstance()->Init();

    ObjectRegistry::sInstance->StaticInit();

    Server s;
    while(g_LOOP)
    {

        if(!SocketUtil::Select(&readBlockSockets,&readAbleSockets,nullptr,nullptr,nullptr,nullptr))
        {
            continue;
        }

        std::vector<TCPSocketPtr> newSockets;

        for(const TCPSocketPtr& socket:readAbleSockets)
        {
            // socket = 알바생이 "여기 불 켜졌어요" 하고 들고 온 카메라 주소
            // sockServerTcp = 우리가 알고 있는 "정문" 카메라 주소
            if(socket==sockServerTcp)
            {
                // "어? 알바생이 들고 온 카메라 주소가 우리 '정문' 주소랑 똑같네?"
                // 정문 문손잡이가 덜컹거렸다는 뜻이니까 ➔ "아! 새로운 손님이 접속했구나!"
                SocketAddress newClientAddr;
                TCPSocketPtr newClientSock=sockServerTcp->Accept(newClientAddr);
                if(newClientSock)
                {
                    std::cout<<"New Client Connected : "<<newClientAddr.ToString()<<std::endl;
                    newSockets.push_back(newClientSock);
                    
                    ClientSessionPtr cs=std::make_shared<ClientSession>(newClientSock,nextClientSessionID);
                    cs->SetReplicationManager(new ReplicationManagerServer);                    
                    clientSessions.push_back(cs);
                    clientSessions[nextClientSessionID]=cs;
                    nextClientSessionID++;
                }
            }
            else
            {
                ClientSessionPtr currentClientSession=nullptr;
                int foundIdx=-1;
                for(int i=0;i<clientSessions.size();i++)
                {
                    if(clientSessions[i]->GetSocket()->GetSocket()==socket->GetSocket())
                    {
                        currentClientSession=clientSessions[i];
                        foundIdx=clientSessions[i]->GetSessionID();
                        break;
                    }
                }
                if(currentClientSession!=nullptr)
                {                    
                    bool isAlive=currentClientSession->ProcessIncomingData();
                    NetworkManagerServer::sInstance->SendOutgoingReplicationPackets();

                    if(!isAlive)
                    {
                        std::cout<<"ㅈ됬노 ㅅㅂ"<<std::endl;
                    }                    
                }                
            }                    
        }
        for(const auto& ns:newSockets)
        {
            readBlockSockets.push_back(ns);
        }
    }    

    return 0;
}