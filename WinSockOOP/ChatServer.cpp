#include "INI.h"
#include "MsgComm.cpp"
#include <thread>

#define TIME_OUT_LIMIT 3000

using namespace std;	
int port=-1;
MsgComm host;

umap<string, MsgComm*> connectors;
mutex forConnectors;
INI ini (".ini");

void handleMessages (MsgComm* conn, umap<string, MsgComm*>* connectors) {
//	conn->SendMsg ("Hello Client");
	
	conn->SendMsg ("Hello Client");
	while (1) {
		try {
			auto msg = conn->RecvMsg ();
			printf ("Msg via conn %p: %s\n", conn, msg.c_str ());			
			auto jmsg = json::parse (msg);
			
			// register on messages
			
			// then on, process "jmsg"
												
		} catch (CodeException& e) { // or instance of message connection exception
			// TODO: when socket is disconnected	
			break;
		} catch (exception& e) { // json and another exception
			json resp;
					resp ["type"] 		= "response";
//					resp ["request"] 	= "name";
					resp ["result"] 	= "error";
					resp ["describe"] 	= "bad request";
			conn->SendMsg (resp.dump ());
			break;
		}
	}
	
	forConnectors.lock ();
	conn->Close();
//	connectors->erase (name);
	printf ("conn: 0x%x closed\n", conn);
	forConnectors.unlock ();
}

//void asyncCommunication (MsgComm* conn, umap<string, MsgComm*>* connectors) {
//	// Initial part 
//	
//	auto now = now_micro ();
//	json jmsg;
//	
//	bool reject = false, allow = false;
//	string connName = "";
//	
//	printf ("Connection provision is acquiring at 0x%x\n", conn);
//	
//	do {
//		auto msg = conn->RecvMsg();
//		// name='expected_connectors_name (module name)'
//		// haven't thought yet
//		
//		if (msg != "")
//			try {
//				jmsg = json :: parse (msg);	
//				connName = jmsg ["name"]; // throws an exception if no key "name" is found
//				
//				forConnectors.lock ();
//				
//				if (exists (connName, *connectors)) {
//					forConnectors.unlock ();
//					throw CodeException (string ("Connector name '") + connName + "' conflicted.", (int) toLL (ini["conflict_error"]));				
//				}
//				
//				(*connectors)[connName] = conn;
//				printf ("A connection is established, connector name: %s, at 0x%x\n", connName.c_str(), conn); // Debug purpose only
//				
//				forConnectors.unlock ();
//				
//				allow = true;
//			} catch (CodeException& e) { // read error
//				printf ("Internal error for 0x%x: %s\n", conn, e.what ());
//				reject = true;
//				break;	
//			} catch (exception& e) { // json parsing - related error
//				// TODO: Try another error behavior due to the fact that the json couldn't be send
////				json resp;
////					resp ["type"] 		= "response";
////					resp ["request"] 	= "name";
////					resp ["result"] 	= "error";
////					resp ["describe"] 	= e.what (); // 
////				conn->SendMsg (resp.dump ());
//				reject = true;
//				break;
//			}
//	} while (!(reject || allow) && now_micro () - now <= TIME_OUT_LIMIT);
//	
//	if (reject) {
//		printf ("Rejected connection at 0x%x\n", conn); // Debug purpose only
//		conn->Close();
//		return;
//	}
//	
//	// else, start handling messages loop:)
//	// handleMessages (connName, conn, connectors);		
//}

void asyncAccept (MsgComm* host, umap<string, MsgComm*>* connectors) {
	
//	vector <thread*> thrdManager;
//	
	while (1) {
		auto conn = host->Accept();
		printf ("A connection is accepted at 0x%x ...\n", conn);
		thread  aThread (handleMessages, conn, connectors);
		aThread.detach ();
	}
}
void serverInitialize (MsgComm* host, int port, umap<string, MsgComm*>* connectors) {
	
	Socket::initialize();
	printf ("Binding %s on port %d\n", ini["host"].c_str(), port);
	host->Bind(ini["host"], port);
	printf ("Listening localhost on port %d\n", port);
	host->Listen ();
	
	thread acceptProgress (asyncAccept, host, connectors);
	acceptProgress.join ();	
}

void serverFinalize () {
	
	// TODO: terminate acceptProgress thread
	Socket::finalize ();
}
int main (int n, char** argv){
	
	port = (int) toLL (ini.get("port", "8888"));
//	if (n>1) port = (int) toLL (argv[1]);
	serverInitialize (&host, port, &connectors);
	serverFinalize ();
}
