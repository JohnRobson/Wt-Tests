cmake_minimum_required(VERSION 2.6)

project(Test)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_COMPILER "g++")
#set(CMAKE_CXX_COMPILER "g++-4.7")

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++98 -pedantic -Wall -Wextra -O0 -g3 -ggdb3 -DDEBUG -D_DEBUG=9") # -std=c++11 | -pthread

#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -pedantic -Wall -Wextra -march=core2 -O2 -g0 -DNDEBUG -UNDEBUG -s")

find_package(Boost COMPONENTS REQUIRED) # COMPONENTS date_time regex program_options signals system filesystem thread REQUIRED

include_directories(/usr/local/include ${CMAKE_SOURCE_DIR}/src ${Wt_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
link_directories(/usr/lib /usr/local/lib ${Boost_LIBRARY_DIRS})

#add_executable(test.wt test.cpp PanelList.cpp ChartConfig.cpp CsvUtil.cpp)
add_executable(test.wt test.cpp CsvUtil.cpp)

target_link_libraries(test.wt wt wthttp ${Boost_LIBRARIES}) # wthttp / wtfcgi # wtdbo wtdbosqlite3 wtdbopostgres #
