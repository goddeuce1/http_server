cmake_minimum_required(VERSION 3.10)
project(http-server)
set(CMAKE_CXX_STANDARD 10)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(http-server src/main.cpp src/server/server.h src/common/config.h src/common/config.cpp src/server/server.cpp src/connection/connection.h src/connection/connection.cpp src/request/request.h src/request/request.cpp)

if(Boost_FOUND)

    target_link_libraries(http-server ${Boost_LIBRARIES})

endif()


