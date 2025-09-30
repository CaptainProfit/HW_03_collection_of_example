#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
filename=$(basename "$0")

function test() {
    P="/sys/module/ex_binsearch/parameters/"
    count=0
    for i in {1..1000}; do
        echo $i >$P/find
        result=`cat $P/find`
        if [[ $result != *"not found" ]]; then
            ((count++))
        fi
    done
    echo found $count different elements
}

insmod ex_binsearch.ko
test
rmmod ex_binsearch.ko

if [[ ! $count -eq 0 && ! $count -eq 1000 ]]; then 
    echo -e "$filename:${GREEN}OK${NC}"
else 
    echo -e "$filename:${RED}FAIL${NC}"
    exit -1
fi
