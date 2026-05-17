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


enum PacketType
{
    PT_Hello,
    PT_Replication,
    PT_Disconnected,
    PT_MAX
};