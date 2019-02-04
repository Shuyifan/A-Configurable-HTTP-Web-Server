#include <stdio.h>
#include<iostream>

#include "stdlib.h"


std::string get_upper_dir(std::string str) {
    if(str.length() == 0) {
        return str;
    } else if(str.rfind("/") == -1) {
        return "";
    } else {
        return str.substr(0, str.rfind("/"));
    }
}

std::string get_server_dir() {
    std::string file_path = __FILE__;
    file_path = get_upper_dir(file_path);
    file_path = get_upper_dir(file_path);
    return file_path;
}

std::string get_file_name(std::string str) {
    if(str.rfind("/") == -1) {
        return "";
    } else {
        return str.substr(str.rfind("/"));
    }
}