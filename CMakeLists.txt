cmake_minimum_required(VERSION 3.15)
project(PlainMQ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm256")
set(SOURCE_FILES
    "Source/Main.cpp"
    "Source/Commands/CommandCreate.cpp"
    "Source/Commands/CommandSubscribe.cpp"
    "Source/Commands/CommandPublish.cpp"
    "Source/Commands/CommandUnsubscribe.cpp"
    "Source/Commands/CommandProbe.cpp"
    "Source/Core/Channel.cpp"
    "Source/Core/MessageHandler.cpp" 
    "Source/Core/Client.cpp"
    "Source/Core/Server.cpp"
    "Source/Logger.cpp"
)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system thread chrono regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(PlainMQ ${SOURCE_FILES})
set_target_properties(PlainMQ PROPERTIES OUTPUT_NAME "plainmq")

target_include_directories(PlainMQ PRIVATE Source)
target_include_directories(PlainMQ PRIVATE ThirdParty)
target_include_directories(PlainMQ PRIVATE ThirdParty/websocketpp)

target_link_libraries(PlainMQ ${Boost_LIBRARIES})