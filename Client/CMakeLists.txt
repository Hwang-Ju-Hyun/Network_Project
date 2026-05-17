add_executable(Client
    ClientMain.cpp
)

target_link_libraries(Client PRIVATE Shared)