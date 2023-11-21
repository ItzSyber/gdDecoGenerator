include_directories(${CMAKE_CURRENT_SOURCE_DIR}/geometrize)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/geometrize/*.cpp
)

add_library(geometrize ${SOURCES})

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()