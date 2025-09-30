#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

tests=(
"./test/test0.sh"
"./test/test1.sh"
"./test/test2.sh"
"./test/test3.sh"
)

fails=0
for test in ${tests[@]}; do
    #echo "$test"
    if ! $test; then
        ((fails++))
    fi
done

if [ $fails -eq 0 ]; then
    echo -e "${GREEN}All tests passed${NC}"
else 
    echo -e "${RED}Some tests failed${NC}"
    ((fails++))
fi