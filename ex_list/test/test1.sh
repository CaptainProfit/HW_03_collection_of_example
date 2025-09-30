#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
filename=$(basename "$0")

checknum=0
checkok=0
checklist=(
"{[], 4, 3, 2, 1}"
"4"
"3"
"{[2], 1}"
"{[2], 8, 7, 6, 5, 1}"
"2"
"8"
"7"
"{[6], 5, 1}"
"6"
"5"
"1"
"{[]}"
)

function check() {
    conc="$*"
    if [ "$conc" == "${checklist[$checknum]}" ]; then
        echo $filename: part${checknum} passed
        ((checkok++))
    else
        echo $filename: part${checknum} failed
        echo "    result:  \"$conc\""
        echo "    must be: \"${checklist[$checknum]}\""
    fi
    ((checknum++))
}

function test() {
    P="/sys/module/ex_list/parameters/"
    echo "1 2 3 4" | tr ' ' '\n' | xargs -i echo {} > $P/add
    result=`cat $P/print`
    check $result

    echo 1 >$P/next
    for i in {1..2}; do
        result=`cat $P/remove`
        check $result
    done
    result=`cat $P/print`
    check $result

    echo "5 6 7 8" | tr ' ' '\n' | xargs -i echo {} > $P/add
    result=`cat $P/print`
    check $result

    for i in {1..3}; do
        result=`cat $P/remove`
        check $result
    done
    result=`cat $P/print`
    check $result
    
    for i in {1..3}; do
        result=`cat $P/remove`
        check $result
    done
    result=`cat $P/print`
    check $result
}

insmod ex_list.ko
test
rmmod ex_list.ko

echo $checkok/$checknum passed
if [ $checkok -eq $checknum ]; then 
    echo -e "$filename:${GREEN}OK${NC}"
else 
    echo -e "$filename:${RED}FAIL${NC}"
    exit -1
fi