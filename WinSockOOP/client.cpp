#include <iostream>
#include "MsgComm.cpp"
	
int main () {
	
	Socket::initialize();
	
	MsgComm conn;
	try {
		conn.Connect ("localhost", 8888); 	
	} catch (CodeException& e) {
		printf ("%s", e.what ());
		return 0;
	}

	while (1) {
		string msg = conn.RecvMsg();
		cout << msg << endl;
	}
		
	conn.Close ();	
	Socket::finalize();	
}
