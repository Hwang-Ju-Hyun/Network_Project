#pragma once

class Server
{
public:
    Server();
    ~Server(){}
public:
    static void StaticInit();
    void SpawnRoboForHost();
};