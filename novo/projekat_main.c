#include "projekat.h"
/*Argumenti za test primer: 3055122589 2895159338*/

int main( int argc, char **argv ) //kompajlovati sa: gcc projekat_main.c projekat.c projekat.h -o a.out -lm
{
    Graf *G = napravi_graf();
    unsigned char ind;

    printf( "Kako zelite da idete:\n0 -> kolima\n1 -> peske\n" );
    scanf( "%d", &ind );

    if( ind )
    {
      nadji_cilj_pesak( G, argc, argv );
    }
    else
    {
      nadji_cilj_kola( G, argc, argv );
    }

    //stampaj( G );
    return( 0 );
}
