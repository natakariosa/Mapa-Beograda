#include "projekat.h"
#include<math.h>

FILE *otvori( char *ime , char *kako )
{
    FILE *d = fopen( ime , kako );
    
    if( d == NULL ) exit( EXIT_FAILURE );
    
    return( d );
}

Cvor *napravi_cvor( long id , double lat , double lon )
{
    Cvor *novi_el = ( Cvor * ) malloc( sizeof( Cvor ) );
    if( novi_el == NULL ) exit( EXIT_FAILURE );
    
    novi_el->id = id;
    novi_el->lat = lat;
    novi_el->lon = lon;
    novi_el->put = INFINITY;
    novi_el->tezina = 0;
    novi_el->rastojanje = INFINITY;
    novi_el->sledeci = NULL;
    
    return( novi_el );
}

Graf *inicijalizuj_graf( Graf *G , long dim )
{
    int i;
    
    G = ( Graf * ) malloc( sizeof( Graf ) );
    if( G == NULL ) exit( EXIT_FAILURE );
    
    G->dim = dim;
    G->putevi = ( Cvor ** ) malloc( sizeof( Cvor * ) * G->dim );
    
    for( i = 0 ; i < G->dim ; i++ ) G->putevi[i] = ( Cvor * ) malloc( sizeof( Cvor ) );
    
    return( G );
}

double haversine_formula( double lat1 , double lon1 , double lat2 , double lon2 ) 
{ 
    double dLat = (lat2 - lat1) * M_PI/180.0; 
    double dLon = (lon2 - lon1) * M_PI/180.0; 
    
    lat1 = (lat1) * M_PI/180.0; //radijani
    lat2 = (lat2) * M_PI/180.0; 
  
    double a = sin(dLat/2)*sin(dLat/2) + sin(dLon/2)*sin(dLon/2) * cos(lat1)*cos(lat2); //formula
    double rad = 6371; //polurecnik Zemlje u km
    double c = 2*asin(sqrt(a)); 
    
    return( 1000.0*rad*c ); //posto bi inace vratio u kilometrima 
}     

void dodaj_ivicu( Graf **G , long id_prvog , long id_drugog )
{
    char ind1 = 0 , ind2 = 0;
    long i , prvi , drugi;
    
    for( i = 0 ; i < (*G)->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
    {
        if( ind1 == 0 && (*G)->putevi[i]->id == id_prvog )
        {
            prvi = i;
            ind1 = 1;
        }
        else if( ind2 == 0 && (*G)->putevi[i]->id == id_drugog )
        {
            drugi = i;
            ind2 = 1;
        };
    };
    
    (*G)->putevi[prvi]->sledeci = (*G)->putevi[drugi];
    
    (*G)->putevi[drugi]->rastojanje = haversine_formula( (*G)->putevi[prvi]->lat , (*G)->putevi[prvi]->lon , (*G)->putevi[drugi]->lat , (*G)->putevi[drugi]->lon );
    
    if( (*G)->putevi[prvi]->tezina == 0 ) (*G)->putevi[prvi]->tezina = (*G)->putevi[drugi]->rastojanje;
}

void dodaj_ime( Graf **G , long id_cvora , char *ime_puta )
{
    char ind1 = 0;
    long i , prvi;
    
    for( i = 0 ; i < (*G)->dim , ind1 != 1 ; i++ ) 
    {
        if( ind1 == 0 && (*G)->putevi[i]->id == id_cvora )
        {
            prvi = i;
            ind1 = 1;
        };
    };
    
    strcpy( (*G)->putevi[prvi]->ime , ime_puta );
}

Graf *napravi_graf()
{
    Graf *G = NULL;
    FILE *dat1 = otvori( "pevac_putevi.txt", "r" ) , *dat2 = otvori( "pevac_cvorovi.txt", "r" );
    char c , ime[100] , pom[100] , *ime_puta;
    int i = 0 , j = 0 , k = 0 , m , ind = 0;
    long dim , id , id_cvora , id_puta , *niz_id_cvorova;
    double lat , lon;
    
    while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
    
    pom[j] = '\0';
    
    j = 0;
    
    dim = atof( pom );
 
    G = inicijalizuj_graf( G , dim );
    
    while( !feof( dat2 ) )
    {
        fscanf( dat2 , "%ld %lf %lf" , &id , &lat , &lon );
        
        G->putevi[i++] = napravi_cvor( id , lat , lon );
    };
    
    i = 0; //DOVDE JE DOBRO SIGURNO
    
    while( !feof( dat1 ) )
    {
        while( ( c = fgetc( dat1 ) ) != ' ' && c != EOF ) ime[i++] = c;
        
        if( c == EOF ) break;
        
        ime[i] = '\0'; 
        
        if( !strcmp( ime , "way" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            id_puta = atof( pom );
            
            if( k != 0 ) 
            {
                for( m = 0 ; m < k -1 ; m++ ) dodaj_ivicu( &G , niz_id_cvorova[m] , niz_id_cvorova[m+1] );
            
                if( ind == 0 ) for( m = k - 1 ; m > 0 ; m-- ) dodaj_ivicu( &G , niz_id_cvorova[m] , niz_id_cvorova[m-1] );
            
                k = 0;
            
//                 free( niz_id_cvorova );
            };
//             printf( "Way>>%ld\n" , id_puta );
        }
        else if( !strcmp( ime , "id" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            id_cvora = atof( pom );
            
            niz_id_cvorova = ( long * ) realloc( niz_id_cvorova , sizeof( long ) * ( k + 1 ) );
            
            niz_id_cvorova[k++] = id_cvora;
            
//             printf( "Cvor>>%ld\n" , id_cvora );
        }
        else if( !strcmp( ime , "oneway" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            if( !strcmp( pom , "yes" ) ) ind = 1;
            else ind = 0;
        }
        else if( !strcmp( ime , "operator" ) || !strcmp( ime , "name" ) )
        {
            ime_puta = ( char * ) malloc( sizeof( char ) * 100 );
            
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            strcpy( ime_puta , pom );
            
            for( m = 0 ; m < k ; m++ ) dodaj_ime( &G , niz_id_cvorova[m] , ime_puta );
            
//             printf( "Ime>>%s\n" , pom );
            free( ime_puta );
        };
    
        i = 0;
        j = 0;
    };
    
    
    
    fclose( dat1 );
    fclose( dat2 );
    
    return( G );
}
