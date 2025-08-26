#include "examen.h"

//------------------------- libreria string.h---------------------------------
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
//--------------------------TDA VECTOR--------------------------------------
bool crearVector(vector *v, size_t tamElem, size_t cap_ini)
{
    v->ce = 0;
    v->vec = malloc(cap_ini * tamElem);
    if(!v->vec)
    {
        v->cap = 0;
        return false;
    }

    v->tamElem = tamElem;
    v->cap = cap_ini;

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

bool esEliminarElem(vector *v, void *elem, Cmp cmp)
{
    void *pos = buscarElemVecOrd(v,elem, cmp);

    if(pos)
    {
        if(eliminarElemVecOrd(v, pos)== TODO_OK)
            return true;
    }

    return false;
}

int eliminarElemVecOrd(vector *v, void *pos)
{
    void *ult = v->vec + (v->ce -1) * v->tamElem;

    for(void *i = pos ; i < ult ; i+= v->tamElem)
    {
        memcpy(i, i + v->tamElem, v->tamElem);
    }

    v->ce--;

    if((float)v->ce / v->cap <= FACTOR_OCUP)
    {
        if(!redimensionarVector(v,FACTOR_DECR))
        {
            return ERR_MEMORIA;
        }
    }

    return TODO_OK;
}

int vectorOrdBuscar(const vector* v, void* elem, Cmp cmp)
{
    if(v->ce == 0)
        return -1;

    const void* li = v->vec;
    const void* ls = v->vec + (v->ce - 1) * v->tamElem;
    const void* m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;
    int comp = cmp(elem, m);

    while(li <= ls && comp != 0)
    {
        if(comp > 0)
        {
            li = m + v->tamElem;
        }
        else
        {
            ls = m - v->tamElem;
        }

        m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;
        comp = cmp(elem, m);
    }

    if(comp == 0)
    {
        memcpy(elem, m, v->tamElem);
        return (m - v->vec) / v->tamElem;
    }

    return -1;
}

int cmpProducto(const void *a, const void *b)
{
    const producto *p1 = (const producto *)a;
    const producto *p2 = (const producto *)b;

    if(p1->codigo < p2->codigo)
        return -1;
    else if(p1->codigo > p2->codigo)
        return 1;
    else
        return 0;
}

//-------------------------------ORDENAMIENTO-----------------------------

// usar el cmp el cual queremos comparar 
void ordenarBurbujeo(vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) *v->tamElem;

    for(void* i = v->vec, *limJ = ult - v->tamElem; i < ult; i+=v->tamElem, limJ-=v->tamElem)
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
    producto aux;
    producto *a1 = a;
    producto *b1 = b;

    memcpy(&aux, b1, sizeof(producto));
    memcpy(b1, a1, sizeof(producto));
    memcpy(a1, &aux, sizeof(producto));
}

void ordenarSeleccion(vector *v, Cmp cmp)
{
    void *ult = v->vec + (v->ce -1) * v->tamElem;
    void *elem;

    for(void *i =v->vec ; i<ult ; i+=v->tamElem)
    {
        elem = buscarMenor(v,i,cmp);
        if(elem!=i)
        {
            intercambiarProducto(i, elem);
        }
       
    }    
}

void *buscarMenor(vector *v, void *pos , Cmp cmp)
{
    void *ult = v->vec + (v->ce -1) *v->tamElem;
    void *elem = pos;

    for(void *i = pos ; i<ult ; i+=v->tamElem )
    {
        if(cmp(i,i+v->tamElem)<0)
        {
            elem = i;
        }
    }

    return elem;
}

//-------------------------------ITERADOR------------------------------------
void vectorIteradorCrear(vectorIterador *it, vector *v)
{
    it->vector = v;
    it->ult = NULL;
    it->act = NULL;
    it->finIter = true;
    
}

void *vectorIteradorPrimero(vectorIterador * it) // inicia la recorrida en caso de ser necesaria 
{
    vector *v = it->vector;

    if(v->ce == 0)
    {
        it->finIter = true;
        return NULL;
    }

    it->act = v->vec;
    it->ult = v->vec + (v->ce -1) *v->tamElem;
    it->finIter = false;

    return it->act;
}

