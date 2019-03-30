#!/bin/bash

echo "Starting LLDB server on ports 7100-7110."
lldb-server platform --server --listen "*:7100" -m 7101 -M 7110
