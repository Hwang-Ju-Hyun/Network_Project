#include <iostream>
#include "TCPSocket.hpp"
#include "SocketUtil.hpp"
#include <cassert>
#include "SocketAddressFactory.hpp"
#include "ClientProxy.hpp"
#include "NetworkManager.hpp"
#include "NetworkManagerServer.hpp"
#include "Server.hpp"
#include "ObjectRegistry.hpp"
#include "ReplicationManagerServer.hpp"
#include "TCPSession.hpp"
#include "Map.hpp"

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

    readBlockSockets.push_back(sockServerTcp);

    uint32_t nextClientSessionID=1;

    NetworkManagerServer::sInstance = new NetworkManagerServer();
    ObjectRegistry::sInstance->StaticInit();
    NetworkManagerServer::sInstance->Init();
    Map::StaticInit();

    while(g_LOOP)
    {

        // 🌟 1. 0초 대기(논블로킹)용 timeval 구조체 세팅
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        if(!SocketUtil::Select(&readBlockSockets,&readAbleSockets,nullptr,nullptr,nullptr,nullptr,&tv))
            continue;
        
        std::vector<TCPSocketPtr> newSockets;

        for(TCPSocketPtr& socket:readAbleSockets)
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

                    //hello packet을 똑바로 주고 받으면 session id 0이 아니라 갱신이 됨                    
                    NetworkManagerServer::sInstance->OnClientAccepted(newClientSock);
                }
            }
            else
            {
                ClientProxyPtr currentClientPtr=nullptr;                
                int foundIdx=-1;
                auto proxies = NetworkManagerServer::sInstance->GetPendingProxies();
                for(int i=0;i<proxies.size();i++)
                {                    
                    if(proxies[i]->GetSession()->GetSocket()==socket)
                    {
                        currentClientPtr=proxies[i];
                        foundIdx=proxies[i]->GetSessionID();
                        break;
                    }                    
                }
                if(currentClientPtr!=nullptr)
                {                
                    bool isAlive=currentClientPtr->GetSession()->ProcessIncomingData();                                                                       

                    if(!isAlive)
                    {
                        std::cout<<"ㅈ됬노 ㅅㅂ"<<std::endl;
                    }                    
                }                
            }                                
        }
        NetworkManagerServer::sInstance->SendOutgoingReplicationPackets();
        for(const auto& ns:newSockets)
        {
            readBlockSockets.push_back(ns);
        }
    }    

    return 0;
}