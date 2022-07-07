# SDKiller

compile with ```g++ -O0 test.cpp -o test```

Add ```-D_POSIX_C_SOURCE=200112L``` if necessary.

fill the SD card using ```dd if=/dev/urandom bs=512k of=/dev/sdb conv=fsync oflag=sync```

then just leave it run over night:

```./test /dev/sdb```

