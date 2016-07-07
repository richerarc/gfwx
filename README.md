# Compiling GFWX with g++ on ubuntu

### 1) Create the static lib

```
cd libgfwx

g++ -std=c++11 -c gfwx.cpp

ar rvs gfwx.a gfwx.o

```

### 2) Use gfwx

```
// Random .cpp file

#include "libgfwx/gfwx.hpp"

```

### 3) Link the library

```
g++ -std=c++11 -o exec random.cpp libgfwx/gfwx.a
```