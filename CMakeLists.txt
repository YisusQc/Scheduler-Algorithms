cmake_minimum_required(VERSION 3.30)
project(Scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib QUIET)

if(NOT raylib_FOUND)
  message(WARNING "No se encontró raylib, descargando...")
  include(FetchContent)

  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
  )

  FetchContent_MakeAvailable(raylib)
endif()

add_executable(main
    main.cpp
    algorithms.cpp
    display.cpp
    app.cpp
    appGraphic.cpp
    appConsole.cpp
)

target_link_libraries(main raylib)

