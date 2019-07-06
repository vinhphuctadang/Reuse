#include "INI.h"
#include "MsgComm.cpp"
//#include "DisplayUtils.h"
#include <thread>
#define TIME_OUT_LIMIT 3000
#define GateDict umap<string, MsgComm*>
//#include <unistd.h>

using namespace std;	
int port=-1;
MsgComm host;

GateDict connectors;
GateDict requestTable;
GateDict functionHub;

mutex forConnectors;
mutex forRequestTable;
mutex forFunctionHub;
INI ini (".ini");

class BadRequestException : public CodeException {
	public:
	
	BadRequestException (const char* __inf, int errCode) : CodeException (__inf, errCode) {}
	const char * what () const throw () {
    	string result = string ("Bad request: ") + CodeException :: what ();
		return result.c_str();
    }
//};
};



string generateId () {
	return randStr (14);
}


void kernelHandling (MsgComm* conn, GateDict* connectors, GateDict* requestTable, json& jmsg) {
	
	try {
		string command = jmsg ["command"];
		if (command == "name") {
			
			json resp;
				resp ["sucess"] = "true";
				resp ["result"] = "Chat Server MayMay";
			conn->SendMsg (resp.dump ());
		} 
		
		// other handling
				
	} catch (exception& e) {
		throw BadRequestException (e.what (), 1);
	}
}


//request structure:
//	type: request
//	target: <target module name>/"kernel"
//	command: "command name"
//	args: [] // if any
//	


// response structure:
//		type: response
//		source: request_id
//		success: true/false
//		// substances

// register structure:
//		type: register
//		module: <module name>
//		function: 
//			[
//				 
////					<name>, <function's name>			
////					
////					// example: "copy", "getDirs"
////					
//				]
//			

void requestToTarget (MsgComm* conn, MsgComm* target, GateDict* requestTable, json& jmsg) {
	
	jmsg ["type"] = "request";
	string source;
	source = jmsg ["source"] = generateId ();
	forRequestTable.lock ();
		(*requestTable) [source] = conn;
	forRequestTable.unlock ();	
	target->SendMsg(jmsg.dump ()); // save path for tracing (if needed)
}			
void requestReact (MsgComm* conn, GateDict* connectors, GateDict* requestTable, json& jmsg) {
	
	if (jmsg.find ("target") == jmsg.end ()) {
		string command = jmsg ["command"];
		printf ("Command is inquiring: %s\n", command.c_str());
		forFunctionHub.lock ();
		auto it = functionHub.find (command);
		forFunctionHub.unlock ();
		
		if (it != functionHub.end ()) {
			printf ("[*] Command is requesting on 0x%x for informations\n", it->second);
			requestToTarget (conn, it->second, requestTable, jmsg);
		} else
			throw BadRequestException ("Unknown function", 1);
		return;
	}
	
	if (jmsg["target"] == "kernel") {
		// kernel handling request
		kernelHandling (conn, connectors, requestTable, jmsg);
	} else {
		// inquiring information by module name
		
		string target = jmsg["target"];
		jmsg.erase ("target");
		string source = jmsg["source"] = generateId ();
		
		forConnectors.lock ();
		auto it = connectors->find (target);
		forConnectors.unlock ();
		
		if (it != connectors->end ()) {
			requestToTarget (conn, it->second, requestTable, jmsg);	
		} else 
			throw BadRequestException ("Unknown target", 1);
			
	}	
}

void registerReact (MsgComm* conn, GateDict* connectors, GateDict* requestTable, json& jmsg) {
	string moduleName = jmsg ["module"];
	
	forConnectors.lock ();
	
		if (connectors->find (moduleName) != connectors->end ()) { // checking state, build a function to wrap the problem
			forConnectors.unlock ();
			throw BadRequestException ("Conflict module name. Name already existed", 2);
		}
		
		(*connectors)[moduleName] = conn;
	forConnectors.unlock ();
		
	vector<string> functions = jmsg ["function"];	
	vector<string> success;
	
	for (auto it: functions) printf ("Need register '%s' for 0x%x \n", it.c_str (), conn);
	
	for (auto name: functions) {
//		string name = each["name"];
		// TODO: At first, assume that all the functions are safe and mono
		forFunctionHub.lock ();
			functionHub [name] = conn;
		forFunctionHub.unlock ();
		
		success.push_back(name);
		
		printf ("Registered '%s' at module 0x%x, which named %s\n", name.c_str(), conn, moduleName.c_str());
	}
	
	json resp;
			resp ["sucess"] = "true";
			resp ["result"] = success;
	conn->SendMsg (resp.dump ());
}

