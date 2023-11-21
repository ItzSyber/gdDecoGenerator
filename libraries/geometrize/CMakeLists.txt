# Добавляем путь к заголовочным файлам
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/geometrize)

# Список исходных файлов
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/geometrize/*.cpp
)

# Добавляем исходные файлы к вашему исполняемому файлу
add_library(geometrize ${SOURCES})

# Указываем, что требуется использовать pthread
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()