set(CMAKE_BUILD_TYPE Debug)

add_executable(Client
    ClientMain.cpp
    RenderManager.cpp
    Client.cpp
    NetworkManagerClient.cpp
    ReplicationManagerClient.cpp    
)

target_link_libraries(Client PRIVATE Shared)
