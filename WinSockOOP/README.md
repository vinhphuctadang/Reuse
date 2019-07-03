Socket C++ OOP implementation

Compile with -fpermissive, -lWs2_32
Need "StringUtils.h" (provided)

------- Server side ---------
'''C++
int main () {
  Socket::initialize ();
  Socket sock;
  sock.bind ('localhost', 8888);
  sock.listen ()
  while (true){
    auto conn = sock.accept ();
    // Process connections
      conn->Send ("Hello World", 11)    // 11 is buffer - length
   }

  sock.close ();
  Socket::finalize ();
}
'''

------- Client size ---------

'''C++
Socket::initialize ();
Socket sock;
sock.connect ('localhost', 8888);
char buf[255];
sock.Recieve (buf, 255);
printf ("%s", buf);

sock.close ();
Socket::finalize ();

'''
