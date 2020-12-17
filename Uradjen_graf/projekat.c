#include "projekat.h"
#include<math.h>

FILE *otvori( char *ime , char *kako )
{
    FILE *d = fopen( ime , kako );
    
    if( d == NULL ) exit( EXIT_FAILURE );
    
    return( d );
}

Cvor *napravi_cvor( long id , double lat , double lon , double tezina , char *ime )
{
    Cvor *novi_el = ( Cvor * ) malloc( sizeof( Cvor ) );
    if( novi_el == NULL ) exit( EXIT_FAILURE );
    
    novi_el->id = id;
    novi_el->lat = lat;
    novi_el->lon = lon;
    novi_el->tezina = tezina;
    novi_el->put = INFINITY;
    novi_el->rastojanje = INFINITY;
    novi_el->sledeci = NULL;
    
    strcpy( novi_el->ime , ime );
    
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

void dodaj_ivicu( Cvor **A , Cvor *B , char *ime )
{
    Cvor *pom = NULL;
    double tezina = haversine_formula( (*A)->lat , (*A)->lon , B->lat , B->lon );
    
    if( *A == NULL ) *A = napravi_cvor( B->id , B->lat , B->lon , tezina , ime );
    else
    {
        pom = *A;
        
        while( pom->sledeci ) pom = pom->sledeci;
        
        pom->sledeci = napravi_cvor( B->id , B->lat , B->lon , tezina , ime ); 
    };
}

Graf *napravi_graf()
{
    Graf *G = NULL;
    Cvor *pomocni = NULL;
    FILE *dat1 = otvori( "pevac_putevi.txt", "r" ) , *dat2 = otvori( "pevac_cvorovi.txt", "r" );
    char c , ind1 = 0 , ind2 = 0 , ime[100] , pom[100] , *ime_puta = NULL;
    int i = 0 , j = 0 , k = 0 , m , ind = 0;
    long dim , id , prvi , drugi , id_cvora , id_puta , *niz_id_cvorova = NULL;
    double lat , lon;
    
    while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
    
    pom[j] = '\0';
    
    j = 0;
    
    dim = atof( pom );
 
    G = inicijalizuj_graf( G , dim );
    
    while( !feof( dat2 ) )
    {
        fscanf( dat2 , "%ld %lf %lf" , &id , &lat , &lon );
        
        G->putevi[i++] = napravi_cvor( id , lat , lon , 0 , "Nema" );
    };
    
    i = 0;
    
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
                if( ime_puta )
                {
                    for( m = 0 ; m < k -1 ; m++ )
                    {
                        for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                        {
                            if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                            {
                                prvi = i;
                                ind1 = 1;
                            }
                            else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m+1] )
                            {
                                drugi = i;
                                ind2 = 1;
                            };
                        };

                        dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , ime_puta );
                        
                        ind1 = 0;
                        ind2 = 0;
                    };
                }
                else 
                {
                    for( m = 0 ; m < k -1 ; m++ )
                    {
                        for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                        {
                            if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                            {
                                prvi = i;
                                ind1 = 1;
                            }
                            else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m+1] )
                            {
                                drugi = i;
                                ind2 = 1;
                            };
                        };

                        dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , "Nema" );
                        
                        ind1 = 0;
                        ind2 = 0;
                    };
                };
            
                if( ind == 0 )
                {
                    if( ime_puta )
                    {
                        for( m = k - 1 ; m > 0 ; m-- )
                        {
                            for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                            {
                                if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                                {
                                    prvi = i;
                                    ind1 = 1;
                                }
                                else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m-1] )
                                {
                                    drugi = i;
                                    ind2 = 1;
                                };
                            };

                            dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , ime_puta );
                        
                            ind1 = 0;
                            ind2 = 0;
                        };
                    }
                    else
                    {
                        for( m = k - 1 ; m > 0 ; m-- )
                        {
                            for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                            {
                                if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                                {
                                    prvi = i;
                                    ind1 = 1;
                                }
                                else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m-1] )
                                {
                                    drugi = i;
                                    ind2 = 1;
                                };
                            };
                            
                            dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , "Nema" );
                        
                            ind1 = 0;
                            ind2 = 0;
                        };
                    };
                };
                
                if( ime_puta ) ime_puta = NULL;

                k = 0;
            };
        }
        else if( !strcmp( ime , "id" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            id_cvora = atof( pom );
            
            niz_id_cvorova = ( long * ) realloc( niz_id_cvorova , sizeof( long ) * ( k + 1 ) );
            
            niz_id_cvorova[k++] = id_cvora;
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
        };
    
        i = 0;
        j = 0;
    };
    
    fclose( dat1 );
    fclose( dat2 );
    
    for( i = 0 ; i < G->dim ; i++ )  //ova for petlja je da bi se naziv ulice nalazio na oba cvora (A,B), ako se ukloni, naziv ulice ce se nalaziti samo na drugom (B) cvoru. Cvor = A->B
    {
        if( !strcmp( G->putevi[i]->ime , "Nema" ) )
        {
            pomocni = G->putevi[i];
            
            if( pomocni->sledeci )
            {
                if( strcmp( pomocni->sledeci->ime , "Nema" ) ) strcpy( G->putevi[i]->ime , pomocni->sledeci->ime );
            };
        };
    };
    
    return( G );
}

void stampaj( Graf *G )
{
    int i;
    
    for( i = 0 ; i < G->dim ; i++ )
    {
        while( G->putevi[i] )  //ovo if( !strcmp( G->putevi[i]->ime , "Милутина Гарашанина" ) ) je samo primer, da se vidi da radi
        {
            if( !strcmp( G->putevi[i]->ime , "Милутина Гарашанина" ) ) printf( "%ld %lf %lf %lf %s\n", G->putevi[i]->id , G->putevi[i]->lat , G->putevi[i]->lon , G->putevi[i]->tezina , G->putevi[i]->ime );
            
            G->putevi[i] = G->putevi[i]->sledeci;
        };
    };
}