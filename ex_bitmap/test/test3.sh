#!/bin/bash
cd ..
filename=$(basename "$0")
checknum=0
checkok=0
checklist=(
"0b11101010001010001010001000001010000010001010001000001000001010000010001010000010001000001000000010001010001010001000000000000010"
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

    for p in {2..12}; do
        c=$((2*p))
        chk=`cat $P/print`
        chk=${chk:p + 1:1}
        if [ $chk -eq 1 ]; then
            while [ $c -le 128 ]; do
                echo $((c - 1)) >$P/clear
                c=$((c + p))
            done
        fi
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
