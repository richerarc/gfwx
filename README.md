# Compiling GFWX with g++ on ubuntu

## 1) Create the static lib

```
cd libgfwx

g++ -std=c++11 -c gfwx.cpp

ar rvs gfwx.a gfwx.o

```

## 2) Use gfwx

```
// Random .cpp file

#include "libgfwx/gfwx.hpp"

```

## 3) Use the library with g++

```
g++ -std=c++11 -o exec random.cpp libgfwx/gfwx.a
```

### 3.1) Build with OpenCV

```
g++ -std=c++11 -o test main.cpp libgfwx/gfwx.a -I `pkg-config --libs opencv`
```

### 3.1) Build with OpenMP

```
g++ -std=c++11 -o test main.cpp libgfwx/gfwx.a -fopenmp
```