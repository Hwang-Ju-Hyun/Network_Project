#include <iostream>
#include "client_header.hpp"
#include "Object.hpp"

class RenderManager
{
private:
    RenderManager(){}
public:
    static RenderManager& GetInstance()
    {
        static RenderManager rm;
        return rm;
    }
public:
   void Render();
};