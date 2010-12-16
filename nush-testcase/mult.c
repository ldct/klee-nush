#include "klee/klee.h"

int main ( int x0 , int y0 ) {

    unsigned int x, y;

    klee_make_symbolic(&(x), sizeof(x), "x0");
    klee_make_symbolic(&(y), sizeof(y), "y0");
    
    if (x > 512 || y > 512)
        return 0;
        
    int z = 0;
    
    while (x != 0) {
        if (x & 1)
            z += y ;
        x = x >> 1 ;
        y = y << 1 ;
    }
    return z ;
}

