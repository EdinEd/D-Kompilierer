#!/bin/bash

COMPILER="build/compiler"
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

PASSED=0
FAILED=0


run_test_suite() {
    local FOLDER=$1
    local EXPECTED_CODE=$2
    
    echo "=== Testing $FOLDER Programme (Expected Exit-Code: $EXPECTED_CODE) ==="
    
    for input_file in $FOLDER/*.d; do
        [ -e "$input_file" ] || continue 
        
        $COMPILER $input_file > /dev/null 2>&1
        
        # Speichere den Rückgabewert
        ACTUAL_CODE=$?
        
        if [ $ACTUAL_CODE -eq $EXPECTED_CODE ]; then
            echo -e "${GREEN}[PASS]${NC} $input_file"
            PASSED=$((PASSED+1))
        else
            echo -e "${RED}[FAIL]${NC} $input_file (Expected: $EXPECTED_CODE, Actual: $ACTUAL_CODE)"
            FAILED=$((FAILED+1))
        fi
    done
    echo ""
}


echo "Compiling Parser..."
mkdir -p build
cd build
cmake ../..
make
cd ..


echo ""
echo "=== Starting Tests ==="
run_test_suite "correct" 0
run_test_suite "incorrect" 1

echo "=== Summary ==="
echo -e "${GREEN}Passed: $PASSED${NC}"
if [ $FAILED -gt 0 ]; then
    echo -e "${RED}Failed: $FAILED${NC}"
    exit 1
else
    exit 0
fi

echo "Cleaning up..."
rm -rf build