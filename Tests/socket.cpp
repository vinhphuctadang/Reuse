
void Socket::initialize() {
	auto iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
		throw CodeException ("Windows socket initialization failed", iResult);
}

void Socket::finalize () {
	WSACleanup();
}

//static string GetHostName (int bufferSize=255) { // get Host name by given IP
//	
//}

Socket::Socket(int family=AF_INET, int type=SOCK_STREAM, int proto=0, int fileno=AI_PASSIVE) { // init a socket, almost use default type=> TCP/IP
	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;
    hints.ai_flags = fileno;    
    ListenSocket = INVALID_SOCKET; // listen socket is now ready
}

Socket::Socket(SOCKET socket) { // Socket create via given fd, for technical use only
	addrinfo *result = NULL;
    ZeroMemory(&hints, sizeof(hints));
	ListenSocket = socket;
}

void Socket::Bind (string addr, int port) {
	
	auto iResult = getaddrinfo(addr.c_str(), toString ((long long)port).c_str (), &hints, &result);
    if (iResult != 0 ) {
//    	WSACleanup();
    	throw CodeException ("Getaddrinfo failed failed", iResult);
    }
 
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
    	int lastErr = WSAGetLastError();
    	freeaddrinfo(result);
//    	WSACleanup();
        throw CodeException ("WinSocket failed retrieving listen socket", lastErr);
    }
    
	long argp = 1;
    // attempt to setup the socket as non-blocking
    //  thanks to author of thread: https://www.reddit.com/r/learnprogramming/comments/1jjbt5/nonblocking_tcpip_with_winsock_example/
    
    if (ioctlsocket(ListenSocket, FIONBIO, &argp) == SOCKET_ERROR) { 
    	// See MS doc at the link below
    	// https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket
        throw CodeException ("Socket setting non blocking", WSAGetLastError());
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
//        WSACleanup();
        throw CodeException ("Bind failed", WSAGetLastError());
    }
        
    freeaddrinfo(result);
}
void Socket::Listen (int number=SOMAXCONN) {
	
	auto iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        closesocket(ListenSocket);
//        WSACleanup();
		throw CodeException ("Listen failed", WSAGetLastError());
//        return 1;
    }
}


Socket* Socket::Accept() {
	auto ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
    	int lastErr = WSAGetLastError();
    	if (lastErr == WSAEWOULDBLOCK)
    		return NULL;
    		
    	closesocket(ListenSocket);
        throw CodeException ("Accept failed", lastErr);
    }
    
    return new Socket (ClientSocket);
}

void Socket::Send(char* buffer, int bufferSize) {
	
	auto iSendResult = send(ListenSocket, buffer, bufferSize, 0);
	
    if (iSendResult == SOCKET_ERROR) {
    	int lastErr = WSAGetLastError();    	
    	closesocket(ListenSocket);
//    	WSACleanup();
        throw CodeException ("Send failed", lastErr);
    }
}
int Socket::Receive(char* buffer, int bufferSize=1024, ull timeOut = 0) { // -1 stands for permanent waiting
//	char buf[bufferSize]; // TODO: Edit
	if (bufferSize <= 0)
		return 0;
		
	int iResult = 0;
	auto marked = now_micro ();
	int lastErr = 0;
	do {
		iResult = recv(ListenSocket, buffer, bufferSize, 0);
		if (iResult < 0) {
			lastErr = WSAGetLastError();
			if (lastErr == WSAEWOULDBLOCK) 
				continue;
			throw CodeException ("Recieve error", lastErr);
		}
		break;
	} while (timeOut == 0 || now_micro () - marked <= timeOut);
	
	if (iResult < 0) 
		throw CodeException ("Recieve error: Time out", lastErr);
	
	if (iResult == 0) 
		throw CodeException ("Recieve terminated while socket is closing", -1);
	return iResult;
}


void Socket::Close() {
	if (closesocket(ListenSocket) == SOCKET_ERROR) {
		throw CodeException ("Socket closing error", WSAGetLastError());
	}
}

int Socket::Connect(string addr, int port, ull timeOut = 0) {
	
	// Resolve the server address and port
    auto iResult = getaddrinfo(addr.c_str(), toString ((long long)port).c_str (), &hints, &result);
    if ( iResult != 0 ) {
//        WSACleanup();
        throw CodeException ("Getaddrinfo failed failed", iResult);
    }

    // Attempt to connect to an address until one succeeds
    auto ConnectSocket = INVALID_SOCKET;
    
	long long countAttempts = 0;
	int lastErr = 0;
	
	auto marked = now_micro ();
	int sockWaiting = 0;
	do {
		sockWaiting = 0;
	    for(auto ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
	    	countAttempts ++;   
	        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
	            ptr->ai_protocol);
				    
	        if (ConnectSocket == INVALID_SOCKET) 
	            throw CodeException ("Socket retrieve failed", WSAGetLastError());
	            
		//  thanks to author of thread: https://www.reddit.com/r/learnprogramming/comments/1jjbt5/nonblocking_tcpip_with_winsock_example/
			long argp = 1;
		    // attempt to setup the socket as non-blocking
		    if (ioctlsocket(ConnectSocket, FIONBIO, &argp) == SOCKET_ERROR) { 
		    	// See MS doc at the link below
		    	// https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket
		        throw CodeException ("Socket setting non blocking failed", WSAGetLastError());
		    }

	        // Connect to server.
	        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	        printf ("ATTEMPING %d\n", iResult);
	        if (iResult == SOCKET_ERROR) {
	        	
	        	lastErr = WSAGetLastError();     	
	        	closesocket(ConnectSocket);
	        	ConnectSocket = INVALID_SOCKET;
	        	
	        	if (lastErr == WSAEWOULDBLOCK) {
	    			sockWaiting ++; // some socket is waiting    		
				}
	        	continue;
	    	}
	    	break;
	    }
	    
	    if (iResult != SOCKET_ERROR) 
			break;
    } while (sockWaiting && (timeOut == 0 || now_micro() - marked <= timeOut));
    freeaddrinfo(result);
    
    if (timeOut && now_micro() - marked > timeOut)
    	throw CodeException (string ("Connect time out. Attempt counts: ") + toString (countAttempts), 
			5);
    if (ConnectSocket == INVALID_SOCKET) 
        throw CodeException (string ("Socket can not create connection. Attempt counts: ") + toString (countAttempts), 
			lastErr);
	ListenSocket = ConnectSocket;   
}

void Socket::lock () {
	secure.lock ();	
}

void Socket::unlock () {
	secure.unlock ();
}
