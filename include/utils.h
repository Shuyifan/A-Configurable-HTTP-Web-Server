#pragma once
#include <string>
#include <vector>

/**
 * Given a path, output the upper level path (e.g. "/static/sata" -> "/static"),
 * If str is empty, return an empty string (e.g. "" -> "").
 * @param  str String of a path
 * @return upper level path
*/
std::string get_upper_dir(std::string str);

/**
 * A function to find out the base dictory of this project
 * @return the base directory of the server.
*/
std::string get_server_dir();

/**
 * Given a path of a file, output the name of the file (e.g. "/static/sata/abc.txt" -> "/abc.txt")
 * The str or the file does not exist, return an empty string.
 * @param str A path of the file
 * @param contain_extension If true, the function will return the name with extension (e.g. "abc.txt");
 *                          else return the name without the extension (e.g. "abc") 
 * @return the name of the file
*/
std::string get_file_name(std::string str, bool contain_extension = true);

/**
 * Perform URL-decoding on a string. Returns false if the encoding was invalid.
 * @param in The input URL.
 * @param out The output URL after being decoded from the input URL.
 * @return whther the encoding is valid. 
*/
bool url_decode(const std::string& in, std::string& out);

/**
 * Find all the files in a directory, save the files' direcotries in the a vector ("file_name").
 * @param path The directory.
 * @param file_name A vector where we store the files' location.
*/
void find_files_in_folder(std::string path, std::vector<std::string>& file_name);

/**
 * Sanitize the input.
 * @param rawInput The input needed to be santized.
*/
void sanitizeInput(std::string& rawInput);