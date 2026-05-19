#include <iostream>
#include "SocketAddressFactory.hpp"
#include "SocketUtil.hpp"
#include "TCPSocket.hpp"
#include <cassert>
#include "MemoryStream.hpp"


int main()
{ 
    const std::string destination="127.0.0.1:9000";
    SocketAddressPtr dest_sock_addr= SocketAddressFactory::CreateIPv4FromString(destination);
    TCPSocketPtr tcp_sock = SocketUtil::CreateTCPSocket(AF_INET);
    
    assert(tcp_sock->Connect(*dest_sock_addr)!=ERROR);        

    std::string data;
    data="Hello I'm Client";       
    //tcp_sock->Send(data.c_str(),static_cast<size_t>(data.length()+1));    


    OutputMemoryStream payloadStream;
    uint32_t packetType=PacketType::PT_Hello;
    payloadStream.Write(&packetType,sizeof(packetType));    
    uint16_t totalPacketSize = static_cast<uint16_t>(sizeof(uint16_t) + payloadStream.GetLength());
    
    OutputMemoryStream finalPacketStream;
    finalPacketStream.Write(&totalPacketSize, sizeof(uint16_t)); // 전체 길이 기록
    finalPacketStream.Write(payloadStream.GetBuffer(), payloadStream.GetLength()); // 페이로드 붙이기

    // 4. 최종 스트림 전송
    tcp_sock->Send(finalPacketStream.GetBuffer(), finalPacketStream.GetLength());
    
    return 0;
}