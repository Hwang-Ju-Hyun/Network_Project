set(CMAKE_BUILD_TYPE Debug)

add_library(Shared STATIC
    SocketAddress.cpp
    SocketUtil.cpp
    TCPSocket.cpp
    UDPSocket.cpp
    MemoryStream.cpp
    MemoryBitStream.cpp
    LinkingContext.cpp
    SocketAddressFactory.cpp
    Object.cpp
    Robo.cpp    
    World.cpp
    Map.cpp    
    NetworkManager.cpp
    ObjectRegistry.cpp        
)

target_include_directories(Shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

