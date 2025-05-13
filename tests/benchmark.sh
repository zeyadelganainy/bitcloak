#!/bin/bash

# Directories for test files
PNG_DIR="./pngs"
FILE_DIR="./files"
OUTPUT_DIR="./outputs"

# Create output directory if it doesn't exist
mkdir -p $OUTPUT_DIR

# Encryption modes
ENCRYPTION_MODES=("none" "xor" "aes")

# Benchmark embedding
echo "Benchmarking embedding..."
for mode in "${ENCRYPTION_MODES[@]}"; do
    echo "Testing with encryption mode: $mode"
    for png in $PNG_DIR/*.png; do
        for file in $FILE_DIR/*; do
            # Determine output file name based on encryption mode
            output="$OUTPUT_DIR/$(basename $png .png)_$(basename $file .txt)_$mode.png"
            echo "Embedding $(basename $file) into $(basename $png) with $mode encryption..."

            # Run embedding based on encryption mode
            if [ "$mode" == "none" ]; then
                time ../bitcloak.exe embed -i $png -f $file -o $output
            elif [ "$mode" == "xor" ]; then
                time ../bitcloak.exe embed -i $png -f $file -o $output --key "benchmarkkey" --use-xor
            elif [ "$mode" == "aes" ]; then
                time ../bitcloak.exe embed -i $png -f $file -o $output --key "benchmarkkey"
            fi
        done
    done
done

# Benchmark extraction
echo "Benchmarking extraction..."
for embedded_png in $OUTPUT_DIR/*.png; do
    output_file="$OUTPUT_DIR/extracted_$(basename $embedded_png .png).txt"
    echo "Extracting from $(basename $embedded_png)..."
    time ../bitcloak.exe extract -i $embedded_png -o $output_file --key "benchmarkkey"
done