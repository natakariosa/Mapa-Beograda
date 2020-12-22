#include "projekat.h"

int main( int argc, char **argv ) //kompajlovati sa: gcc projekat_main.c projekat.c projekat.h -o a.out -lm
{
    Graf *G = napravi_graf();

    nadji_cilj( G, argc, argv );
   
    //stampaj( G );
    
    return( 0 );
}
