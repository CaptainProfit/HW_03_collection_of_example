#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
filename=$(basename "$0")

if insmod ex_rbtree.ko; then
    rmmod ex_rbtree
    echo -e "$filename:${GREEN}OK${NC}"
else 
    echo -e "$filename:${RED}FAIL${NC}"
    exit -1
fi