#!/bin/bash

cd $(dirname $0)
UARTLOG=../uartlog
STATUS=0

last() {
	STATUS=1
}

echo_random() {
	while [[ $STATUS -eq 0 ]]
	do
		current=$RANDOM
		if [[ $current -gt 30000 ]]
		then
			echo "new file($current)"
		else
			echo "$current"
		fi
	
		sleep 1
	done
}

trap last 1 2 3 15

echo_random | $UARTLOG -s "new file" -l latest.log -f %H%M%S.log
