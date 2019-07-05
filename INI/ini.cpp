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
		// constructors
		INI ();
		INI (umap <string, string>& __map);
		INI (const string& fileName); 
		INI (const INI& ini);
		// utils
		void fromFile (const string& fileName);
		void toFile   (const string& fileName);
		
		string 		get (string key);
		string 		get (string key, string default_value);
		void 		set (string key, string value);
		void 		remove (string key);
	
		string&		operator[] (string key); // for intuition only, make use of get, set and remove
		
		friend ostream& operator<<(ostream& ostr, const INI& ini){
			
			for (auto element : ini.mapping) {
				ostr << "'" << element.first << "'='" << element.second << "' " << endl;
			}	
			return ostr;
		}
		
		umap <string, string>::iterator begin() { return mapping.begin (); }
	    umap <string, string>::const_iterator begin() const { return mapping.begin (); }
	    umap <string, string>::iterator end() { return mapping.end (); }
	    umap <string, string>::const_iterator end() const { return mapping.end (); }
	private:
		pair <string, string> 	fromString (const string& sample);		
	
};

// constructors
INI::INI () {
	mapping.clear ();	
}

INI::INI(const string& fileName) {
	fromFile (fileName);
}

INI::INI (const INI& ini) {
	mapping = ini.mapping; // clone a difference INI object
}

INI::INI (umap <string, string>& __map) {
	mapping = __map;
}


// utils

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

void INI::fromFile (const string& fileName) {
	
	mapping.clear ();
	ifstream file (fileName, ios_base::in);
	while (!file.eof ()) {
		string sample;
		getline (file, sample);	
		try {
			auto each = fromString (sample);
			mapping [each.first] = each.second;
		} catch (exception& e) {
			// TODO: output log exceptions	
		}
	}
	file.close ();
}

void INI::toFile   (const string& fileName) {
	fstream file;
	file.open (fileName.c_str (), ios_base::out);
	for (auto value : mapping) {
		string line = value.first+"="+value.second;// formation performed
		file << line << endl;
	}	
	file.close();
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

void INI::remove (string key) {
	auto it = mapping.find (key);
	if (it != mapping.end ()) mapping.erase (it);
}

string& INI::operator[] (string key) {
	return mapping [key];
}

