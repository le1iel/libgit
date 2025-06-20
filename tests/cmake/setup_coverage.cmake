set(TEST_COVERAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/coverage")
file(MAKE_DIRECTORY ${CMAKE_TEST_COVERAGE_DIR})

add_custom_target(coverage
    COMMAND llvm-profdata merge -sparse
        ${TEST_COVERAGE_DIR}/coverage_*.profraw 
        -o ${TEST_COVERAGE_DIR}/summary.profdata

    COMMAND llvm-cov show
        $<TARGET_FILE:gitxx-lib>
        -instr-profile=${TEST_COVERAGE_DIR}/summary.profdata
        -format=html
        -output-dir=coverage_html

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_dependencies(coverage test)
