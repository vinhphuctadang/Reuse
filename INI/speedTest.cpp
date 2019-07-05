#include "ini.cpp"
#include <stdlib.h>
#define ull unsigned long long
#include <time.h>


ull now_micro () {
    struct timespec result;
    clock_gettime(CLOCK_REALTIME, &result); 
    return result.tv_sec * 1000 + result.tv_nsec / 1000000;
}

string randomString (int n) {
	string result = "";
	for (int i = 0; i<n; ++i) {
		result += (rand () % 26 + 'a');
	}
	return result;
}


int main () {
	
	auto marked = now_micro ();
	
	printf ("Start watch ...\n");
	INI ini ("input.ini");
	
	for (int i = 0; i<100000; ++i) {
		auto key = randomString (100),
			value = randomString (100);
		ini[key] = value;
	}
	
	ini.toFile("input.ini");
	printf ("Progress done after %u ms", now_micro () - marked);
}
