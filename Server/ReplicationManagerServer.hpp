#pragma once
#include <unordered_map>
#include "header.hpp"
#include <vector>

class OutputMemoryStream;

class ReplicationManagerServer
{
private:
    // 어떤 NetworkID를 가진 로봇이 어떤 행동(Create/Update/Destroy)을 해야 하는지 저장하는 장부
    std::unordered_map<uint32_t, ReplicationAction> m_Commands;
    std::vector<uint32_t> m_ObjectToRemove;
public:
    void ReplicateCreate (uint32_t _networkID){m_Commands[_networkID]=ReplicationAction::RT_CREATE;}
    void SetStateDirty(uint32_t _networkID)
    {
        if(m_Commands.find(_networkID)==m_Commands.end())
        {
            m_Commands[_networkID]=ReplicationAction::RT_UPDATE;
        }
    }    
    void ReplicateDestroy(uint32_t _networkID){m_Commands[_networkID]=ReplicationAction::RT_DESTORY;}

    void Write(OutputMemoryStream& _outStream);
};