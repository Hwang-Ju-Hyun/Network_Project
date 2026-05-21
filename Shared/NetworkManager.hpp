#pragma once
#include "header.hpp"

class InputMemoryStream;
class ClientSession;

//SINGLETON
class NetworkManager
{
public:
    static NetworkManager* GetInstance()
    {
        static NetworkManager nm;
        return &nm;
    }
    static NetworkManager* sInstance;
public:
    virtual~NetworkManager(){}
    virtual void Init(){}
    virtual void ProcessPacket(ClientSession* _cs,InputMemoryStream& _stream){};
};