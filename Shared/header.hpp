#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <arpa/inet.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

using SOCKET = unsigned int;

#define ERROR -1

enum PacketType : uint8_t
{
    PT_Replication=0,
    PT_MAZE_DATA=1,
    PT_Hello=2,
    PT_Disconnected=3,
    PT_MAX
};

enum ReplicationAction : uint8_t
{
    RT_CREATE,
    RT_UPDATE,
    RT_DESTORY,
    MAX
};