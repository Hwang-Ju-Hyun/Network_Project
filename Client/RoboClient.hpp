#pragma once
#include "Robo.hpp"
#include "Object.hpp"

class RoboClient : public Robo
{
public:
    RoboClient(){}
    ~RoboClient()override;
public:
    static ObjectPtr StaticCreate(){return std::make_shared<RoboClient>();}
};