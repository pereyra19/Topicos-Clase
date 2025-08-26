#include "merse.h"

// atoi ... 
// sscanf(num, "%d", &a)
// sprinf(num, "%d", a)

#define ERR_ARCHIVO -1
#define TODO_OK 0

void procesarProdNuevo(movimiento *mov,FILE *archMovs,FILE *archProdstmp);  

int main()
{  





    return 0;
}

int marse (const char *nomArchProds, const char *nomarchMovs)
{
    FILE *archProds = fopen (nomArchProds, "rb");
    if(!archProds)
    {
        return ERR_ARCH;
    }

    FILE *archMovs = fopen (nomarchMovs, "rb");
    if(!archMovs)
    {
        fclose(archMovs);
        return ERR_ARCH;
    }

    char nomProdstmp[strlen(nomArchProds)+1];
    
    strcpy(nomProdstmp , nomArchProds);
    cambiarExt(nomProdstmp, "tmp");

    FILE *archProdstmp = fopen(nomProdstmp, "wb");
    if(!archProdstmp)
    {
        fclose(archMovs);
        fclose(archProds);
        return ERR_ARCH;
    }

    producto prod;
    movimiento mov;

    fread(&prod,sizeof(producto),1,archProds);
    fread(&prod,sizeof(producto),1,archMovs);


    while(!feof(archProds)&& !feof(archMovs))
    {
        int comp = strcmp(prod.codigo, mov.codigo);
        
        if(comp == 0)
        {
            prod.stock += mov.cant;
            fread(&mov, sizeof(movimiento),1,archMovs);
        }

        if (comp<0) // producto ya ingresado 
        {
            fwrite(&prod, sizeof(producto),1,archProdstmp);
            fread(&prod,sizeof(producto),1,archProds);
        }
        if(comp>0) // nuevo producto
        {
            procesarProdNuevo(&mov,archMovs,archProdstmp); //&mov manda la dir del movimiento ya que necesito saber los mov siguientes
        }
    }

    while(!feof(archProds))
    {
        fwrite(&prod,sizeof(producto),1,archProdstmp);
        fread(&prod,sizeof(producto),1,archProds);
    }
    while(!feof(archMovs))
    {
        procesarProdNuevo(&mov,archMovs,archProdstmp);
    }

    fclose(archProds);
    fclose(archMovs);
    fclose(archProdstmp);

    remove(nomArchProds);
    rename(nomProdstmp, nomArchProds);
    return TODO_OK;
}

void procesarProdNuevo(movimiento *mov , FILE *archMovs , FILE *archProdstmp)
{
    producto prodNue;

    strcpy( prodNue.codigo, mov->codigo);
    *prodNue.descripcion = '\0'; 
    prodNue.stock = mov->cant;

    fread(mov,sizeof(movimiento),1,archMovs);
    while(!feof(archMovs) && strcmp(mov->codigo, prodNue.codigo)==0)
    {
        prodNue.stock +=mov->cant;
        fread(mov,sizeof(movimiento),1,archMovs);
    }

    fwrite(&prodNue,sizeof(producto),1,archProdstmp);
}

// otro merse para la descripcion siempre y cuando el de producto generico tenga vacio la descripcion
// otro merse generico para que si los codigos son iguales copiar los dos 
//



















