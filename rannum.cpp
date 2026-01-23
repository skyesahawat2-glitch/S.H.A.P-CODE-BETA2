#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main ()
{
    srand ( time( NULL ) );
    printf( "%d\n", (rand() % 10 + 5 ) );
    //printf( "%d\n", time (NULL) );
    //1768299367
}
