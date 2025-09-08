#!/bin/bash
P="/sys/module/ex_queue/parameters/"
insmod ex_queue.ko 
echo x >${P}remove 
rmmod ex_queue
