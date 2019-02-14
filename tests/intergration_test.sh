#!/bin/bash
# make input path for the script
# $? means exit status
# $! means current process id

PATH_FOR_SERVER="../build/bin/server test.conf"
if [ ! -d "../build" ]; then
    echo "here"
    PATH_FOR_SERVER="../build_coverage/bin/server test.conf"
fi
PATH_TO_ECHO_OUTPUT="echo/echo_output"
PATH_TO_ECHO_EXPECTED="echo/echo_output_expected"
PATH_TO_STATIC_EXPECT="static/expect/test.txt"
PATH_TO_STATIC_OUT="static/current/test.txt"
PATH_TO_STATIC_EXPECT_JPG="static/expect/test.jpg"
PATH_TO_STATIC_OUT_JPG="static/current/test.jpg"
PATH_TO_STATIC_EXPECT_HTML="static/expect/index.html"
PATH_TO_STATIC_OUT_HTML="static/current/index.html"
FLAG=0 
PORT=12345
timeout 3  $PATH_FOR_SERVER  & > /dev/null 2> /dev/null
PID=$!
# Here -X specify the port I am using
# -sS not show progress but to show errors 
# -d Send the specified data in an (HTTP) POST request
# check if the curl request work
curl -X POST -sS http://localhost:$PORT/echo -d "FISTTEST" -o $PATH_TO_ECHO_OUTPUT
cmp -n 20  $PATH_TO_ECHO_OUTPUT $PATH_TO_ECHO_EXPECTED
current=$?;
if [ $current -eq 0 ]
then
    echo "Success"
else
    FLAG=$((FLAG+1))
    echo "Fail"
fi

#check if the static file is the same
curl -sS http://localhost:$PORT/static/test.txt > $PATH_TO_STATIC_OUT
cmp -n 20 -s $PATH_TO_STATIC_OUT $PATH_TO_STATIC_EXPECT
current=$?
if [ $current -eq 0 ]
then
    echo "Success"
else
    FLAG=$((FLAG+1))
    echo "Fail"
fi

#check if the jpg file for static works
curl -sS http://localhost:$PORT/static/test.jpg > $PATH_TO_STATIC_OUT_JPG
cmp -n 20 -s $PATH_TO_STATIC_EXPECT_JPG $PATH_TO_STATIC_OUT_JPG
current=$?
if [ $current -eq 0 ]
then
    echo "Success"
else
    FLAG=$((FLAG+1))
    echo "Fail"
fi

#check if the jpg file for static works
curl -sS http://localhost:$PORT/static/index.html > $PATH_TO_STATIC_OUT_HTML
cmp -n 20 -s $PATH_TO_STATIC_OUT_HTML $PATH_TO_STATIC_EXPECT_HTML
current=$?
if [ $current -eq 0 ]
then
    echo "Success"
else
    FLAG=$((FLAG+1))
    echo "Fail"
fi


#check if the html pages for static works
kill $PID

if [ "$FLAG" == 0 ]
then 
    echo "Pass intergration test"
    exit 0
else
    echo "$FLAG" | od -c
    echo "Fail intergration test"
    exit 1
fi
