#pragma once
#include "TCPSession.hpp"

class InputManager
{
private:
    InputManager() {}
    ~InputManager() {}

public:
    static InputManager& GetInstance()
    {
        static InputManager instance;
        return instance;
    }

public:
    // 매 프레임 호출되어 입력을 감지하고 패킷을 쏩니다.
    void ProcessInput(TCPSessionPtr _session);
};