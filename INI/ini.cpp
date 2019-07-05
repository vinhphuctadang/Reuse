#include <stdio.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <unordered_map>
#define umap unordered_map
using namespace std;

class INI {
	
	private:
		umap <string, string> mapping;
		
	public:
		INI ();
//		INI (const string& fileName);
//		INI (INI& ini);
//		INI ()

		void fromFile (const string& fileName);
//		void fromStream (const istream& stream);
		void print ();
		
		string get (string key);
		string get (string key, string default_value);
		void set (string key, string value);
		
	private:
		pair <string, string> fromString (const string& sample);		
		
};



pair <string, string> INI::fromString (const string& sample) {
	std:: string key = "", value = "";
	int i;
	for (i = 0; i < sample.length (); ++i) {
		if (sample [i] == '=') {
			break;	
		} else 
			key += sample[i];
	}
	
	return {key, sample.substr(i+1, sample.length() - i - 1)};	
}

//void INI::fromStream (const istream& stream) {
//	
//}
void INI::fromFile (const string& fileName) {
	
	ifstream file (fileName, ios_base::in);
	while (!file.eof ()) {
		string sample;
		getline (file, sample);	
//		cout << sample << endl;
		try {
			auto each = fromString (sample);
			mapping [each.first] = each.second;
		} catch (exception& e) {
			// TODO: output log exceptions	
		}
	}
	file.close ();
}

void INI::print() {
	for (auto element : mapping) {
		cout << "'" << element.first << "' = '" << element.second << "' " << endl;
	}
}

string INI::get (string key){
	return mapping [key];
}

string INI::get (string key, string default_value){
	auto it = mapping.find (key);
	if (it != mapping.end ())
		return it->second;
	return default_value;
}

void INI::set (string key, string value) {
	mapping [key] = value;
}

INI::INI () {
	
//	auto res = fromString ("First_Word=Hello world");
//	std::cout << res.first << "=" << res.second << "|" << std::endl;
}

int main () {
	INI ini;
	ini.fromFile("input.ini");
	ini.print ();
//	ini
}
