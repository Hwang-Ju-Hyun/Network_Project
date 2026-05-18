#pragma once
#include <memory>
#include <vector>

#define MAX_ROW 50
#define MAX_COL 50

class Map
{
private:
    Map();    
    ~Map();
public:
    static std::unique_ptr<Map> sInstance;
    static void StaticInit();
private:
    static std::vector<std::vector<char>> m_map;
public:
    static std::vector<std::vector<char>> GetMap();
};