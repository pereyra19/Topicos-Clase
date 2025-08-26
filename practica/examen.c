#include "examen.h"




// libreria string.h
char *mi_strcpy(char *s1, const char*s2)
{
    char *act = s1;

    while(*s2 != '\0')
    {
        *act = *s2;
        act ++;
        s2++;
    }

    *act = '\0';

    return s1;
}

int mi_strlen(const char *s1)
{
    const char *i = s1;
    int ce=0;

    while(*i != '\0')
    {
        i++;
        ce++;
    }

    return ce;
}

//busca una subcadena s2 en la cadena s1
char *mi_strstr(const char *s1, const char *s2)
{
    if (!*s2)
        return NULL;

    const char *i = s1;

    while (*i != '\0')
    {
        const char *pos1 = i;
        const char *pos2 = s2;

        while (*pos1 != '\0' && *pos2 != '\0' && *pos1 == *pos2)
        {
            pos1++;
            pos2++;
        }

        if (*pos2 == '\0')
            return (char *)i;

        i++;
    }

    return NULL;
}

//busca la ultima aparicion del caracter
char *mi_strrchr(const char *s1, const char c)
{
    const char *i = s1 + strlen(s1);
    const char *ini = s1;

    while(i >= ini)
    {
        if(*i == c)
            return (char *)i;
        else
            i--;
    }

    return NULL;
}

//busca la primera aparicion del caracter
char *mi_strchr(const char *s1, const char c)
{
    const char *i = s1;

    while(*i != '\0')
    {
        if(*i == c)
        {
            return (char *)i;
        }
        else
            i++;
    }

    return NULL;
}

// Función para saber si un caracter está entre los delimitadores
bool es_delimitador(char c, const char *delim)
{
    while(*delim)
    {
        if(c == *delim)
            return true;
        delim++;
    }
    return false;
}

//(desde el inicio -> fin)
//separa la cadena en tokens pero CUIDADO ya que la modifica
char *mi_strtok(char *s1, const char *delim) {
    static char *resto = NULL;
    char *inicio;

    // Primera llamada
    if (s1 != NULL)
        resto = s1;

    // Si ya no queda más cadena
    if (resto == NULL)
        return NULL;

    // Saltar delimitadores iniciales
    while (*resto && es_delimitador(*resto, delim))
        resto++;

    // Si se terminó la cadena
    if (*resto == '\0')
        return NULL;

    // Marcar inicio del token
    inicio = resto;

    // Avanzar hasta el próximo delimitador o fin
    while (*resto && !es_delimitador(*resto, delim))
        resto++;

    // Si encontramos delimitador, lo reemplazamos por '\0' y avanzamos
    if (*resto)
    {
        *resto = '\0';
        resto++;
    }
    else
        resto = NULL;// Ya no hay más tokens

    return inicio;
}

char *normalizar(char *s1)
{
    char nomAux[CANTNOMBREAUX];
    int i=0;

    while(*s1 != '\0')
    {
        if(esLetra(*s1) )
        {
            //tulower(*s1);
            nomAux[i++] = toupper(*s1);
        }
        else if(esNumero(*s1))
        {
            nomAux[i++] = *s1;
        }

        s1++;
    }

    nomAux[i] = '\0';
    strcpy(s1,nomAux);

    return s1;
}
// TDA VECTOR
bool crearVector(vector *v, size_t tamElem, size_t capIni)
{
    v->ce = 0;
    v->vec = malloc(tamElem * capIni);
    if(!v->vec)
        return false;


    v->cap = capIni;
    v->tamElem = tamElem;

    return true;
}

void destruirVec(vector *v)
{
    v->ce = 0;
    free(v->vec);
    v->vec = NULL;
    v->tamElem =0;
    v->cap = 0;
}

int insertarAlPrincipio(vector *v, void *elem)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v,FACTOR_INCR))
        {
            return ERR_MEMORIA;
        }
    }

    void *ini = v->vec;
    void *fin = v->vec + (v->ce -1) * v->tamElem;

    for(void *i = fin; i >= ini ; i-= v->tamElem)
    {
        memcpy(i + v->tamElem, i, v->tamElem);
    }

    memcpy(ini, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}

