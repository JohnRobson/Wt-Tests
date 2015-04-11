cmake_minimum_required(VERSION 2.6)

project(Emails)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_COMPILER "g++")
#set(CMAKE_CXX_COMPILER "g++-4.7")

# $ cat /proc/cpuinfo
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -pedantic -Wall -Wextra -march=ivybridge -pthread -O0 -g3 -ggdb3 -DDEBUG -D_DEBUG=9") # -std=gnu++11 | -pthread

#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -pedantic -Wall -Wextra -march=ivybridge -pthread -O2 -g0 -DNDEBUG -UNDEBUG -pthread")

find_package(Boost COMPONENTS REQUIRED) # COMPONENTS date_time regex program_options signals system filesystem thread REQUIRED

include_directories(/usr/include /usr/local/include ${CMAKE_SOURCE_DIR}/src ${Wt_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
link_directories(/usr/lib /usr/local/lib ${Boost_LIBRARY_DIRS})

add_executable(test.wt test.cpp)

target_link_libraries(test.wt wt wthttp ${Boost_LIBRARIES}) # wthttp / wtfcgi # wtdbo wtdbosqlite3 wtdbopostgres #
