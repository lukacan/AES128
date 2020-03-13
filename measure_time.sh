#!/bin/bash

echo "Testing AES for $1 itteration/s"
TIME_NI=$(ts=$(date +%s%N) ; ./aes_NI.o $1 ; tt=$((($(date +%s%N) - $ts)/1000000)) ; echo "Time taken by AES_NI: $tt milliseconds")
TIME_Simple=$(ts=$(date +%s%N) ; ./aes_simple.o $1 ; tt=$((($(date +%s%N) - $ts)/1000000)) ; echo "Time taken by AES_Simple: $tt milliseconds")
TIME_TBoxes=$(ts=$(date +%s%N) ; ./aes_TBoxes.o $1 ; tt=$((($(date +%s%N) - $ts)/1000000)) ; echo "Time taken by AES_TBoxes: $tt milliseconds")

echo $TIME_NI
echo $TIME_Simple
echo $TIME_TBoxes
