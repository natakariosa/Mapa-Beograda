#include <stdio.h>
#include <stdlib.h>

#include "grafBFSR.h"

int main( int argc, char **argv )
{
    Graf *G;
    int niz[argc - 1]; // lista povezanosti
    Brojac i;
    
    for( i = 0 ; i < argc - 1 ; i++ )
    {
        niz[i] = atoi( *( argv + i + 1 ) );
    };
    
    G = napravi_graf( niz, argc - 1 );
    stampaj_graf( G );
    
    printf( "Cvor od kojeg pocinje BFS pretraga\n[0] <= [ ] <= [%d]\n-> ", G->broj_cvorova - 1 );
    scanf( "%hu", &i );
    
    pomBFS( &G, i );
    
    obrisi_graf( G );
    
    return 0;
}
