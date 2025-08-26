#include "..\examen.h"



int cmpProducto(const void *a, const void *b);
void *buscarElemVecOrd(vector *v, void *elem,Cmp cmp);



int main()
{
    vector v;

   // Crear vector para almacenar producto
   crearVector(&v,sizeof(producto),CAPINI);

   insertarAlFinal(&v, &(producto){1, "Producto A"});
   insertarAlFinal(&v, &(producto){2, "Producto B"});
   insertarAlFinal(&v, &(producto){3, "Producto C"});

    producto p4 = {2, "Producto B"};
    producto * encontrado = buscarElemVecOrd(&v, &p4, cmpProducto);

   printf("Producto encontrado: Codigo %d, Nombre %s\n", encontrado->codigo, encontrado->nombre);
   destruirVec(&v);

   return 0;
}


void *buscarElemVecOrd(vector *v, void *elem, Cmp cmp)
{
    if(v->ce == 0)
        return NULL;

    void *li = v->vec;
    void *ls = v->vec + (v->ce - 1) * v->tamElem;
    void *m;

    while(li <= ls)
    {
        m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;

        int comp = cmp(elem, m);

        if(comp == 0)
            return m;
        else if(comp > 0)
            li = m + v->tamElem;
        else
            ls = m - v->tamElem;
    }

    return NULL;
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



