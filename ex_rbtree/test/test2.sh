#!/bin/bash
filename=$(basename "$0")
checknum=0
checkok=0
checklist=(
"tree: {0, 1, 2, 3, 4, 5, 6, 7, 9, 10}"
"tree: {1, 3, 5, 7, 9}"
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
    for i in {0..10}; do
        echo $i >$P/insert
    done
    result=`cat $P/print`
    check $result

    for i in {0..10..2}; do
        echo $i >$P/remove
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
