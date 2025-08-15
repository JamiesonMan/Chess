cmake_minimum_required(VERSION 3.10.0)
project(Chess VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

add_subdirectory(libs/chess_engine)

add_executable(Chess
    src/Main.cpp
    src/Game.cpp
    src/DBoard.cpp
)

target_include_directories(Chess PRIVATE src)
target_link_libraries(Chess PRIVATE chess_engine)

add_executable(debug_kiwipete
    debug_kiwipete.cpp
)

target_link_libraries(debug_kiwipete PRIVATE chess_engine)

# Visual Studio specific settings
if(WIN32 AND MSVC)
    # Set startup project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)
    
    # Set working directory for debugging
    set_target_properties(Chess PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        VS_DEBUGGER_COMMAND_ARGUMENTS "dev"
    )
    
    # Create folders in Solution Explorer
    set_target_properties(Chess PROPERTIES FOLDER "Applications")
    set_target_properties(chess_engine PROPERTIES FOLDER "Libraries")
endif()

enable_testing()