#pragma once
#include <unordered_map>
#include "header.hpp"
#include <vector>

class InputMemoryStream;

class ReplicationManagerClient
{
public:
    void Read(InputMemoryStream& _inStream);
public:
    void ReadAndDoCreateAction  ( InputMemoryStream& _inInputStream, uint32_t _inNetworkId );
    void ReadAndDoUpdateAction  ( InputMemoryStream& _inInputStream, uint32_t _inNetworkId );
    void ReadAndDoDestroyAction ( InputMemoryStream& _inInputStream, uint32_t _inNetworkId );
};