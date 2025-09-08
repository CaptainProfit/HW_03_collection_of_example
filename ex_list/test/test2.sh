#!/bin/bash
insmod ex_list.ko
# добавляю в начало, удаляю из конца
P="/sys/module/ex_list/parameters/"
echo "1 2 3 4" | tr ' ' '\n' | xargs -i echo {} > $P/add
echo 0 >$P/print
echo 0 >$P/prev
echo "0 0" | tr ' ' '\n' | xargs -i echo {} > $P/remove
echo 0 >$P/next
echo 0 >$P/print
echo "5 6" | tr ' ' '\n' | xargs -i echo {} > $P/add
echo "7 8" | tr ' ' '\n' | xargs -i echo {} > $P/add
echo 0 >$P/prev
echo 0 >$P/print
echo "0 0 0" | tr ' ' '\n' | xargs -i echo {} > $P/remove
echo 0 >$P/print
echo "0 0 0" | tr ' ' '\n' | xargs -i echo {} > $P/remove
echo 0 >$P/print
rmmod ex_list.ko
