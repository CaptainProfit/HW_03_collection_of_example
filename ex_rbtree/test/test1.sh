#!/bin/bash
filename=$(basename "$0")
checknum=0
checkok=0
checklist=(
"tree: {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}"
"tree: {0, 1, 6, 10, 11, 16, 20, 21, 26, 30, 31, 36, 40, 41, 46, 50, 51, 56, 60, 61, 66, 70, 71, 76, 80, 81, 86, 90, 91, 96, 100}"
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
    P="/sys/module/ex_rbtree/parameters"
    for i in {0..100..10}; do
        echo $i >$P/insert
    done
    result=`cat $P/print`
    check $result

    for i in {1..100..5}; do
        echo $i >$P/insert
    done
    result=`cat $P/print`
    check $result
}

cd ..
insmod ex_rbtree.ko
test
echo $checkok/$checknum passed
if [ $checkok -eq $checknum ]; then 
    echo $filename:OK
else 
    echo $filename:FAIL
fi
rmmod ex_rbtree
