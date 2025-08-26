#ifndef SOLUCION_PARCIAL_GALLETITAS_TOPICOS_H
#define SOLUCION_PARCIAL_GALLETITAS_TOPICOS_H

typedef struct
{
	int nro_dep;
	char cod_prod[16];
	int stock;
}
Deposito;


typedef struct
{
	int depOrigen;
	int depDestino;
	char codProd[16];
	int cantidad;
}
Transferencia;


typedef struct
{
	int nroDep;
	char codProd[16];
	int cantidad;
}
TransferenciaElem;


void generarArchivos();
void mostrarArchivoTransferencias(const char* archTransferencias);
void mostrarArchivoDepositos(const char* archDepositos);
void invertirSubcadena(char* origen, char* destino);
void encriptarTransferencias(const char* archOrigen, const char* archDestino);
void desencriptarTransferencias(const char* archOrigen, const char* archDestino);
void actualizarDepositos(const char* archTransferencias, const char* archDepositos);


#endif // SOLUCION_PARCIAL_GALLETITAS_TOPICOS_H