void responseReact (MsgComm* conn, GateDict* connectors, GateDict* requestTable, json& jmsg) {
	string rqId = jmsg ["source"];
	jmsg.erase ("source");
	jmsg.erase ("type");
	
	auto it = requestTable->find (rqId); 
	if (it != requestTable->end ()) {
		printf ("Found a destination request id=%s\n", rqId.c_str());
		it->second->SendMsg (jmsg.dump ());
		forRequestTable.lock ();
		requestTable->erase (it);		
		forRequestTable.unlock ();
	} else {
		throw CodeException ("Request id not found", 2); // TODO: Build up error system for tracing and maintainance
	}
}

void respOnBad (MsgComm* conn, exception& e) {
	try {
		json resp;
			resp ["type"] = "response";
			resp ["success"] = "false";
			resp ["result"] = e.what ();
		conn->SendMsg (resp.dump ());
	} catch (CodeException& e) {
		// TODO: tell itself
	}
}

void handleMessages (MsgComm* conn, GateDict* connectors, GateDict* requestTable) {

	while (1) {
		try {
//			sleep (1);
			auto msg = conn->RecvMsg ();
			printf ("Msg via conn 0x%x: %s\n", conn, msg.c_str ());			 // debug purpose 
			auto jmsg = json::parse (msg);
				
			// register on messages
			// then on, process "jmsg"
			
			string type = jmsg ["type"];
			if (type == "request") {
				requestReact (conn, connectors, requestTable, jmsg);
			} else if (type == "response") {
				responseReact (conn, connectors, requestTable, jmsg);
			} else if (type == "register") {
				registerReact (conn, connectors, requestTable, jmsg);
			} else 
				throw BadRequestException ("Unknown type of message", 3);
												
		} catch (BadRequestException& e){
			printf ("Bad Request is thrown\n");
			respOnBad (conn, e);
		} catch (CodeException& e) { // or instance of message connection exception
			// TODO: when socket is disconnected	
			
			break;
		} catch (exception& e) { // json and another exception
			respOnBad (conn, e);
			break;
		}
	}
	
	forConnectors.lock ();
	conn->Close();
	// TODO: Find more efficent delete method, delete consume O(N)
	for (auto it=connectors->begin ();it != connectors->end (); ++it)
		if (it->second == conn) connectors->erase (it);
	for (auto it=functionHub.begin ();it != functionHub.end (); ++it)
		if (it->second == conn) functionHub.erase (it);
			
	for (auto it=requestTable->begin ();it != requestTable->end (); ++it)
		if (it->second == conn) requestTable->erase (it);
	printf ("conn: 0x%x closed\n", conn);
	delete conn;
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

void asyncAccept (MsgComm* host, GateDict* connectors, GateDict* requestTable) {
	
//	vector <thread*> thrdManager;
	while (1) {
		auto conn = host->Accept();
		printf ("A connection is accepted at 0x%x ...\n", conn);
		thread  aThread (handleMessages, conn, connectors, requestTable);
		aThread.detach ();
	}
}
void serverInitialize (MsgComm* host, int port, GateDict* connectors, GateDict* requestTable) {
	
	Socket::initialize();
	printf ("Binding %s on port %d\n", ini["host"].c_str(), port);
	host->Bind(ini["host"], port);
	printf ("Listening localhost on port %d\n", port);
	host->Listen ();
	
	thread acceptProgress (asyncAccept, host, connectors, requestTable);
	acceptProgress.join ();	
}

void serverFinalize () {
	
	// TODO: terminate acceptProgress thread
	Socket::finalize ();
}
int main (int n, char** argv){
	
	port = (int) toLL (ini.get("port", "8888"));
	serverInitialize (&host, port, &connectors, &requestTable);
	serverFinalize ();
}
