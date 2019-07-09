#include <iostream>
#include "MsgComm.cpp"
#include "ini.h"

INI ini (".ini");



string getValue (MsgComm* conn, string key) {
	
	json jmsg;
		jmsg["type"]="request";
		jmsg["function"]="kernel";
//		jmsg["command"]="get";
		jmsg["request"]="get";
		jmsg["variable"]=key;
	string result = "";
	conn->SendMsg(jmsg.dump ());
	while (result == "") result = conn->RecvMsg ();
	json j = json::parse (result);
	
	return j["result"].dump ();
}
int main (int n, char** argv) {
	
	Socket::initialize();
	
	
	MsgComm conn;
	auto ip = ini["host"];
	auto port = (int) toLL (ini["port"]);
		
	try {
		conn.Connect (ip, port); 	
//		cout << getName (&conn);
	} catch (CodeException& e) {
		printf ("%s", e.what ());
		Socket::finalize();	
		return 0;
	}
	
//	conn.SendMsg("OKE");
			
	printf ("Connected\n");
	
	
	string varName = "";
	cin >> varName;
	while (varName != "exit") {
		string result;
//		for (int i = 0; i<100000;++i) 
//			result = getValue (&conn, varName);
//		printf ("%s\n", getValue (&conn, varName).c_str ());		
		cout << getValue (&conn, varName) << endl;
		cin >> varName;
	}
	
	
	conn.Close ();	
	Socket::finalize();	
}
