set(TEST_COVERAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/coverage")
file(MAKE_DIRECTORY ${CMAKE_TEST_COVERAGE_DIR})

target_compile_options(gitxx-lib PRIVATE  -g -O0 -fprofile-instr-generate -fcoverage-mapping)
target_link_libraries(gitxx-lib PRIVATE -fprofile-instr-generate)

add_custom_target(coverage
    COMMAND /opt/homebrew/opt/llvm/bin/llvm-profdata merge -sparse
        ${TEST_COVERAGE_DIR}/coverage_*.profraw
        -o ${TEST_COVERAGE_DIR}/summary.profdata

    COMMAND /opt/homebrew/opt/llvm/bin/llvm-cov show
        $<TARGET_FILE:gitxx-lib>
        -instr-profile=${TEST_COVERAGE_DIR}/summary.profdata
        -format=html
        -output-dir=coverage_html

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(coverage-json
    COMMAND /opt/homebrew/opt/llvm/bin/llvm-profdata merge -sparse
        ${TEST_COVERAGE_DIR}/coverage_*.profraw
        -o ${TEST_COVERAGE_DIR}/summary.profdata

    COMMAND /opt/homebrew/opt/llvm/bin/llvm-cov export
        -summary-only
        $<TARGET_FILE:gitxx-lib>
        -instr-profile=${TEST_COVERAGE_DIR}/summary.profdata | jq . > ${CMAKE_BINARY_DIR}/coverage.json

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
