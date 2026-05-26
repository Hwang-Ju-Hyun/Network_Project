#include "RenderManager.hpp"
#include "Map.hpp"
#include "NetworkManagerClient.hpp"
#include "LinkingContext.hpp"

void RenderManager::Render()
{    
    std::vector<std::vector<char>> drawBuffer = Map::sInstance->GetMap();
    
    std::unordered_map<uint32_t,ObjectPtr> allObjects = NetworkManagerClient::sInstance->GetLinkingContext()->GetAllObjects();
    std::system("clear");
    std::cout<<"======Game======"<<std::endl<<std::endl;
    for (auto& pair : allObjects)
    {
        ObjectPtr obj = pair.second;
        
        // 아까 Object 클래스에 만들어두신 좌표 가져오기 함수 사용
        int x = obj->GetPosX(); 
        int y = obj->GetPosY();

        //방어 코드: 객체가 맵 밖으로 나가면 프로그램이 튕기므로 안전 검사 필수!
        if (y >= 0 && y < drawBuffer.size() && x >= 0 && x < drawBuffer[y].size())
        {
            // 해당 좌표의 배경(예: 흙이나 바닥)을 지우고, 캐릭터('R')로 덮어씌웁니다.
            drawBuffer[y][x] = 'R'; 
        }
    }
    
    for(int i = 0; i < drawBuffer.size(); i++)
    {
        for(int j = 0; j < drawBuffer[i].size(); j++)
        {
            std::cout << drawBuffer[i][j] << " "; // 띄어쓰기를 넣으면 맵이 더 정사각형처럼 예쁘게 보입니다.
        }
        std::cout << '\n'; // std::endl 보다 '\n'이 렌더링 속도가 훨씬 빠릅니다!
    }
};