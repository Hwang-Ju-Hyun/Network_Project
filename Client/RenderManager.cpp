#include "RenderManager.hpp"
#include "Map.hpp"


void RenderManager::Render()
{    
    std::vector<std::vector<char>> map=Map::GetMap();
    for(int i=0;i<map.size();i++)
    {
        for(int j=0;j<map[i].size();j++)
        {
            std::cout<<map[i][j]<<" ";
        }
        std::cout<<std::endl;
    }    
};