#pragma once
#include "Object.hpp"

class Robo:public Object
{   
public:
    CLASS_IDENTIFICATION('ROBO',Robo);
public:
    Robo();
    virtual ~Robo()override{}      
};