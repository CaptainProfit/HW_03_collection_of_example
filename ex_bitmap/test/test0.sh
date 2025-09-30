#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
filename=$(basename "$0")

if insmod ex_bitmap.ko; then
    rmmod ex_bitmap
    echo -e "$filename:${GREEN}OK${NC}"
else 
    echo -e "$filename:${RED}FAIL${NC}"
    exit -1
fi