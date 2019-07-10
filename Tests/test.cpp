#include <stdio.h>
#include "MsgComm.cpp"

int main () {
	
	Socket::initialize();
	
	MsgComm sock;
	sock.Bind ("localhost", 8888);
	sock.Listen ();
	
	while (1) {
		auto conn = sock.Accept ();
		if (conn != 0) {
			char* msg = "Hello World";
			for (int i = 0; i<3; ++i)
				conn->SendMsg (msg);
			printf ("A client served\n");
			conn->Close ();
			delete conn;
//			break;
		}
	}
	
	sock.Close ();	
	Socket::finalize();	
}

