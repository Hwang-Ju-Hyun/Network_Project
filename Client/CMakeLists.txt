set(CMAKE_BUILD_TYPE Debug)

add_executable(Client
    ClientMain.cpp
)

target_link_libraries(Client PRIVATE Shared)

set(CMAKE_BUILD_TYPE Debug)