#include <windows.h>
#include <cstring>
#include <ctime>
#include <sstream>
#include <vector>
#include <unordered_map>

#ifndef CTILS_H
#include "CTils.h"
#endif

#include "E:\\CPP\\HNhu\\json-develop\\single_include\\nlohmann\\json.hpp"

#define umap unordered_map
#define HNHU_MAX_PATH 702
#define exists(x,y) ((y).find(x) != (y).end())

using namespace std;
using json=nlohmann::json;

// TODO: WRITE A TOKEN RECOGNITOR, now; it acts like a variable recognitor
 
typedef

	umap<string, string> dict;
	umap<char, char> emap = {
		{'\"', '\"'},	
		{'\?', '\?'},	
		{'\\', '\\'},	
		{'a', '\a'},
		{'b', '\b'},	
		{'f', '\f'},	
		{'n', '\n'},	
		{'r', '\r'},	
		{'t', '\t'},	
		{'v', '\v'},
	};
	umap<char, string> demap = {
		{'\"', "\\\""},	
		{'\?', "\\\?"},	
		{'\\', "\\\\"},	
		{'\a', "\\a"},
		{'\b', "\\b"},	
		{'\f', "\\f"},	
		{'\n', "\\n"},	
		{'\r', "\\r"},	
		{'\t', "\\t"},	
		{'\v', "\\v"},
	};

string decodeEscapeChar (string& s) {
	string res;
	for(int i=0;i<s.length (); ++i) {
		if (s[i] == '\\') {

			if (i+1<s.length ()) {
				if (exists (s[i+1], emap))
					res += emap[s[++i]];
				else {
					res += '\\';
					res += s[++i];
				}
					
			} else 
				res += '\\';
		} else 
			res += s[i];
	}
	return res;
}

string encodeEscapeChar (string& s){
	string res = "\"";
	for (auto c:s)
		if (exists (c, demap)){
			res += demap[c];
		} else
			res += c;
	res += "\"";
	return res;
}

string getCurrentDir () {
	char buffer[HNHU_MAX_PATH];
	GetCurrentDirectoryA (HNHU_MAX_PATH, buffer);
	string result (buffer);
	return result;
}

int setCurrentDir (string dir) {
	
	if (!SetCurrentDirectory (dir.c_str ()))
		return GetLastError ();
	return 0;	
}

string getModuleFileDir () {
	char buffer[HNHU_MAX_PATH];
	GetModuleFileName (NULL, buffer, HNHU_MAX_PATH);
	string result (buffer);
	return buffer;
}

string getModuleDir () {
	string fileName = getModuleFileDir ();
	while (fileName[fileName.length () - 1] != '\\') 
		fileName.pop_back();
	return fileName;
}

char toLower (char c) {
	if ('A' <= c && c <= 'Z')
		return c + 32;
}

string paddingLeft (string s, int numOfChar) {
	while (s.length () < numOfChar) s+=' ';
	return s;
}

bool exist (string token, string& s) {
	return s.find (token) != string::npos;
}

bool replace (string token, string with, string& s) {
	size_t pos = s.find (token);
	if (pos == string::npos)
		return false;		
	s.replace(pos, token.length (), with);
	return true;
}

string proper (string source) {
	string result;
	int i = 0; 
	int n = source.length ();
	for(i=0; i < n; ++i)
		if (source[i] != ' ') break;
		
	while (n && source[n-1] == ' ') --n;
	
	bool quotes = false;
	for(;i<n; ++i) {
		
		if (source[i] == '"') {
			result += source[i];
			quotes = !quotes;
			continue;
		}
		if (quotes) {
			result += source[i];
			continue;
		}
				
		if (source[i] == ' ' && (i == 0 || source[i-1] != ' ')) {
			result += source[i];
		} else if (source[i] != ' ')		
			result += source[i];
	}
	return result;
}

vector<string> toParams (string& s) {
	vector<string> result;
	s = proper (s);
	string tmp;
	bool quote = false;
	for(int i = 0; i<s.length (); ++i) {
		
		if (s[i] == '\\'){
			tmp += '\\';
			if (i+1<s.length ()) tmp += s[++i];
			continue;
		}
		
		if (s[i] == '"') {
			quote = !quote;		
			continue;
		}
		
		if (quote){
			tmp += s[i];
		} 	
		else 
			if (s[i] == ' ') {
				//tmp = convertEscapeChar (tmp);
				result.push_back (tmp);
				tmp = "";
			} else {
				tmp += s[i];
			}
	}
	
	if (tmp != "") {
		result.push_back (tmp);
	}
		
	return result;
}

