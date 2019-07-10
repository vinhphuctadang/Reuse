#include <iostream>
//#include "StringUtils.h"
#include "MsgComm.cpp"
#include "ini.h"

INI ini (".ini");

int main (int n, char** argv) {
	
	Socket::initialize();
		
	MsgComm conn;
	auto ip = ini["host"];
	auto port = (int) toLL (ini["port"]);
		
	try {
		conn.Connect (ip, port); 	
	} catch (CodeException& e) {
		printf ("%s", e.what ());
		Socket::finalize();	
		return 0;
	}
	
	conn.SendMsg (ini["register"]);
	string listen = "";
	while (listen == "") listen = conn.RecvMsg();
	
	printf ("Recieved %s\n", listen.c_str());
		
	while (1) {
		try {
			auto msg = json::parse (conn.RecvMsg());
			printf ("Comming message: %s\n", msg.dump ().c_str ());
			
			if (msg["type"] == "request" && 
				msg["command"] == "get") {
				
					
				string variable = msg ["variable"];
				string result = ini.get (variable, "KHONG TIM THAY");
				json jmsg;
					jmsg["type"] = "response";
					jmsg["source"] = msg["source"]; // mandatory
					jmsg["success"] = "true";
					jmsg["result"] = result;
				conn.SendMsg(jmsg.dump ());
			}
		} catch (CodeException& e) {
			break;	
		} catch (exception& e) {
			
			printf ("Progress failed\n");	
			break;
		}
	}
		
	conn.Close ();	
	Socket::finalize();	
}