void *vectorIteradorSiguiente(vectorIterador * it)
{
    void *sig = it->act + it->vector->tamElem;

    if(sig > it->ult)
    {
        it->finIter= true;
        return NULL;
    }

    it->act = sig;

    return sig;
}

bool vectorIteradorFin(vectorIterador *it)
{
    return it->finIter;
}

//------------------------------ARCHIVO BINARIO--------------------------------
int vectorCrearDeArchivo(vector* v, size_t tamElem, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "rb");

    if(!arch)
    {
        return ERR_ARCH;
    }

    fseek(arch, 0L, SEEK_END);
    size_t cantBytes = ftell(arch);
    size_t cantRegs = cantBytes / tamElem;

    v->vec = malloc(cantBytes);

    if(!v->vec)
    {
        return ERR_MEMORIA;
    }

    rewind(arch);

    fread(v->vec, tamElem, cantRegs, arch);
    v->ce = cantRegs;
    v->cap = cantRegs;
    v->tamElem = tamElem;

    fclose(arch);

    return TODO_OK;
}

int convertirBINaTXT (const char *nomArchBin, const char *nomArchTxt, size_t tamReg, BINaTXT binaTxt)
{
    FILE *archBin = fopen(nomArchBin, "rb");
    if(!archBin)
    {
        return ERR_ARCH;
    }

    FILE *archTxt = fopen(nomArchTxt, "wt");
    if(!archTxt)
    {
        fclose(archBin);
        return ERR_ARCH;
    }

    void *reg = malloc(tamReg);
    if(!reg)
    {
        fclose(archTxt);
        fclose(archBin);
        return ERR_MEMORIA;
    }

    // fseek(archBin, 0L, SEEK_END);
    // size_t cantBytes = ftell(archBin);
    // if(cantBytes == 0)
    //     return VACIO;
    // rewind(archBin);

    while(fread(reg,tamReg,1,archBin))
    {
        binaTxt(reg, archTxt);
    }

    fclose(archBin);
    fclose(archTxt);
    free(reg);
    
    return TODO_OK;
}

void binaTxtV(const void *reg, const char*ArchTxt)
{
    const producto *pro= reg;
    fprintf(ArchTxt,"%d|%s \n",pro->codigo,pro->nombre);
}
/* 
 lo que hace cada valor:  (%0) rellena con 0  
                          (%-) alineacion a la izquierda
                          (%*) "puntero a la variable constante definida" #define TAM 10
*/
void binaTxtF(const void *reg, const char*ArchTxt)
{
    const producto *pro= reg;
    fprintf(ArchTxt,"%-0*d %*s \n",TAMCODIGO,pro->codigo,TAMNOMBRE,pro->nombre);
}

int convertirTXTaBIN(const char* nomArchBin, const char *nomArchTxt, size_t tamReg, TXTaBIN txtaBin)
{
    FILE *archBin = fopen(nomArchBin,"rb");
    if(!archBin)
    {
        return ERR_ARCH;
    }

    FILE *archTxt = fopen(nomArchTxt,"wt");
    if(!archTxt)
    {
        fclose(archBin);
        return ERR_ARCH;
    }

    void *reg = malloc(tamReg);
    if(!reg)
    {
        fclose(archBin);
        fclose(archTxt);
        return ERR_MEMORIA;
    }
    
    char linea [CANTLINEA+1];
    int ret = TODO_OK;

    while(ret == TODO_OK && fgets(linea, CANTLINEA+1, archTxt))
    {
        ret = txtaBin(reg,linea);

        if(ret == TODO_OK)
        {
            fwrite(reg,tamReg,1,archBin);
        }
    }


    fclose(archBin);
    fclose(archTxt);
    return TODO_OK;
}

int productoTxtaBinV(void *reg, char *linea)
{
    producto *pro = reg;

    char *act = strrchr(linea,'\n');
    if(!act)
        return ERR_TAM_LINEA;

    //nombre
    *act = '\0';
    act = strrchr(linea,'|');
    sscanf(act+1,"%s",pro->nombre);

    //codigo
    *act = '\0';
    act = strrchr(linea,'|');
    sscanf(act+1,"%s",pro->codigo);

    return TODO_OK;
}

