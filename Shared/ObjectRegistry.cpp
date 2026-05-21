#include "ObjectRegistry.hpp"
#include <cassert>

std::unique_ptr<ObjectRegistry> ObjectRegistry::sInstance=nullptr;

void ObjectRegistry::StaticInit()
{
    sInstance.reset(new ObjectRegistry());
}

void ObjectRegistry::RegisterCreationFunction(uint32_t _inClassName,ObjectCreationFunc _inCreationFunction)
{
    assert(m_NameToObjectCreationFuncMap.find(_inClassName)==m_NameToObjectCreationFuncMap.end());

    m_NameToObjectCreationFuncMap[_inClassName]=_inCreationFunction;
}

ObjectPtr ObjectRegistry::CreateObject(uint32_t _inClassName)
{
    ObjectCreationFunc createFunc = m_NameToObjectCreationFuncMap[_inClassName];
    ObjectPtr obj=createFunc();          
    
    return obj;
}