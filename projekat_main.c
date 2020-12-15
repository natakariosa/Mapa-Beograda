#include "projekat.h"

int main( void )
{
    int dim;
    
    Graf **G = napravi_graf( G , &dim );
 
    stampaj( G , dim );
    
    obrisi_graf( G , dim );
    
    return( 0 );
}
