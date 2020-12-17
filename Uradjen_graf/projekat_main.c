#include "projekat.h"

int main( void ) //kompajlovati sa: gcc projekat_main.c projekat.c projekat.h -o a.out -lm
{
    Graf *G = napravi_graf();
   
    stampaj( G );
    
    return( 0 );
}
