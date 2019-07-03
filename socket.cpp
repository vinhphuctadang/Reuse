
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
    	
    	freeaddrinfo(result);
//    	WSACleanup();
        throw CodeException ("WinSocket failed retrieving listen socket", WSAGetLastError());
    }
    
    // Setup the TCP listening socket
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
    	closesocket(ListenSocket);
//        WSACleanup();
        throw CodeException ("Accept failed", WSAGetLastError());
//        return 1;
    }
    
    return new Socket (ClientSocket);
}

void Socket::Send(char* buffer, int bufferSize) {
	
	auto iSendResult = send(ListenSocket, buffer, bufferSize, 0);
	
    if (iSendResult == SOCKET_ERROR) {
    	closesocket(ListenSocket);
//    	WSACleanup();
        throw CodeException ("Send failed", WSAGetLastError());
    }
}
int Socket::Receive(char* buffer, int bufferSize=1024) {
//	char buf[bufferSize]; // TODO: Edit
	
	auto iResult = recv(ListenSocket, buffer, bufferSize, 0);
	if (iResult < 0) {
		throw CodeException ("Recieve error", WSAGetLastError());
	}
	return iResult;
}


void Socket::Close() {
	if (closesocket(ListenSocket) == SOCKET_ERROR) {
		throw CodeException ("Recieve error", WSAGetLastError());
	}
}

int Socket::Connect(string addr, int port) {
	
	// Resolve the server address and port
    auto iResult = getaddrinfo(addr.c_str(), toString ((long long)port).c_str (), &hints, &result);
    if ( iResult != 0 ) {
//        WSACleanup();
        throw CodeException ("Getaddrinfo failed failed", iResult);
    }

    // Attempt to connect to an address until one succeeds
    auto ConnectSocket = INVALID_SOCKET;
    
	long long countAttempts = 0;
    for(auto ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
    	
    	countAttempts ++;
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            throw CodeException ("Socket retrieve failed", WSAGetLastError());
        }
        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        throw CodeException (string ("Socket can not create connection. Attempt counts: ") + toString (countAttempts), 
			WSAGetLastError());
    }
    
    ListenSocket = ConnectSocket;
}