int productoTxtaBinF(void *reg, char *linea)
{
    producto *pro = reg;

    char *act = strrchr(linea,'\n');
    if(!act)
        return ERR_TAM_LINEA;

    //nombre
    *act = '\0';
    act -= TAMNOMBRE;
    strncpy(pro->nombre,act,TAMNOMBRE);
    pro->nombre[TAMNOMBRE] = '\0';

    //codigo 
    *act ='\0';
    act -=TAMCODIGO;
    sscanf(linea, "%d", pro->codigo);

    return TODO_OK;
}

//----------------------------- MERSE ------------------------------------------

int merse (const char *nomArchPro, const char *nomArchMov)
{
    FILE *archPro = fopen(nomArchPro, "rb");
    if(!archPro)
    {
        return ERR_ARCH;
    }

    FILE *archMov = fopen(nomArchMov, "rb");
    if(!archMov)
    {
        fclose(archPro);
        return ERR_ARCH;
    }

    size_t len = strlen(nomArchMov);
    char *nomArchMovTem = malloc(len + 1);
    if (!nomArchMovTem) 
    {
        fclose(archMov);
        fclose(archPro);
        return ERR_MEMORIA;
    }   
    
    // cambiar la extencion de nomArch

    FILE *archMovTem = fopen("productotem.dat","wb");
    if(archMovTem)
    {
        free(nomArchMovTem);
        fclose(archMov);
        fclose(archPro);
        return ERR_ARCH;
    }

    producto pro;
    movimiento mov;

    /*  Producto     movimiento
        A11   -        A11  -  
        A13   -        A11  -
        A14            A12  -
                       A12  -
                       A13  -
    */
    fread(&pro,sizeof(producto),1,archPro);
    fread(&mov,sizeof(movimiento),1,archMov);

    while(!feof(archPro) && !feof(archMov))
    {
        int cmp = strcmp(pro.nombre,mov.nombre);

        if(cmp==0)
        {
            pro.stock += mov.stock;
            fread(&mov,sizeof(movimiento),1,archMov);
        }
        if(cmp<0) 
        {
            fwrite(&pro,sizeof(producto),1,archMovTem);
            fread(&pro,sizeof(producto),1,archPro);
        }
        if(cmp>0) // producto nuevo
        {
            ingresarNuevo(&mov,archMov,archMovTem);
        }

    }


    // terminar IMCOMPLETO 

    free(nomArchMovTem);
    fclose(archMov);
    fclose(archPro);
}

void ingresarNuevo(movimiento *mov, FILE *archMov, FILE * archMovTem)
{
    movimiento nueMov;

    nueMov.codigo = mov->codigo;
    strcpy(nueMov.nombre,mov->nombre);
    nueMov.descripcion ='\0';
    nueMov.stock = mov->stock;

    while(fread(&mov,sizeof(producto),1,archMov) && cmp(nueMov.nombre,mov->nombre)==0)
    {
        nueMov.stock += mov->stock;
    }

    fwrite(&nueMov,sizeof(movimiento),1,archMovTem);
}

//------------------------map - reduce - filter----------------------------------- 
void vectorReduce(const vector* v, Reduce reduce, void* reduceElem)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        reduce(i, reduceElem);
    }    
}

void vectorFilter(const vector* v, Filter filter, const void* datosFiltro, vector* vFiltrado)
{
    vectorCrear(vFiltrado, v->tamElem);
    
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        if(filter(i, datosFiltro))
        {
            vectorInsertarAlFinal(vFiltrado, i);
        }
    }      
}

bool vectorMap(const vector* v, Map map, const void* datosMap, vector* vMapeado, size_t tamElemMapeado)
{
    vectorCrear(vMapeado, tamElemMapeado);
    
    void* elemMapeado = malloc(tamElemMapeado);

    if(!elemMapeado)
    {
        return false;
    }

    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        map(i, datosMap, elemMapeado);
        vectorInsertarAlFinal(vMapeado, elemMapeado);
    }
    
    free(elemMapeado);

    return true;
}


