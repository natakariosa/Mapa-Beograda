#include "projekat.h"

FILE *otvori( char *ime , char *kako )
{
    FILE *d = fopen( ime , kako );
    
    if( d == NULL ) exit( EXIT_FAILURE );
    
    return( d );
}

Graf *napravi_element( int vrednost , double tezina , char *ime , int br_ulice )
{
    Graf *novi = ( Graf * ) malloc( sizeof( Graf ) );
    
    strcpy( novi->ime , ime );
    novi->br_ulice = br_ulice;
    
    novi->id = vrednost;
    novi->tezina = tezina;
    novi->put = INFINITY;
    novi->rastojanje = INFINITY;
    novi->sledeci = NULL;

    return( novi );
}

void dodaj_ivicu( Graf **glava , double vrednost , double tezina , char *ime ) //ovde je problem sa brojem ulice!
{
    Graf *pom = NULL;

    if( (*glava)->tezina == -1 )
    {
        if( strcmp( ime , "Nema naziv" ) == 0 ) *glava = napravi_element( vrednost, tezina , ime , 0 );
        else *glava = napravi_element( vrednost, tezina , ime , 1 );
        
        ( *( glava ) )->rastojanje = tezina;
        ( *( glava ) )->tezina = 0;
    }
    else
    {
        pom = *glava;
        
        while( pom->sledeci ) pom = pom->sledeci;
        
        if( strcmp( ime , "Nema naziv" ) == 0 ) pom->sledeci = napravi_element( vrednost, tezina , ime , 0 );
        else pom->sledeci = napravi_element( vrednost, tezina , ime , pom->br_ulice + 1 );
    };
}

char **napravi_niz( char **niz , int dim )
{
    int i = 0 , j;
    char c , ime[50];
    
    FILE *dat = otvori( "pevac.pypgr_names", "r" );
   
    niz = ( char ** ) malloc( sizeof( char * ) * dim );
    
    for( j = 0 ; j < dim ; j++ ) niz[j] = ( char * ) malloc( sizeof( char ) * 150 );
    
    j = 0;
    
    while( !feof( dat ) )
    {
        while( ( c = fgetc( dat ) ) != '\n' && c != EOF ) ime[j++] = c;
        
        if( c == EOF ) break;
        
        if( j > 0 ) ime[j] = '\0'; 
        
        if( j == 0 ) strcpy( niz[i] , "Nema naziv" );
        else strcpy( niz[i] , ime );
 
        j = 0;
        i++;
    };
    
    fclose( dat );
    
    return( niz );
}

Graf **napravi_graf( Graf **G , int *dim1 )
{
    char **niz = NULL , pom[50];

    int i , dim2 , pocetni , krajnji , jednosmerna , brzina;
    double duzina;

    FILE *dat1 = otvori( "pevac.pypgr" , "r" );

    fscanf( dat1, "%d", dim1 );
    fscanf( dat1, "%d", &dim2 );

    G = ( Graf ** ) malloc( *dim1 * sizeof( Graf * ) ); //Notacija od 0 jer nam je id od 0
    if( G == NULL )
    {
      printf( "Greska pri alokaciji memorije!\n" );
      exit( EXIT_FAILURE );
    };
    
    for( i = 0 ; i < *dim1 ; i++ )
    {
        G[i] = ( Graf * ) malloc( sizeof( Graf ) );
      
        fscanf( dat1 , "%d %lf %lf", &G[i]->id , &G[i]->latituda, &G[i]->longituda );
        
        G[i]->tezina = -1;
    };
    
    niz = napravi_niz( niz , dim2 );
    
    for( i = 0 ; i < dim2 ; i++ )
    {
        fscanf( dat1 , "%d %d %lf %s %d %d" , &pocetni , &krajnji , &duzina , pom , &brzina , &jednosmerna );
        
        dodaj_ivicu( &( G[pocetni] ) , krajnji , duzina , niz[i] );
            
        if( jednosmerna == 0 ) dodaj_ivicu( &( G[krajnji] ) , pocetni , duzina , niz[i] );
    };

    fclose( dat1 );
    
    for( i = 0 ; i < dim2 ; i++ ) free( niz[i] );
    
    free( niz );

    return( G );
}

void stampaj( Graf **G, Brojaci dim )
{
    Brojaci i;

    printf( "\n" );
    
    for( i = 0; i < dim ; i++ )
    {
        printf( "Lista povezanosti za cvor %d: \n" , i );
        
        while( *( G + i ) != NULL )
        {
            printf( "%d[%lf][%s %d] -> ", ( *( G + i ) )->id, ( *( G + i ) )->tezina , ( *( G + i ) )->ime , ( *( G + i ) )->br_ulice );
            *( G + i ) = (*( G + i ))->sledeci;
        };
        
        printf( "Kraj liste\n" );
    };
}

void obrisi_graf( Graf **G , int dim )
{
    int i;
    
    for( i = 0 ; i < dim ; i++ ) free( G[i] );
    
    free( G );
}