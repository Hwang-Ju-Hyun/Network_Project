#include "LinkingContext.hpp"
#include <assert.h>

LinkingContext::LinkingContext()
    :m_NextNetworkID(1)
{
}

LinkingContext::~LinkingContext(){}

uint32_t LinkingContext::GetNetworkID(Object* _obj)
{
    auto iter=m_ObjectToNetworkId.find(_obj);

    if(iter!=m_ObjectToNetworkId.end())
    {
        return iter->second;
    }

    return iter->second;
}

Object* LinkingContext::GetObject(uint32_t _networkId)
{
    auto iter=m_NetworkIdToObject.find(_networkId);

    if(iter!=m_NetworkIdToObject.end())
    {
        return iter->second;
    }   
    return nullptr; 
}

void LinkingContext::AddObject(Object* _obj,uint32_t _networkID)
{
    m_NetworkIdToObject[_networkID]=_obj;
    m_ObjectToNetworkId[_obj]=_networkID;
}

void LinkingContext::RemoveObject(Object* _inObject)
{
    uint32_t networkID=m_ObjectToNetworkId[_inObject];
    m_ObjectToNetworkId.erase(_inObject);
    m_NetworkIdToObject.erase(networkID);    
}

