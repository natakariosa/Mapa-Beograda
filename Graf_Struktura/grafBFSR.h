#include <stdio.h>
#include <stdlib.h>

#include "tip.h"

Cvor *napravi_cvor( int vrednost )
{
    Cvor *novi = ( Cvor * ) malloc( sizeof( Cvor ) );
    if( novi == NULL )
    {
        printf( "Greska pri alokaciji memorije\n" );
        exit( EXIT_FAILURE );
    };
    
    novi->vrednost = vrednost;
    novi->sledeci = NULL;
    
    return novi;
}

Graf *napravi_graf( int *niz, int dimniz )
{
    Graf *G = NULL;
    Cvor *pom;
    Brojac dim, dodaj = 0, i = 0;
    
    G = ( Graf * ) malloc( sizeof( Graf ) );
    if( G == NULL )
    {
        printf( "Greska pri alokaciji memorije\n" );
        exit( EXIT_FAILURE );
    };
    
    while( niz[i + 1] > niz[i] ) i++;
    G->broj_cvorova = i + 1;
    
    G->lista_povezanosti = ( Cvor ** ) malloc( sizeof( Cvor * ) * G->broj_cvorova );
    if( G->lista_povezanosti == NULL )
    {
        printf( "Greska pri alokaciji memorije\n" );
        exit( EXIT_FAILURE );
    };
    
    G->poseceni = ( BYTE * ) calloc( G->broj_cvorova, sizeof( BYTE ) );
    if( G->poseceni == NULL )
    {
        printf( "Greska pri alokaciji memorije\n" );
        exit( EXIT_FAILURE );
    };
    
    for( i = 0 ; i < G->broj_cvorova ; i++ )
    {
        dim = ( i == ( G->broj_cvorova - 1 ) ) ? dimniz - niz[i] : niz[i + 1] - niz[i];
        
        while( dim-- )
        {
            pom = napravi_cvor( niz[( G->broj_cvorova ) + dodaj++] );
            
            pom->sledeci = G->lista_povezanosti[i];
            G->lista_povezanosti[i] = pom;
        };
    };
    
    return G;
}

void stampaj_graf( Graf *G )
{
    Cvor *pom;
    Brojac i;
    
    for( i = 0 ; i < G->broj_cvorova ; i++ )
    {
        printf( "[%hu] ", i );
        
        pom = G->lista_povezanosti[i];
        
        while( pom != NULL )
        {
            printf( " %d ->", pom->vrednost );
            pom = pom->sledeci;
        };
        printf( "\n" );
    };
}

void dodaj_u_listu( Cvor **glava, int vrednost )
{
    Cvor *novi = napravi_cvor( vrednost );
    
    novi->sledeci = *glava;
    *glava = novi;
}

void obrisi_listu( Cvor **glava )
{
    Cvor *pom = *glava;
    
    while( pom )
    {
        *glava = pom;
        pom = pom->sledeci;
        
        free( *glava );
    };
}

void BFS( Graf **G, int vrednost )
{
    Cvor *lista_povezanosti = ( *G )->lista_povezanosti[vrednost];
    Cvor *pom = lista_povezanosti;
    Cvor *glava = NULL;
    
    while( pom )
    {
        if( !( *G )->poseceni[pom->vrednost] )
        {
            printf( "[%d]", pom->vrednost );
            ( *G )->poseceni[pom->vrednost] = 1;
            
            dodaj_u_listu( &glava, pom->vrednost );
        };
        
        pom = pom->sledeci;
    };
    
    pom = glava;
    
    while( pom )
    {
        BFS( G, pom->vrednost );
        
        pom = pom->sledeci;
    };
    
    obrisi_listu( &glava );
}

void pomBFS( Graf **G, int vrednost )
{
    printf( "[%d]", vrednost );
    ( *G )->poseceni[vrednost] = 1;
    
    BFS( G, vrednost );
    printf( "\n" );
}

void obrisi_graf( Graf *G )
{
    Cvor *pom;
    Brojac i;
    
    for( i = 0 ; i < G->broj_cvorova ; i++ )
    {
        pom = G->lista_povezanosti[i];
        
        while( pom )
        {
            G->lista_povezanosti[i] = pom;
            pom = pom->sledeci;
            
            free( G->lista_povezanosti[i] );
        };
    };
    
    free( G->lista_povezanosti );
    free( G->poseceni );
    free( G );
}
