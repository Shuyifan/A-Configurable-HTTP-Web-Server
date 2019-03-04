#pragma once
#include <string>
#include <vector>

/**
Given a path, output the upper level path (e.g. "/static/sata" -> "/static"),
If str is empty, return an empty string.

@param string of a path
@return upper level path
*/
std::string get_upper_dir(std::string str);

/**
A function to find out the base dictory of this project

@param 
@return base directory
*/
std::string get_server_dir();


/**
Given a path of a file, output the name of the file (e.g. "/static/sata/abc.txt" -> "/abc.txt")
The str or the file does not exist, return an empty string.
@param  a path of the file
@return the name of the file
*/
std::string get_file_name(std::string str);

/**
Perform URL-decoding on a string. Returns false if the encoding was invalid.
*/
bool url_decode(const std::string& in, std::string& out);

void find_files_in_folder(std::string path, std::vector<std::string>& file_name);