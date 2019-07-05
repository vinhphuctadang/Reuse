# Simple INItializing file (configuration)

```C++11
INI ini;
ini.fromFile(".ini");
ini.print ();

cout << ini.get ("first_param", "");
```
