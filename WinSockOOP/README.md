# Socket C++ OOP implementation

Mandatory files: socket.h, socket.cpp, MsgComm.cpp, StringUtils.h, INI.cpp in INI folder

Compile with -fpermissive, -lWs2_32

------- Server side ---------
```C++
int main () {
  Socket::initialize ();
  Socket sock;
  sock.Bind ('localhost', 8888);
  sock.Listen ()
  while (true){
    auto conn = sock.Accept ();
    // Process connections
      conn->Send ("Hello World", 11)    // 11 is buffer - length
   }

  sock.Close ();
  Socket::finalize ();
}
```

------- Client size ---------

```C++
Socket::initialize ();
Socket sock;
sock.Connect ('localhost', 8888);
char buf[255];
sock.Recieve (buf, 255);
printf ("%s", buf);

sock.Close ();
Socket::finalize ();

```

Rest of the documentation will be upload later
