#include "UDPSocket.hpp"
#include "SocketAddress.hpp"
#include <iostream>

UDPSocket::UDPSocket(SOCKET _socket)
    :m_Socket(_socket)
{

}

int UDPSocket::Bind(const SocketAddress& _inAddr)
{
    int result = bind(m_Socket,&(_inAddr.m_sockAddr),_inAddr.GetSize());
    if(result==-1)
    {
        std::cout<<"Bind Error : "<<strerror(errno)<<std::endl;
        return -1;
    }
    return result;
}

int UDPSocket::SendTo(const void* _inData, size_t _length, const SocketAddress& _inAddr)
{
    size_t sendBytesCount= send(m_Socket,static_cast<const char*>(_inData),_length,0);
    if(sendBytesCount<=0)
    {
        std::cout<<"Send Error : "<<strerror(errno)<<std::endl;
        return -1;
    }
    return sendBytesCount;
}


int UDPSocket::ReceiveFrom( void* _outData, size_t _Maxlength, SocketAddress& _outAddr)
{
    socklen_t outLength=_outAddr.GetSize();
    int readBytesCount = recvfrom(m_Socket,static_cast<char*>(_outData),_Maxlength,0,&(_outAddr.m_sockAddr),&outLength);
    if(readBytesCount<=0)
    {
        std::cout<<"Read Error : "<<strerror(errno)<<std::endl;
        return -1;
    }
    return readBytesCount;
}