#include "NetworkManagerClient.hpp"
#include "MemoryStream.hpp"
#include <iostream>
#include "ClientSession.hpp"

void NetworkManagerClient::ProcessPacket(ClientSession* _session,InputMemoryStream& _inStream)
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
        break;
    }
}

void NetworkManagerClient::HandleHello_Packet(ClientSession* _session,InputMemoryStream& _inStream)
{        
    uint32_t SessionID;
    _inStream.Read(SessionID);
    _session->SetSessionID(SessionID);
    std::cout<<"서버의 hello packet을 받았습니다 저의 아이디는 : "<<_session->GetSessionID()<<std::endl;
}