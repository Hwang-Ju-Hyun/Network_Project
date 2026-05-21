#include "Server.hpp"
#include "ObjectRegistry.hpp"
#include "RoboServer.hpp"

Server::Server()
{
    ObjectRegistry::sInstance->RegisterCreationFunction('ROBO',RoboServer::StaticCreate);
}


void Server::StaticInit()
{

}