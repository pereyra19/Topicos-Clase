#include <stdio.h>
#include <stdlib.h>
#include "../TDAVector/Vector.h"

int main()
{
    Vector miVec;

    printf("hola\n");

    vectorCrear(&miVec);

	vectorOrdInsertar(&miVec,7);
	vectorOrdInsertar(&miVec,20);
	vectorOrdInsertar(&miVec,10);
	vectorOrdInsertar(&miVec,2);
	vectorOrdInsertar(&miVec,4);
	vectorOrdInsertar(&miVec,30);

	printf("%d",miVec.vec[0]);

	system("pause");
    return 0;

}
