#include <stdio.h>
#include<iostream>

#include "stdlib.h"

std::string get_server_dir() {
    std::string file_path = __FILE__;
    file_path = file_path.substr(0, file_path.rfind("/"));
    file_path = file_path.substr(0, file_path.rfind("/"));
    return file_path;
}