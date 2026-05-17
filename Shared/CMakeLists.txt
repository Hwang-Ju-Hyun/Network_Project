add_library(Shared STATIC
    SocketAddress.cpp
    SocketUtil.cpp
    TCPSocket.cpp
    UDPSocket.cpp
    MemoryStream.cpp
    MemoryBitStream.cpp
    LinkingContext.cpp
)

target_include_directories(Shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)