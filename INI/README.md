# Simple INItializing file (configuration)

```C++11
#include <iostream.h>
#include "INI.cpp"

int main (){
  INI ini;
  ini.fromFile(".ini");
  ini.print ();

  cout << ini.get ("first_param", "");
}
```
