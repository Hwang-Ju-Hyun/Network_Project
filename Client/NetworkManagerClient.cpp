#include "NetworkManagerClient.hpp"
#include "MemoryStream.hpp"
#include <iostream>
#include "LinkingContext.hpp"
#include "ObjectRegistry.hpp"


NetworkManagerClient* NetworkManagerClient::sInstance=nullptr;

void NetworkManagerClient::Init()
{
    m_LinkingContext=new LinkingContext;
}

void NetworkManagerClient::ProcessPacket(InputMemoryStream& _inStream)
{
    uint8_t packet_type;
    _inStream.Read(packet_type);
    switch (packet_type)
    {
    case PT_Hello:        
        HandleHello_Packet(_inStream);
        break;
    case PT_Replication:
        HandleReplication_Packet(_inStream);
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

void NetworkManagerClient::HandleHello_Packet(InputMemoryStream& _inStream)
{        
    uint32_t SessionID;
    _inStream.Read(SessionID);
    //_session->SetSessionID(SessionID);
    //std::cout<<"서버의 hello packet을 받았습니다 저의 아이디는 : "<<_session->GetSessionID()<<std::endl;
}

void NetworkManagerClient::HandleReplication_Packet(InputMemoryStream& _inStream)
{
    uint32_t commandCount;
    _inStream.Read(commandCount);

    for(int i=0;i<commandCount;i++)
    {
        uint32_t networkID;
        uint8_t  actionByte;        

        _inStream.Read(networkID);
        _inStream.Read(actionByte);

        ReplicationAction action=static_cast<ReplicationAction>(actionByte);      

        switch (action)
        {
        case RT_CREATE:
        {
            uint32_t classID;            
            _inStream.Read(classID);
                        
            Object* obj = m_LinkingContext->GetObject(classID);

            if(obj==nullptr)
            {
                ObjectPtr newObj = ObjectRegistry::sInstance->CreateObject(obj->GetClassID());

                newObj->Read(_inStream);

                m_LinkingContext->AddObject(newObj.get(),networkID);
                
                char* cc=reinterpret_cast<char*>(&classID);
                std::cout << "[클라] 서버 지시로 객체 생성 및 월드 배치 완료! 종류: " 
                                  << cc[3] << cc[2] << cc[1] << cc[0] 
                                  << " | NetworkID: " << networkID << std::endl;
            }
        }
            break;
        case RT_UPDATE:
        {
                // 1. 이미 태어난 정식 등록 객체이니 내 호적부에서 검색합니다.
                Object* obj = m_LinkingContext->GetObject(networkID);
                if (obj != nullptr)
                {
                    // 2. 바뀐 데이터(매 프레임 변하는 동적 좌표 등)만 읽어서 실시간 동기화!
                    obj->Read(_inStream);
                }
        }        
            break;

        case RT_DESTORY:
        {
            // 1. 사라질 객체를 찾아서
            Object* obj = m_LinkingContext->GetObject(networkID);
            if (obj != nullptr)
            {
                // 2. 가상 세계(World)와 내 호적부(LinkingContext)에서 깔끔하게 파내버립니다.            
                m_LinkingContext->RemoveObject(obj);                
                std::cout << "[클라] 객체 소멸 완료. NetworkID: " << networkID << std::endl;
            }
        }
        break;
        default:
            break;
        }
    }
}