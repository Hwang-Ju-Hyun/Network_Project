#pragma once
#include <iostream>
#include <cstring>
#include <stdint.h>
#include <memory>

class Object;

#define CLASS_IDENTIFICATION(inCode,inClass)\
enum{kClassID=inCode};\
virtual uint32_t GetClassID()const{return kClassID;}\
static Object* CreateInstance(){return static_cast<Object*>(new inClass());}\

class Object
{
public:
    Object();
    virtual ~Object(){}
    CLASS_IDENTIFICATION('OBJ',Object);
private:
    int m_posX,m_posY;
    char m_Name[10];
    int m_NetworkID;
public:
    void SetPosX(int _posX){m_posX=_posX;}
    void SetPosY(int _posY){m_posY=_posY;}
    void SetPos(int _x,int _y){m_posX=_x,m_posY=_y;}
    int GetPosX()const{return m_posX;}
    int GetPosY()const{return m_posY;}
    
    void SetName(char* _name){std::strcpy(m_Name,_name);}
    char* GetName(){return m_Name;}

    void SetNetworkID(int _netID){m_NetworkID=_netID;}
    int GetNetworkID()const{return m_NetworkID;}
};

typedef std::shared_ptr<Object> ObjectPtr;