#GFWX

gfwx is for goof fast wavelet codec. This is a implementation in C based on the code found at gfwx.org 

### IMPORTANT NOTE : WIP


# Cmake build and test

```
git clone git@github.com:richerarc/gfwx

cd gfwx

cmake .

make

cd test/compresion

./gfwx-compress <input-img> <output data> <img-decoded>

```

# Compiling GFWX with g++ without CMake

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
