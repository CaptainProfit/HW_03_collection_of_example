#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
filename=$(basename "$0")

checknum=0
checkok=0
checklist=(
"1"
"2"
"3"
"4"
"5"
"6"
"7"
"8"
"9"
"10"
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
    P="/sys/module/ex_queue/parameters/"
    echo "1 2 3" | tr ' ' '\n' | xargs -i echo {} > $P/add
    result=`cat $P/remove`
    check $result
    
    echo "4 5 6" | tr ' ' '\n' | xargs -i echo {} > $P/add
    for i in {1..2}; do
        result=`cat $P/remove`
        check $result
    done

    echo "7 8 9" | tr ' ' '\n' | xargs -i echo {} > $P/add
    for i in {1..6}; do
        result=`cat $P/remove`
        check $result
    done

    P="/sys/module/ex_queue/parameters/"
    echo "10 11 12" | tr ' ' '\n' | xargs -i echo {} > $P/add
    result=`cat $P/remove`
    check $result
}

insmod ex_queue.ko
test
rmmod ex_queue.ko

echo $checkok/$checknum passed
if [ $checkok -eq $checknum ]; then 
    echo -e "$filename:${GREEN}OK${NC}"
else 
    echo -e "$filename:${RED}FAIL${NC}"
    exit -1
fi