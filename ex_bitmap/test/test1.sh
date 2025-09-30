#!/bin/bash
cd ..
filename=$(basename "$0")
checknum=0
checkok=0
checklist=(
"0b10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
"0b11111111111111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
"0b11111111111111111111111111111111111111111111111111111111111111110000000000000000000000000000000000000000000000000000000000000000"
"0b11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
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
    echo 0 >$P/set
    result=`cat $P/print`
    check $result

    for i in {1..15}; do
        echo $i >$P/set
    done
    result=`cat $P/print`
    check $result

    for i in {16..63}; do
        echo $i >$P/set
    done
    result=`cat $P/print`
    check $result

    for i in {64..127}; do
        echo $i >$P/set
    done
    result=`cat $P/print`
    check $result
}

insmod ex_bitmap.ko
test
echo $checkok/$checknum passed
if [ $checkok -eq $checknum ]; then 
    echo $filename:OK
else 
    echo $filename:FAIL
fi
rmmod ex_bitmap.ko
