//#include "cxxopts/include/cxxopts.hpp"
//#include "StringUtils.h"
#include <iostream>
#include <cstring>
#include <string>
#include <regex>

using namespace std;
//using namespace cxxopts;

//int n = 3;
char* emu_args[] = {"test.exe", "-d", "-o", "my.drive", "-f", "*.py,*.inc,*.p"};

void toCharSS (int n, char* a[], char**& dst){
	dst = (char**) malloc (n*sizeof(char*));
	for (int i = 0;i<n;++i) dst[i] = a[i];
}
void parser (int& n, char**& argv) {
//	toCharSS (n=6, emu_args, argv);
	
//	Options options(argv[0], " - example command line options");
//	options.add_options()
//  		("d,debug", "Enable debugging")
//  		("o,output", "File name", value<std::string>())
//  		("f,filter", "Filter", value<vector<string>>());	
// 	auto result = options.parse(n, argv);
// 	auto vec = result["f"].as <vector<string>> ();
// 	for (auto name:vec) printf ("%s\n", name.c_str ());
}
int main (int n, char** argv) {
	  if (regex_match ("aa.sys", regex(".*\\.system") ))
    		cout << "string literal matched\n";
}
