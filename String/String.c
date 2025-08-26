#include "String.h"

size_t mi_strlen (const char *s)
{
	size_t cant = 0;

	while(*s != '\0')
	{
		cant++;
		s++;
	}
	return cant;
}

char *mi_strcpy(char *s1, const char *s2)
{
	char *ini = s1;

	while(*s2 != '\0')      // Mientras no sea el final de la cadena
	{
		*s1 = *s2;          // Copiá el carácter
		s1++;               // Avanzá el puntero destino
		s2++;               // Avanzá el puntero origen
	}
	*s1 = '\0';              // Acordate de terminar la cadena copiada

	return ini;  // devolvemos la pos inicial del puntero
}

char *mi_strncpy(char *s1, const char *s2, size_t n)
{
	char *ini = s1;

	while( n!=0 && *s2 != '\0')      // Mientras no sea el final de la cadena
	{
		*s1 = *s2;          // Copiá el carácter
		s1++;               // Avanzá el puntero destino
		s2++;               // Avanzá el puntero origen
		n--;
	}
	*s1 = '\0';              // Acordate de terminar la cadena copiada

	return ini;  // devolvemos la pos inicial del puntero
}


char *mi_strcat (char *s1, const char *s2)
{
	char *ini = s1;
	size_t ce = mi_strlen(s1);

	s1 = s1 + ce; //mueve a s1 a la pos final para poder concatenar 

	while(*s2 != '\0')
	{
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1 = '\0';

	return ini;
}

char *strncat (char *s1, const char *s2, size_t n)
{
	char *ini = s1;
	size_t ce = mi_strlen(s1);
	
	s1 += ce;

	while( n!=0 && *s2!='\0' )
	{
		*s1 = *s2;
		s1++;
		s2++;
		n--;
	}

	*s1 = '\0';
	return ini;
}

char *mi_strchr (char *s, int c)
{

	do           //uso del DO ya que tmb compara el caracter '\0'
	{
		if((unsigned char)*s == (unsigned char)c) // casteamos para no tener poblemas con los enteros mayores a 128 (num negativos)
			return s;
		
		s++;
	}
	while(*s != '\0');

	return NULL;
}

int mi_strcmp (const char *s1,const char *s2)
{
	while (*s1 !='\0' && *s2 !='\0')
	{
		if(*s1 == *s2)
		{
			s1++;
			s2++;
		}
		else 
			return (unsigned char)*s1 - (unsigned char)*s2;
	}

	return (unsigned char)*s1 - (unsigned char)*s2;
}

int mi_strncmp (const char *s1,const char *s2, size_t n)
{
	while (*s1 !='\0' && *s2 !='\0' && n!=0)
	{
		if(*s1 == *s2)
		{
			s1++;
			s2++;
			n--;
		}
		else return *s1-*s2;
	}

	return *s1 - *s2;
}

char *strstr (const char *s1, const char *s2) // busca la cadena s2 en s1 
{
	char *iniS2 = s2;

	while(*s1 !='\0' || *s2!='\0')
	{
		if(*s1 == *s2)
		{
			s1++;
			s2++;
		}
		else
		{
			s2 = iniS2;
		}
	}

	if(*s2 == '\0')
		return s1 -= mi_strlen(s2);
	else		
		return NULL;
}




