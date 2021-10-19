### BUILD

* Just run `make` to build.

### SIMPLE INSTALL

* Run `make install` 
* It will copy `gdbout_api.h`to `/usr/include`

### INSTALL ALL

* Run `sudo make install_all` 
* It will copy `gdbout_api.h`to `/usr/include`, `rdircgdberr`, `rdircgdbout`, `serverrun.sh` to `/usr/bin/`

### HOW TO USE
* If you run you program with this header file, you program will try to connect to serverrun.sh, or will not.
* Just `#include <gdbout_api.h>`, and simply call`clientInit(STDOUT_FILENO, STDERR_FILENO);` or `clientAutoInit();` at begin of function`main`. Run `serverrun.sh` before calling `clientInit();`

### There is a demo called `test.c`



