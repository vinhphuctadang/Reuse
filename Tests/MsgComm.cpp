#include "socket.h"
#include <algorithm> 
#define MSG_HEADER_TYPE unsigned int
#define MSG_HEADER_SIZE sizeof (MSG_HEADER_TYPE)
//
class MsgCommException : public CodeException {
	public:
	
	MsgCommException (const char* __inf, int errCode) : CodeException (__inf, errCode) {}
	const char * what () const throw () {
    	string result = string ("Message Communication exception (src: Socket). ") + CodeException :: what ();
		return result.c_str();
    }
//};
};


class MsgComm: public Socket {
	
	public:
		void SendMsg (const string& msg);
		string RecvMsg (int chunk=1024, unsigned int threshold = 1000000000);
		MsgComm* Accept ();

	private:
		void sendHeader (MSG_HEADER_TYPE header);
};


void MsgComm :: SendMsg (const string& msg){ 
	sendHeader (msg.size());
	Socket::Send(msg.c_str(), (int) msg.length());	
}

void MsgComm :: sendHeader (MSG_HEADER_TYPE header) {
	Socket::Send ((char*) &header, MSG_HEADER_SIZE);	
}

string MsgComm :: RecvMsg (int chunk=1024, unsigned int threshold = 1000000000) {
	
	unsigned int msgSize = 0;
	string result = "";
	
	int __len = Socket::Receive((char*) &msgSize, MSG_HEADER_SIZE); // Read Message Header first, then decide whether to serve or not; overflow hindered
	
	if (msgSize > threshold)
		throw MsgCommException ("Overflow size", -1);
	
	char* buffer = (char*) malloc (chunk);
	
	while (msgSize) {
		unsigned int recvSize = min (msgSize, (unsigned int) chunk);
//		printf ("Read needs: %u\t", recvSize);
		Socket::Receive	(buffer, recvSize);
		result.append(buffer, recvSize);
		msgSize -= recvSize;
	}
	
	free (buffer);
	
	return result;
}

MsgComm* MsgComm::Accept () {
	return (MsgComm*) Socket::Accept();
}
