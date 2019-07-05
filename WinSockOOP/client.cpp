#include <iostream>
#include "MsgComm.cpp"
#include "ini.h"
	
int main (int n, char** argv) {
	
	Socket::initialize();
	INI ini (".ini");
	
	MsgComm conn;
	auto ip = ini["host"];
	auto port = (int) toLL (ini["port"]);
		
	try {
		conn.Connect (ip, port); 	
//		conn.SendMsg("{\"name\":\"PC\"}");
	} catch (CodeException& e) {
		printf ("%s", e.what ());
		return 0;
	}
	
//	conn.SendMsg("OKE");

	while (1) {
		string msg = conn.RecvMsg();
		cout << msg << endl;
	}
		
	conn.Close ();	
	Socket::finalize();	
}
