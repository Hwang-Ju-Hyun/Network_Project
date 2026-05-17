add_executable(Server
    ServerMain.cpp
)

target_link_libraries(Server PRIVATE Shared)