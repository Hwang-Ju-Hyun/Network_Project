#pragma once
#include "Object.hpp"
#include <vector>

#define MAX_ROW 50
#define MAX_COL 50

class Map : public Object
{
private:
    Map();    
    ~Map();
public: 
    CLASS_IDENTIFICATION('MAP',Map);
public:
    static std::unique_ptr<Map> sInstance;
    static void StaticInit();
private:
    static std::vector<std::vector<char>> m_map;
public:
    static std::vector<std::vector<char>> GetMap();
};