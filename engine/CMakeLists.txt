add_library(orbis_core STATIC
    src/version.cpp
)

target_include_directories(orbis_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(orbis_core PRIVATE
    -Wall -Wextra -Wpedantic
)

add_executable(orbis-engine src/main.cpp)
target_link_libraries(orbis-engine PRIVATE orbis_core)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()