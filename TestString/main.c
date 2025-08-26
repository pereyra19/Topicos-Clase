#include <stdio.h>
#include <stdlib.h>
#include "..\String\String.h"





int main()
{
	char v[5] = "hola";
	size_t cant;

	printf("cadena: %s\n",v);

	cant = mi_strlen(v);

	printf("cantidad de elementos: %d",cant);

    return 0;
}
