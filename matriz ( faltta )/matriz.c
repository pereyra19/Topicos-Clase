#include<stdio.h>


#define FILAS 4
#define COLUMNAS 4
#define ORDEN 4

/*
void cargarMatriz (size_t filas, size_t columnas, int mat[][COLUMNAS] );
void mostrarMatriz (size_t filas, size_t columnas, int mat[][COLUMNAS] );
int sumaDiagonalPrincipal(size_t orden, int mat[][orden]);
int sumaDiagonalSecundaria(size_t orden, int mat[][orden]);
int sumaTriangularInfDP(size_t orden, int mat[][orden]);
int sumaTriangularSupDS(size_t orden, int mat[][orden]);
*/

void **crearMatriz(size_t filas, size_t columnas, size_t tamElem);
void destruirMatriz (void **mat, size_t filas);
void cargarMatriz (int **mat, size_t filas, size_t columnas );
void mostrarMatriz (int **mat, size_t filas, size_t columnas);
int sumaDiagonalPrincipal(int ** mat, size_t orden);
int sumaDiagonalSecundaria(int ** mat, size_t orden);
int sumaTriangularInfDP(int ** mat, size_t orden);
int sumaTriangularSupDS(int ** mat, size_t orden);

int main ()
{
/*
    int matriz[FILAS][COLUMNAS] =
    {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,16}
    };
*/

    int orden = ORDEN;
/*
    int(*matriz)[COLUMNAS];
    matriz = malloc (orden * orden * sizeof(int));
*/
    
    int **matriz = (int **)crearMatriz(orden, orden, sizeof(int));

    if (!matriz)
    {
        puts("no hay memoria ");
        return 1;
    }

    cargarMatriz(matriz, orden, orden);
    mostrarMatriz(matriz, orden, orden);

    printf ("Suma diag princ: %d\n",sumaDiagonalPrincipal(ORDEN, matriz));
    printf ("Suma diag Secun: %d\n",sumaDiagonalPrincipal(ORDEN, matriz));
    printf ("suma triang inf DP: %d\n", sumaTriangularInfDP(ORDEN, matriz));


    destruirMatriz((void **)matriz, orden);
    //free(matriz);

    return 0;
}

void mostrarMatriz (int **mat, size_t filas, size_t columnas)
{
    for(int i=0; i<filas; i++)
    {
        for(int j=0; j<COLUMNAS ; j++)  // posV = i * cantCOL + j  calculo interno 
        {
            printf("%4d", mat[i][j]);
        }

        putchar('\n');
    }
}



int sumaDiagonalPrincipal(int ** mat, size_t orden)
{

    int suma =0;
    for( int i=0; i<orden; i++)
    {
        suma += mat [i][i];
    }

    return suma;
}


int sumaDiagonalSecundaria(int ** mat, size_t orden)
{
    int suma =0;

    for( int i=0, j=orden-1; i<orden; i++, j--)
    {
        suma += mat [i][j];
    }

    return suma;

}

int sumaTriangularInfDP(int ** mat, size_t orden)
{
    int suma =0;

    for(int i=1; i<orden; i++)
    {
        for (int j=0; j<i ; j++)
        {
            suma += mat[i][j];
        }
    }

    return suma;
}

int sumaTriangularSupDS(int ** mat, size_t orden)
{
    int suma =0;
    int limI = orden -1;

    for( int i=0, limj = limI ; i <= limI; limj--)
    {
        for(int j=0; j<= limj ; j++)
        {
            suma += mat[i][j];
        }
    }
    return suma;

}
void cargarMatriz (int **mat, size_t filas, size_t columnas )
{
    int cont =1;

    for(int i=0; i<filas; i++)
    {
        for(int j=0; j<COLUMNAS ; j++) 
        {
            mat[i][j] = cont++;
        }
    }

}

void ** crearMatriz(size_t filas, size_t columnas, size_t tamElem)
{
    void **mat = malloc (filas * sizeof(void*));

    if(!mat)
    {
        pust("no hay memoria");
        return  NULL;
    }

    void **ult = mat + filas-1;

    for (void ** i= mat; i<= ult; i++)
    {
        *i = malloc( columnas * tamElem);

        if(*i == NULL)
        {
            destruirMatriz(mat, i - mat); // la distancia entre punteros i - mat 
            return NULL;
        }
    }
}

void destruirMatriz (void **mat, size_t filas)
{
    void **ult = mat + filas-1;

    for (void ** i= mat; i<= ult; i++)
    {
        free(*i);
    } 

    free(mat);
}