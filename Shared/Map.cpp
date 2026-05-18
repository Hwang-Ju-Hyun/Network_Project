#include "Map.hpp"

void Map::StaticInit()
{   
    sInstance.reset(new Map()); 
}

Map::Map()
{
    m_map.resize(MAX_ROW,std::vector<char>(MAX_COL));

    for(int i=0;i<MAX_ROW;i++)
    {
        for(int j=0;j<MAX_COL;j++)
        {
            m_map[i][j]='X';
        }
    }   
}

std::vector<std::vector<char>>Map :: GetMap()
{
    return m_map;
}