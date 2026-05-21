set(CMAKE_BUILD_TYPE Debug)

add_executable(Server
    ServerMain.cpp
    NetworkManagerServer.cpp    
    Server.cpp
    RoboServer.cpp    
    ReplicationManagerServer.cpp
    ClientSession.cpp
)

target_link_libraries(Server PRIVATE Shared)

set(CMAKE_BUILD_TYPE Debug)