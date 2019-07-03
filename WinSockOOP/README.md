# Socket C++ OOP implementation

Compile with -fpermissive, -lWs2_32
Need "StringUtils.h" (provided)

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
