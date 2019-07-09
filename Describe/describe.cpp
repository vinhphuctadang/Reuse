#include "StringUtils.h"
#include "cxxopts/include/cxxopts.hpp"
#include <windows.h>
#include <cstring>
#include <stdio.h>
#include <string>
#include <fstream>
#pragma comment(lib, "User32.lib")
using namespace std;
typedef 
class DescribeException : public exception {
	protected:
		string information;
		int errCode; 
	public:
		DescribeException (const string& __information, int __errCode) : information (__information), errCode(__errCode) {
		}
		
		string getInformation () {
			return information;
		}
		
		int getErrCode () {
			return errCode;
		}
		
	    const char * what () const throw () {
	    	
	    	string result = information + ".Code=" + toString ((long long) errCode);
			return result.c_str();
	    }
};

void walk (const string& source, vector<string>& files, vector<string>& dirs) {
//	printf ("%s\n", source.c_str());
	WIN32_FIND_DATA ffd;
   	LARGE_INTEGER filesize;
   	size_t length_of_arg;
   	HANDLE hFind = INVALID_HANDLE_VALUE;
   	DWORD dwError=0;
	auto theDir = source + "\\*";

	hFind = FindFirstFile(theDir.c_str (), &ffd);
	
	do {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      	string tmp (ffd.cFileName);
      	if (tmp != "." && tmp != "..")
      		dirs.push_back(string (ffd.cFileName));
//         _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
      } else {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
         string tmp (ffd.cFileName);
         files.push_back(string (ffd.cFileName));
//         _tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
      }
    } while (FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   		throw DescribeException ("NO MORE FILE", -1);
}

regex toRegex (const string& pattern) {
	string proccessed_pattern = "";
	for (int i = 0; i<pattern.length(); ++i) {
		if (pattern[i] == '.' || pattern[i] == '\\')
			proccessed_pattern += "\\";
		else if	(pattern[i] == '*' || pattern[i] == '?' || pattern[i]=='^')
			proccessed_pattern += string("."); 
		proccessed_pattern += pattern[i];
 	} 
	return regex (proccessed_pattern);	
}

json load (const string& name) {
	ifstream file(name, ios_base::in);
	json result;
	if (file.good ()) {
		file >> result;
	} else
		throw DescribeException (string ("Bad file ") + name, -2);
	file.close ();
	return result;
}

void save (const string& name, json& j) {
	ofstream file(name, ios_base::out);
	if (file.good ()) {
		file << j;
	}
	file.close ();
}

json collect (const string& root, const string& current, 
	vector<regex>& filter, 
	vector<regex>& ignore,
	const string& virtual_dir // for regex to check matching expression pattern with ease
	) {
		
	json result;
	vector<string> files, dirs;
	auto theDir = root + "\\" + current;
	walk (theDir, files, dirs);
	
	result["name"] = current;
	vector<json> discription;
	
	for (auto name:files) {		
		
		bool cont = 0;
		
		for (auto reg : ignore) {
			if (regex_match (name, reg)) {
				cont = 1;
				break;
			}
		}
		if (cont) continue;
		
		if (filter.size()) {
			for (auto reg : filter)
				if (regex_match (name, reg)) {
					json temp;
					temp["name"] = name;
					discription.push_back(temp);
					break;
				}
		} else {
//			printf ("%s\n", name.c_str ());	
			json temp;
			temp["name"] = name;
			discription.push_back(temp);
		}
	}
	
	for (auto name: dirs) {
		bool cont = 0;
		for (auto reg : ignore) {
			
			if (regex_match (virtual_dir+"\\"+name, reg)) { 
				cont = 1;
				break;
			}
		}
		if (cont) continue;
		
		json temp = collect (theDir, name, filter, ignore, virtual_dir+"\\"+name); // recursively call
		discription.push_back(temp);
	}
	
	result ["content"] = discription;
	return result;
}

cxxopts::Options init (char* program) {
	cxxopts::Options options(program, " - describe current directory to json file *.drive");
	options.add_options()
		("p,path", "Origianl path to describe, '.' by default", cxxopts::value<std::string>())
  		("i,ignore", "Ignore file/folder", cxxopts::value<vector<string>>())
  		("f,filter", "Filter (only accept)", cxxopts::value<vector<string>>())
		("o,output", "File name", cxxopts::value<string>())
		("m,main", "Main file (for execution, optional)", cxxopts::value<std::string>())
		("h,help", "Show help", cxxopts::value<bool>())
		;	
	return options;
}

void input (cxxopts::ParseResult& parsed, vector<regex>& filter, vector<regex>& ignore, string& root, string& main_value, string& oFile) {
	
	try {
		root = parsed ["p"].as <string> ();		
	} catch (exception& e) {
		root = ".";
	}
		
	
	try {
		oFile = parsed ["o"].as <string> ();		
	} catch (exception& e) {
		oFile = ".drive";
	}
		
	try {
		main_value = parsed ["m"].as <string> ();		
	} catch (exception& e) {
		main_value = "";
	}	
		
	vector<string> vec;
	try {
		vec = parsed ["f"].as <vector<string>> ();
		for (auto element : vec) {
			filter.push_back(toRegex (element));
		}
	} catch (exception& e) {
		filter = {};
	}
		
	try {
		vec = parsed ["i"].as <vector<string>> ();
		for (auto element : vec) {
			printf ("%s\n", element.c_str ());
			ignore.push_back(toRegex (element));
		}
	} catch (exception& e) {
		ignore = {};
	}
			
}

json run (vector<regex>& filter, vector<regex>& ignore, string& root, string& main_value) {
	auto result = collect (".", root, filter, ignore, "");
	if (main_value != "")
		result ["main"] = main_value;
	return result;
}

void done (json& j, const string& file) {
	save (file, j);
}

int main(int n, char** argv) {
	
	// "describe -o myfolder.drive -f *.py,*.exe,*.ini,*.json -i \content -i file*"
	vector<regex> filter, ignore;
	string root, main_value, oFile;
	auto options = init (argv[0]);
	auto parsed = options.parse (n, argv);
	bool h = false;
	try {
		h = parsed["h"].as <bool> ();
		if (h) {
			printf ("%s", options.help (options.groups ()).c_str ());
			return 0;
		}
	} catch (exception& e) {
	}
	
	input (parsed, filter, ignore, root, main_value, oFile);
	auto result = run (filter, ignore, root, main_value);
	done (result, root + "\\" + oFile);		
	return 0;
}
