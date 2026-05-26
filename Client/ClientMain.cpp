#include <iostream>
#include "SocketAddressFactory.hpp"
#include "SocketUtil.hpp"
#include "TCPSocket.hpp"
#include <cassert>
#include "MemoryStream.hpp"
#include "NetworkManagerClient.hpp"
#include "TCPSession.hpp"
#include "ObjectRegistry.hpp"
#include "RenderManager.hpp"
#include "InputManager.hpp"
#include "Map.hpp"
#include <chrono>
#include <thread>


int main()
{ 
    ObjectRegistry::sInstance->StaticInit();
    
    NetworkManagerClient::sInstance=new NetworkManagerClient();
    NetworkManagerClient::sInstance->Init();            

    const std::string destination="127.0.0.1:9999";
    SocketAddressPtr dest_sock_addr= SocketAddressFactory::CreateIPv4FromString(destination);
    TCPSocketPtr tcp_sock = SocketUtil::CreateTCPSocket(AF_INET);

    assert(tcp_sock->Connect(*dest_sock_addr)!=ERROR);            
    tcp_sock->SetNonBlockingMode(true);

    TCPSessionPtr serverSession =std::make_shared<TCPSession>();
    serverSession->OnPacketReceived=[](InputMemoryStream& inStream){NetworkManagerClient::sInstance->ProcessPacket(inStream);};
    serverSession->SetSocket(tcp_sock);

    OutputMemoryStream payloadStream;
    uint8_t packetType=PacketType::PT_Hello;

    std::string data;
    data="Hello I'm Client";
        
    payloadStream.Write(packetType);
    payloadStream.Write(data.c_str(),data.length());
    serverSession->SendPacket(payloadStream);

    Map::StaticInit();
        
    while(true)
    {        
        serverSession->ProcessIncomingData();

        InputManager::GetInstance().ProcessInput(serverSession);

        RenderManager::GetInstance().Render();                      
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }    

    return 0;
}