#!/bin/bash

INPUTC=`echo -e "GET / HTTP/1.1\r\n\r\nxxxxx\r\n\r\n" | nc localhost 8080 -w 5`
OUTPUTC=$'HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nGET / HTTP/1.1\r\n\r\nxxxxx\r\n\r\n'
OUTPUTC=${OUTPUTC%$'\n'}

if [ "$INPUTC" == "$OUTPUTC" ]; then
    echo "pass"
else
    echo "fail"
    echo "$INPUTC" | od -c
    echo "$OUTPUTC" | od -c
fi