vector<string> toVector (int n, char* args[]) {
	string tmp;
	vector<string> result;
	for (int i = 0; i<n; ++i) {
		tmp.assign (args[i]);	
		result.push_back (tmp);
	}
	return result;
}

bool checkProtected (string& s) {
	
	for(auto it:s) {
		if (it == ' ')
			return true;
		if (it == '^')
			return true;
	}
		
	return false;	
}

string vectorToString (vector<string>::iterator from, vector<string>::iterator to, bool connectParams) {
	string result = "";
	for(auto it = from; it != to; ++it) {
		if (connectParams && checkProtected (*it)) 
			result += string("\"") + *it + "\"";
		else 
			result += *it;
		if (it+1 != to) result += ' ';
	}
			
	return result;
}

long long toLL (string s) {
	stringstream ss(s);
	long long result;
	ss >> result;
	return result;
}
string toString (long long n) {
	stringstream ss;
	ss << n;
	string result;
	ss >> result;
	return result;
}

string toString (unsigned long long n) {
	stringstream ss;
	ss << n;
	string result;
	ss >> result;
	return result;
}

string toString (double n) {
	stringstream ss;
	ss << n;
	string result;
	ss >> result;
	return result;
}

//  void outp (vector<string> v) { // DEBUG PURPOSE ONLY
//  	for(auto it:v) cout << it << endl;
//  	cout << endl;
//  }

string __s__ (long long n, string es) { // decide if an -s/-es could be added or not
	if (n > 1) return es;
	return "";
}

size_t seekToken (string& s, char c, int from, string& result, int charCnt = 1) {
	bool quote = false;
	for (int i=from; i<s.length (); ++i) {
		if (s[i] == '"') {quote = !quote; continue;}
		if (!quote && s[i] == c) {
			result = s.substr (i+1, charCnt);
			return i;
		}
	}
	return string::npos;
}

// TODO: Write appending missing token to params

//int loadParam (string maskToken, string& cmd, 
//vector<string>::iterator from, vector<string>::iterator to) {
//	int result = 0;
//	
//	for(auto i=from; i != to && exist (maskToken, cmd); ++i){
//		
//		if (checkProtected (*i))
//			replace (maskToken, '"' + *i + '"', cmd);
//		else 
//			replace (maskToken, *i, cmd);
//		result ++;
//	}
//	
//	while (exist (moduleDirToken, cmd)) replace` (moduleDirToken, getModuleDir(), cmd);
//	return result;		
//}

//string nextSubstr (int n, string& src) {
//	string result = src.substr(0, n);
//	src.erase(0, n);
//	return result;	 
//} 

string nextToken (string token, string& src) {}// build later using KMP
void upQuoteLevel (string& s) {// convert ' to ", just use it in processing purposes only
	bool quotes = false;
	for(auto& it:s) {
		if (it == '"') quotes = !quotes;
		if (!quotes && it=='\'') it='"';
	}
}

string lowerCaseAll (string s) {
	for(auto& it:s) if ('A' <= it && it <= 'Z') it+=32;
	return s;
}

string nextAmpersand (string& src) {
	string result;
	bool quote = false, amper = false;
	for (int i = 0; i<src.length(); ++i) {
		
		if (src[i] == '"') {quote = !quote; amper = false; continue;}
		if (!quote && src[i] == '&') {
			if (amper) {
				result = src.substr (0, i-1), src.erase(0, i+1); 
				return result;
			}
			amper = true;	
		} else 
			amper = false;
	}
	result = src, src ="";
	return result;
}

string randStr (int len = 14) {

	char* res = createId (toString ((long long) len).c_str (), len);
	string result (res);
	free (res);
	return result;
}

void loadVars (string& src, dict& vars, char symbolize_char = '%'){	
	for(size_t i = 0; (i=src.find(symbolize_char, i), i != string::npos) && i<src.length();){
		int pivot = i++;
		if (i<src.length() && src[i] == symbolize_char) {
			src.erase(i-1,1);
			continue;
		}
		string tmp;
		while (i<src.length() && src[i] != symbolize_char) tmp += src[i++];
		if (exists (tmp, vars) && i < src.length ()) {	
			string rep = vars[tmp];
			src.replace(pivot, tmp.length()+2, rep);
			i=i-tmp.length()-2+rep.length ();
		}
	}
}
