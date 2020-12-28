#include "projekat.h"
/*Argumenti za test primer: 3055122589 2895159338*/

int main( int argc, char **argv ) //kompajlovati sa: gcc projekat_main.c projekat.c projekat.h red1.h -o a.out -lm
{
    Graf *G = napravi_graf();
    int ind;

    printf( "Cime idete? ( 1 -> Kola ; 2 -> Peske ): " );
    scanf( "%d" , &ind );
    
    if( ind == 1 ) nadji_cilj_kola( G , argc , argv );
    else if( ind == 2 ) nadji_cilj_pesak( G , argc , argv );
    else printf( "Pogresan unos!\n" );
    
    obrisi_graf( G );
    
    return( 0 );
}
