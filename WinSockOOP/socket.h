#define WIN32_LEAN_AND_MEAN
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include "StringUtils.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
using namespace std;

class CodeException : public exception {
	private:
		string information;
		int errCode; 
	public:
		CodeException (string __information, int __errCode) {
			information = __information;
			errCode = __errCode;
		}
		
		string getInformation () {
			return information;
		}
		
		int getErrCode () {
			return errCode;
		}
		
	    const char * what () const throw () {
	    	string result = information + ".Code=" + toString ((unsigned long long) errCode);
			return result.c_str();
	    }
};
 
class Socket {
	private:
		
		addrinfo *result = NULL;
    	addrinfo hints;	
    	
    	SOCKET ListenSocket = INVALID_SOCKET;
    	
    	mutex secure;
    	
	public:
		static void initialize ();
		static void finalize ();
//		static string GetHostName (int bufferSize=255);
		
        Socket(int family=AF_INET, int type=SOCK_STREAM, int proto=0, int fileno=AI_PASSIVE);
        Socket(SOCKET socket);
//        Socket(SOCKET socket);
//        Socket();
        
        void Bind (string addr, int port);
        void Listen (int number=SOMAXCONN);
		Socket* Accept ();
		void Close();
        int Connect(string addr, int port);
        
        
        void lock ();
        void unlock ();
        
        void Send(char* buffer, int bufferSize);
//        void send(std::istream &stream);
        int Receive(char* buffer, int bufferSize=1024);
//        operator Socket& =(Socket& socket);
    };
WSAData wsaData;
#include "socket.cpp"
