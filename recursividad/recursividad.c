#include<stdio.h>
#include<stdlib.h>

 


int factorial (int n);
int dcm (int x, int y);

int main()
{
    printf("factorial: %d\n", factorial(5));
    printf("dcm: %d\n", dcm(16, 24));

    return 0;
}


int factorial (int n)
{
    if(n<=1)
    {
        return 1;
    }

    return n * factorial(n-1);
}


int dcm(int x, int y)
{
    if(y==0)
    {
        return x;
    }

    return dcm(y, x % y);
}




