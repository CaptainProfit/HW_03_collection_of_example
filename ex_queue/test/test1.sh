#!/bin/bash
P="/sys/module/ex_queue/parameters/"
insmod ex_queue.ko
echo "1 2 3" | tr ' ' '\n' | xargs -i echo {} >${P}add
echo x >${P}remove 
echo "4 5 6" | tr ' ' '\n' | xargs -i echo {} >${P}add
echo x >${P}remove 
echo x >${P}remove 
echo "7 8 9" | tr ' ' '\n' | xargs -i echo {} >${P}add
echo x >${P}remove 
echo x >${P}remove 
echo x >${P}remove 
echo x >${P}remove 
echo x >${P}remove 
echo x >${P}remove 
echo "10 11 12" | tr ' ' '\n' | xargs -i echo {} >${P}add
echo x >${P}remove 
rmmod ex_queue
