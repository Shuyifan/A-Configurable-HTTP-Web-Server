#include <string>

// Given a path, output the upper level path (e.g. "/static/sata" -> "/static")
// If str is empty, return an empty string.
std::string get_upper_dir(std::string str);

// A function to find out the base dictory of this project
std::string get_server_dir();

// Given a path of a file, output the name of the file (e.g. "/static/sata/abc.txt" -> "/abc.txt")
// The str or the file does not exist, return an empty string.
std::string get_file_name(std::string str);