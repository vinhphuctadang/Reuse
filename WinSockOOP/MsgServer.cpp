#include "MsgComm.cpp"
#include "DisplayUtils.h"
#include "SafeQueue.h"
#include <iostream>
#include <thread> 
#include <vector>
#include <mutex>


SafeQueue<string> sq;
vector<MsgComm*> connections;
mutex m;


MsgComm sock;
int main (int n, char** argv) {
	Socket::initialize();
		
	int port = -1;
	if (n>1) port = (int) toLL (argv[1]);
	while (1) {
		if (port < 0)
			port = (int) toLL (input ("Enter Port: "));
		
		try {
			sock.Bind ("localhost", (int) port);
			sock.Listen();
		} catch (exception& e) {
			printf ("%s\n", e.what ());
			port =  (int) toLL (input ("Enter Port: "));
			continue;
		}
		
		break;	
	}
	
	
	auto f = [](MsgComm* comm) { 
		
		
    	while (1) { // Asynchorously 
    		auto conn = comm->Accept ();
    		//    		printf ("A connection is established\n");
    		if (conn != NULL) {
    			
    			m.lock ();
    			
				connections.push_back (conn);
				
				m.unlock ();
			}
		}
    }; 
	
	thread th3(f, &sock); 	
//    
    string msg_to_send = "";
	while (msg_to_send != "exit") {
		msg_to_send = input ("Message to send: ");
		
		m.lock ();
			for (auto v: connections) {
				v->SendMsg (msg_to_send);
				v->SendMsg (msg_to_send);
			}
		m.unlock ();
//		sq.push(msg_to_send);
	}
	
	for (auto v:connections) {
		v->Close ();
		delete v;
	}
}
