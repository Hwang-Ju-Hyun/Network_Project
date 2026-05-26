#include "InputManager.hpp"
#include "MemoryStream.hpp"

namespace 
{
    int _kbhit(void) 
    {
        struct termios oldt, newt;
        int ch;
        int oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if(ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }
        return 0;
    }

    char _getch(void) 
    {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0) perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
        return buf;
    }
}

void InputManager::ProcessInput(TCPSessionPtr _session)
{
    // 입력이 없으면 즉시 종료 (논블로킹)
    if (!_kbhit()) 
    {
        return; 
    }

    char key = _getch(); 
    int moveX = 0;
    int moveY = 0;

    switch (key)
    {
    case 'w': case 'W': moveY = -1; break;
    case 's': case 'S': moveY = 1;  break;
    case 'a': case 'A': moveX = -1; break;
    case 'd': case 'D': moveX = 1;  break;
    default: return; // 다른 키면 무시
    }

    // 패킷 조립 및 발사
    OutputMemoryStream inputStream;
    uint8_t packetType = PacketType::PT_INPUT; 
    
    inputStream.Write(packetType);
    inputStream.Write(moveX);
    inputStream.Write(moveY);

    _session->SendPacket(inputStream);
}