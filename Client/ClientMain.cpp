#include <iostream>
#include "SocketAddressFactory.hpp"
#include "SocketUtil.hpp"
#include "TCPSocket.hpp"
#include <cassert>
#include "MemoryStream.hpp"
#include "NetworkManagerClient.hpp"
#include "../Server/ClientSession.hpp"

int main()
{ 
    NetworkManager::sInstance=new NetworkManagerClient();
    NetworkManager::sInstance->Init();
        
    const std::string destination="127.0.0.1:9999";
    SocketAddressPtr dest_sock_addr= SocketAddressFactory::CreateIPv4FromString(destination);
    TCPSocketPtr tcp_sock = SocketUtil::CreateTCPSocket(AF_INET);

    assert(tcp_sock->Connect(*dest_sock_addr)!=ERROR);            

    ClientSession* serverSession=new ClientSession(tcp_sock,40);

    std::string data;
    data="Hello I'm Client";               

    OutputMemoryStream payloadStream;
    uint8_t packetType=PacketType::PT_Hello;
        
    payloadStream.Write(packetType);
    payloadStream.Write(data.c_str(),data.length());
    serverSession->SendPacket(payloadStream);    
        
    while(true)
    {
        serverSession->ProcessIncomingData();
        sleep(5);
        OutputMemoryStream rtStream;
        uint8_t packetType=PacketType::PT_Replication;
        rtStream.Write(packetType);
        serverSession->SendPacket(rtStream);
    }
    

    return 0;
}