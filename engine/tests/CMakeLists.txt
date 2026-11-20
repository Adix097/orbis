include(Catch)

add_executable(orbis_tests
    test_endian.cpp
    test_result.cpp
    test_byte_writer.cpp
    test_byte_reader.cpp
)

target_link_libraries(orbis_tests PRIVATE
    orbis_core
    Catch2::Catch2WithMain
)

target_compile_options(orbis_tests PRIVATE
    -Wall -Wextra -Wpedantic
)

catch_discover_tests(orbis_tests)