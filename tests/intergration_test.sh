#!/bin/bash
# make input path for the script
# $? means exit status
# $! means current process id

PATH_FOR_SERVER="../build/bin/server"
if [ ! -d "../build" ]; then
    PATH_FOR_SERVER="../build_coverage/bin/server"
fi
PATH_TO_CONFIG="test.conf"
PATH_TO_ECHO_OUTPUT="echo/echo_output"
PATH_TO_ECHO_EXPECTED="echo/echo_output_expected"
PATH_TO_ERROR_OUTPUT="error/error_output"
PATH_TO_ERROR_EXPECTED="error/error_output_expected"
PATH_TO_STATUS_OUTPUT="status/status_output"
PATH_TO_STATUS_EXPECTED="status/status_output_expected"
PATH_TO_STATIC_EXPECT="static/expect/test.txt"
PATH_TO_STATIC_OUT="static/current/test.txt"
PATH_TO_STATIC_EXPECT_JPG="static/expect/test.jpg"
PATH_TO_STATIC_OUT_JPG="static/current/test.jpg"
PATH_TO_STATIC_EXPECT_HTML="static/expect/index.html"
PATH_TO_STATIC_OUT_HTML="static/current/index.html"
PATH_TO_PROXY_OUT="proxy/proxy_output"
FLAG=0 
PORT=12345
timeout 100  $PATH_FOR_SERVER $PATH_TO_CONFIG  & > /dev/null 2> /dev/null
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

curl -X POST -sS http://localhost:$PORT/ -d "SECONDTEST" -o $PATH_TO_ERROR_OUTPUT
cmp -n 20  $PATH_TO_ERROR_OUTPUT $PATH_TO_ERROR_EXPECTED
current=$?;
if [ $current -eq 0 ]
then
    echo "Success"
else
    FLAG=$((FLAG+1))
    echo "Fail"
fi


curl -X POST -sS http://localhost:$PORT/status -d "THIRDTEST" -o $PATH_TO_STATUS_OUTPUT
cmp -n 20  $PATH_TO_STATUS_OUTPUT $PATH_TO_STATUS_EXPECTED
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

#check proxy handler

#first start another instance of the server
SECOND_PORT=12346
PATH_TO_SECOND_CONFIG="proxy_test.conf"
timeout 100  $PATH_FOR_SERVER $PATH_TO_SECOND_CONFIG  & > /dev/null 2> /dev/null
SECOND_PID=$!

#now issue a static file request for the proxied server
echo "TODO -- delete dummy curl statement"
curl -i http://localhost:$PORT/proxy/static/test.txt
curl -sS http://localhost:$PORT/proxy/static/test.txt > $PATH_TO_PROXY_OUT
cmp -n 20 -s $PATH_TO_PROXY_OUT $PATH_TO_STATIC_EXPECT
current=$?
if [ $current -eq 0 ]
then
    echo "Success"
else
    FLAG=$((FLAG+1))
    echo "Fail"
fi

#cleanup second server
kill $SECOND_PID



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

