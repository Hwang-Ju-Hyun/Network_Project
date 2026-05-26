#include "Map.hpp"

std::unique_ptr<Map> Map::sInstance = nullptr;

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
            m_map[i][j]='.';
        }
    }   
}

std::vector<std::vector<char>>Map::GetMap()
{
    return m_map;
}


bool Map::IsWalkable(int _x,int _y)
{
    if(m_map.size()<=0)
        int a=0;
    if (_y < 0 || _y >= m_map.size() || _x < 0 || _x >= m_map[_y].size())
    {
        return false;
    }
    
    if (m_map[_y][_x] == 'X') 
    {
        return false;
    }
 
    return true;
}