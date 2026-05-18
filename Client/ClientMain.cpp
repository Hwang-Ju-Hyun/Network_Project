#include <iostream>
#include "SocketAddressFactory.hpp"
#include "SocketUtil.hpp"
#include "TCPSocket.hpp"
#include <cassert>

int main()
{ 
    const std::string destination="127.0.0.1:9000";
    SocketAddressPtr dest_sock_addr= SocketAddressFactory::CreateIPv4FromString(destination);
    TCPSocketPtr tcp_sock = SocketUtil::CreateTCPSocket(AF_INET);
    
    assert(tcp_sock->Connect(*dest_sock_addr)!=ERROR);
    
    std::string data;
    data="Hello I'm Client";   

    tcp_sock->Send(data.c_str(),static_cast<size_t>(data.length()+1));

    
    
    return 0;
}