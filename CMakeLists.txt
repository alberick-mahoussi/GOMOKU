cmake_minimum_required(VERSION 3.17)

include_directories(include)

project(pbrain-GOMOKOU C CXX)

add_executable(pbrain-GOMOKOU
    src/Protocol.cpp
        src/Brain.cpp
    src/main.cpp
)

if(WIN32)
    set(EXECUTABLE_NAME "pbrain-Gomoku.exe")
else()
    set(EXECUTABLE_NAME "pbrain-Gomoku")
endif()

install(TARGETS pbrain-GOMOKOU DESTINATION ../ RENAME ${EXECUTABLE_NAME})
