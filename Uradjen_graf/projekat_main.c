#include "projekat.h"
/*Argumenti za test primer: 3055122589 2895159338*/

int main( int argc, char **argv ) //kompajlovati sa: gcc projekat_main.c projekat.c projekat.h -o a.out -lm
{
    Graf *G = napravi_graf();

    nadji_cilj( G, argc, argv );
   
    //stampaj( G );
    
    return( 0 );
}
