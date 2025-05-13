#!/bin/bash

echo "Running unit tests..."

# Navigate to the project root directory (if needed)
PROJECT_ROOT="$(dirname "$(dirname "$(dirname "$0")")")"
echo "Project root: $PROJECT_ROOT"
cd "$PROJECT_ROOT" || exit 1

# Debug: List files in tests/unit directory
echo "Listing files in tests/unit directory:"
ls -l tests/unit/

# Compile and run each test
for test_file in tests/unit/*.c; do
    echo "Compiling test file: $test_file"
    test_name=$(basename "$test_file" .c)
    echo "Test name: $test_name"
    gcc -o "tests/unit/$test_name" "$test_file" src/crypto.c src/aes.c src/utils.c -I./src -I./include
    if [ $? -ne 0 ]; then
        echo "Compilation failed for $test_file"
        exit 1
    fi
    ./tests/unit/"$test_name"
    if [ $? -ne 0 ]; then
        echo "Test failed: $test_name"
        exit 1
    fi
done

echo "All unit tests passed!"