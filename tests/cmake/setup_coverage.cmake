set(TEST_COVERAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/coverage")
file(MAKE_DIRECTORY ${CMAKE_TEST_COVERAGE_DIR})

find_program(LLVM_PROFDATA
    NAMES llvm-profdata
    HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
    REQUIRED)
find_program(LLVM_COV
    NAMES llvm-cov
    HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
    REQUIRED)
find_program(JQ jq REQUIRED)

target_compile_options(gitxx-lib PRIVATE  -g -O0 -fprofile-instr-generate -fcoverage-mapping)
target_link_libraries(gitxx-lib PRIVATE -fprofile-instr-generate)

add_custom_target(coverage
    COMMAND ${LLVM_PROFDATA} merge -sparse
        ${TEST_COVERAGE_DIR}/coverage_*.profraw
        -o ${TEST_COVERAGE_DIR}/summary.profdata

    COMMAND ${LLVM_COV} show
        $<TARGET_FILE:gitxx-lib>
        -instr-profile=${TEST_COVERAGE_DIR}/summary.profdata
        -format=html
        -output-dir=coverage_html

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(coverage-json
    COMMAND ${LLVM_PROFDATA} merge -sparse
        ${TEST_COVERAGE_DIR}/coverage_*.profraw
        -o ${TEST_COVERAGE_DIR}/summary.profdata

    COMMAND ${LLVM_COV} export
        -summary-only
        $<TARGET_FILE:gitxx-lib>
        -instr-profile=${TEST_COVERAGE_DIR}/summary.profdata | ${JQ} . > ${CMAKE_BINARY_DIR}/coverage.json

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(coverage-check
    COMMAND ${LLVM_PROFDATA} merge -sparse
        ${TEST_COVERAGE_DIR}/coverage_*.profraw
        -o ${TEST_COVERAGE_DIR}/summary.profdata

    COMMAND ${LLVM_COV} export
        -summary-only
        $<TARGET_FILE:gitxx-lib>
        -instr-profile=${TEST_COVERAGE_DIR}/summary.profdata
        > ${CMAKE_BINARY_DIR}/coverage.json

    COMMAND ${JQ} .data[0].totals ${CMAKE_BINARY_DIR}/coverage.json

    COMMAND ${JQ} --exit-status
        ".data[0].totals.lines.percent == 100 and .data[0].totals.regions.percent == 100 and .data[0].totals.branches.percent >= 95"
        ${CMAKE_BINARY_DIR}/coverage.json

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
