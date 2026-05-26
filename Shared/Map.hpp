#pragma once
#include "Object.hpp"
#include <vector>

#define MAX_ROW 20
#define MAX_COL 20

class Map : public Object
{
private:
    Map();        
public: 
    CLASS_IDENTIFICATION('MAP',Map);
public:
    static std::unique_ptr<Map> sInstance;
    static void StaticInit();
private:
    std::vector<std::vector<char>> m_map;
public:
    std::vector<std::vector<char>> GetMap();
};