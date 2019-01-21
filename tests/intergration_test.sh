#!/bin/bash

STRING1=`echo -e "hello" | nc localhost 80`
STRING2=$'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nhello'


if [ "$STRING1" == "$STRING2" ]; then
    echo "pass"
else
    echo "fail"
fi