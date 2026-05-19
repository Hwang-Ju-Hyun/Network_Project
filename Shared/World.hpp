#pragma once
#include <memory>

class World
{
private:
    World(){}
public:
    ~World(){}
public:
    static std::unique_ptr<World> sInstance;
    static void StaticInit();
};