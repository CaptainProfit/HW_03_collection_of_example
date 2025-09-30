#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
filename=$(basename "$0")

checknum=0
checkok=0
checklist=(
"0b11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
"0b01111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
"0b00000000000000001111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
"0b00000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111111111111111111111111111111111111"
"0b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
)

function check() {
    conc="$*"
    if [ "$conc" == "${checklist[$checknum]}" ]; then
        echo $filename: part${checknum} passed
        ((checkok++))
    else
        echo $filename: part${checknum} failed
        echo "    result: \"$conc\""
        echo "    expect: \"${checklist[$checknum]}\""
    fi
    ((checknum++))
}

function test() {
    P="/sys/module/ex_bitmap/parameters/"
    for i in {0..127}; do
        echo $i >$P/set
    done
    result=`cat $P/print`
    check $result

    echo 0 >$P/clear
    result=`cat $P/print`
    check $result

    for i in {1..15}; do
        echo $i >$P/clear
    done
    result=`cat $P/print`
    check $result

    for i in {16..63}; do
        echo $i >$P/clear
    done
    result=`cat $P/print`
    check $result

    for i in {64..127}; do
        echo $i >$P/clear
    done
    result=`cat $P/print`
    check $result
}

insmod ex_bitmap.ko
test
rmmod ex_bitmap.ko

echo $checkok/$checknum passed
if [ $checkok -eq $checknum ]; then 
    echo -e "$filename:${GREEN}OK${NC}"
else 
    echo -e "$filename:${RED}FAIL${NC}"
    exit -1
fi