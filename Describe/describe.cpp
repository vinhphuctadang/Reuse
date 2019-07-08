#include "StringUtils.h"
//#include "cxxopts/include/cxxopts.hpp"
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


json collect (const string& parentDir, const string& dir) {
	
	json result;
	vector<string> files, dirs;
	auto theDir = parentDir + "\\" + dir;
	walk (theDir, files, dirs);
	
	result["name"] = dir;
	
	vector<json> discription;
	
	for (auto name:files) {
		json temp;
		temp["name"] = name;
		discription.push_back(temp);
	}
	
	for (auto name: dirs) {
		json temp = collect (theDir, name); // recursively call
		discription.push_back(temp);
	}
	
	result ["content"] = discription;
	return result;
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

int main(int argc, char *argv[]) {
	
	return 0;
}
