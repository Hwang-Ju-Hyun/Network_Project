#include "World.hpp"

void World::StaticInit()
{
    sInstance.reset(new World());
}
