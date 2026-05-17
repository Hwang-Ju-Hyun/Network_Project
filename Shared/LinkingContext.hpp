#pragma once
#include <stdint.h>
#include <unordered_map>

class Object;

class LinkingContext
{
public:
    LinkingContext();
    ~LinkingContext();
public:
    uint32_t GetNetworkID(Object* _obj);
    Object* GetObject(uint32_t _networkId);
    void AddObject(Object* _obj,uint32_t _networkID);
    void RemoveObject(Object* _inObject);
private:
    std::unordered_map<uint32_t,Object*> m_NetworkIdToObject;
    std::unordered_map<Object*,uint32_t> m_ObjectToNetworkId;
private:
    uint32_t m_NextNetworkID;
};