int insertarAlFinal(vector *v, void *elem)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v,FACTOR_INCR))
        {
            return ERR_MEMORIA;
        }
    }

    void *fin = v->vec + v->ce * v->tamElem;

    memcpy(fin, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}

int insertarEnPos(vector *v, void *elem, int pos)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v,FACTOR_INCR))
        {
            return ERR_MEMORIA;
        }
    }
    // verificar si es una pos valida

    void *posIns = v->vec + (pos -1)*v->tamElem;
    void *fin = v->vec +(v->ce -1 ) * v->tamElem;

    for(void *i = fin ; i<=posIns ; i-=v->tamElem)
    {
        memcpy(i + v->tamElem , i, v->tamElem);
    }

    memcpy(posIns, elem, v->tamElem);

    v->ce++;

    return TODO_OK;
}

int insertarVecOrd(vector *v, void *elem, Cmp cmp)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v,FACTOR_INCR))
        {
            return ERR_MEMORIA;
        }
    }

    if(v->ce == 0)
    {
        memcpy(v->vec, elem, v->tamElem);
        v->ce++;
        return TODO_OK;
    }

    void *ult = v->vec + (v->ce - 1) * v->tamElem;
    void *i = v->vec;

    while(cmp(elem,i)>0 && i <= ult)
    {
        i += v->tamElem;
    }

    if(cmp(elem,i)==0)
    {
        return DUPLICADO;
    }

    void *posIns = i;

    for(i = ult ; i>= posIns ; i-=v->tamElem)
    {
        memcpy(i + v->tamElem, i , v->tamElem);
    }

    memcpy(posIns, elem, v->tamElem);

    v->ce++;

    return TODO_OK;

}

bool redimensionarVector(vector* v, float factor)
{
    size_t nuevaCap = max(v->cap * factor, CAPINI);
    void* nuevoVec = realloc(v->vec, nuevaCap * v->tamElem);

    if(!nuevoVec) // nuevoVec == NULL
    {
        return false;
    }

    if(v->cap != nuevaCap)
    {
        printf("Redimensión del vector de %lu a %lu.\n",(unsigned long)v->cap, (unsigned long)nuevaCap);
    }

    v->cap = nuevaCap;
    v->vec = nuevoVec;

    return true;
}

size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}


// bool esEliminarElem(vector *v, void *elem)
// {
//     void *pos = buscarElemVecOrd(v,elem, cmp);

//     if(pos)
//     {
//         if(eliminarElemVecOrd(v, pos)== TODO_OK)
//             return true;
//     }

//     return false;
// }

// int eliminarElemVecOrd(vector *v, void *pos)
// {
//     void *ult = v->vec + (v->ce -1) * v->tamElem;

//     for(void *i = pos ; i < ult ; i+= v->tamElem)
//     {
//         memcpy(i, i + v->tamElem, v->tamElem);
//     }

//     v->ce--;

//     if((float)v->ce / v->cap <= FACTOR_OCUP)
//     {
//         if(!redimensionarVector(v,FACTOR_DECR))
//         {
//             return ERR_MEMORIA;
//         }
//     }

//     return TODO_OK;
// }

 



//            ORDENAMIENTO 

// usar el cmp el cual queremos comparar 
void ordenarBurbujeo(vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) *v->tamElem;

    for(void* i = v->vec, *limJ = ult - 1; i < ult; i+=v->tamElem, limJ-=v->tamElem)
    {
        for(void* j = v->vec; j <= limJ; j+=v->tamElem)
        {
            if(cmp(j,j+v->tamElem)>0)
            {
                intercambiarProducto(j, j+v->tamElem);
            }
        }
    }
}

void intercambiarProducto(void *a , void *b)
{
    producto *aux;
    producto *a1 = a;
    producto *b1 = b;

    memcpy(aux, b1, sizeof(producto));
    memcpy(b1, a1, sizeof(producto));
    memcpy(a1, aux, sizeof(producto));
}

void ordenarSeleccion(vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* m;

    for(void* i = v->vec; i < ult; i += v->tamElem)
    {
        m = buscarMenor(i, ult, cmp, v->tamElem);
        intercambiar(m, i, v->tamElem);
    }
}