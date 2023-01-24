cmake_minimum_required(VERSION 2.8.12)
project(samp-gps-plugin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
  endif()
  if(CMAKE_COMPILER_IS_GNUCC
     AND NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 4.0)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
  elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-ignored-attributes")
  endif()
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-noexcept-type -pthread -lrt")
  if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
  endif()
  if(CMAKE_COMPILER_IS_GNUCXX
     AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-attributes")
  endif()
endif()

include(CheckIncludeFile)

check_include_file(alloca.h HAVE_ALLOCA_H)
if(HAVE_ALLOCA_H)
  add_definitions(-DHAVE_ALLOCA_H)
endif()
check_include_file(inttypes.h HAVE_INTTYPES_H)
if(HAVE_INTTYPES_H)
  add_definitions(-DHAVE_INTTYPES_H)
endif()
check_include_file(malloc.h HAVE_MALLOC_H)
if(HAVE_MALLOC_H)
  add_definitions(-DHAVE_MALLOC_H)
endif()
check_include_file(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
  add_definitions(-DHAVE_STDINT_H)
endif()

include_directories(
  ./lib/sdk/amx
)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WINXP
                  -D_CRT_SECURE_NO_WARNINGS
                  -DWIN32_LEAN_AND_MEAN)
endif()

file(GLOB_RECURSE GPS_SOURCES src/*.cpp lib/sdk/amxplugin.cpp lib/sdk/amxplugin2.cpp)

add_library(samp-gps-plugin MODULE ${GPS_SOURCES})

set_target_properties(samp-gps-plugin PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "GPS")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
  set_property(TARGET samp-gps-plugin APPEND_STRING PROPERTY COMPILE_FLAGS " -Wall")
endif()
