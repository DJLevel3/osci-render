# VST3 Plugin Port

## Not working as osci-render at the moment, don't try to build rn

### Where the port is
The port is in the port/ORVST3/ directory, the ported files so far are:
- FFT.cpp & FFT.h
- UserMath.h (was the Math class)
- src/Shape.h
- src/shapes.cpp & src/shapes.h
- src/Vector2.cpp & src/Vector2.h

### To actually compile and stuff:
- Install OpenSSL
- Place the whole project (as in, the base GH repo) in the Examples folder of an iPlug2 installation
- Open ORVST3.sln (which is located in port/ORVST3/) with Visual Studio 2022
- Set the vst3 project as the startup project
- Tell Visual Studio where OpenSSL's lib/ and include/ folders are
- Wait for DJ_Level_3 to actually make it work
- `$ git pull`
- Run with or without debugging (it doesn't really matter)