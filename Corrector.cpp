	/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Ponga su nombre y numero de cuenta aqui.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#include <ctype.h>	
//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
//DECLARAR E INICIAR LAS FUNCIONES
int revertir(int posicion, char szPalabraLeidaCopia2[TAMTOKEN], char szPalabrasSugeridas[][TAMTOKEN], int jPalabra, int longitud);
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos);
void Leerarchivo(char* sznombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos);
void Limpiarpalabra(char* palabra);
void OrdenarDiccionario(char szPalabras[][TAMTOKEN], int iEstadisticas[], int iNumElementos);
void limpiaryNormalizar(char* palabra, const char* simbolos);
void Eliminar(char szPalabras[][TAMTOKEN], int& elementos, int iEstadisticas[]);
void Ordenar(char szPalabras[][TAMTOKEN], int elementos, int iEstadisticas[], int opcion);
	
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{

	iNumElementos = 0;	
	Leerarchivo		     (szNombre, szPalabras, iEstadisticas, iNumElementos);
	OrdenarDiccionario   (szPalabras,iEstadisticas,iNumElementos);

}
void Leerarchivo(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* archivo;
	fopen_s(&archivo, szNombre, "r");
	
	char palabra[TAMTOKEN];
	while (fscanf_s(archivo, "%s", palabra, TAMTOKEN) != EOF)
	{
		Limpiarpalabra(palabra);
		if (strlen(palabra) > 0)
		{
			int existe = 0;
			for (int i = 0; i < iNumElementos; i++)
			{
				if (strcmp(szPalabras[i], palabra) == 0)
				{
					iEstadisticas[i]++;
					existe = 1;
					break;
				}	

			}
			if (!existe && iNumElementos < NUMPALABRAS)
			{
				strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabra);
				iEstadisticas[iNumElementos] = 1;
				iNumElementos++;
			}
		}
	}
	fclose(archivo);
		
}	
void Limpiarpalabra (char* palabra)
{
	int j = 0;
	for (int i = 0; palabra[i]; i++)
	{
		if (isalpha(palabra[i]) || strchr("�����", palabra[i]))
		{
			palabra[j++]= tolower(palabra[i]);
		}		
	}
	palabra[j] = '\0';

}
void limpiarYNormalizar(char* palabra, const char* simbolos)
{
	int indiceValido = 0; // �ndice para sobrescribir caracteres v�lidos

	// Recorrer la cadena y procesar cada car�cter
	for (int i = 0; palabra[i] != '\0'; i++) 
	{
		int esValido = 1;

		// Comprobar si el car�cter est� en la lista de s�mbolos
		for (int j = 0; simbolos[j] != '\0'; j++) 
		{
			if (palabra[i] == simbolos[j])
			{
				esValido = 0;
				break;
			}
		}

		// Si el car�cter es v�lido, conservarlo y convertirlo a min�scula
		if (esValido)
		{
			palabra[indiceValido++] = tolower(palabra[i]);
		}
	}

	// Terminar la cadena con un car�cter nulo
	palabra[indiceValido] = '\0';
}
void OrdenarDiccionario(char szPalabras[][TAMTOKEN], int iEstadisticas[], int iNumElementos)
{
	for (int i = 0; i < iNumElementos - 1; i++)
	{	
		for (int j = 0; j < iNumElementos - i - 1; j++)
		{
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
			{
				char temp[TAMTOKEN];
				
				strcpy_s(temp, TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
				strcpy_s(szPalabras[j + 1], TAMTOKEN, temp);
				int tempFreq = iEstadisticas[j];
				iEstadisticas [j] = iEstadisticas[j + 1];
				iEstadisticas [j + 1] = tempFreq;

			}
		}
	}

}


/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/

void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal

{
	int iEstadisticas2[100];
	iNumLista = 0;
	for (int i = 0; i < iNumSugeridas; i++)
	{
		for (int j = 0; j < iNumElementos; j++)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0)
			{
				strcpy_s(szListaFinal[iNumLista], TAMTOKEN, szPalabras[j]);
				iPeso[iNumLista] = iEstadisticas[j];
				iNumLista++;
			}
		}
	}
	Eliminar(szListaFinal, iNumLista, iEstadisticas2);
	Ordenar(szListaFinal, iNumLista, iPeso, 1);
}
void Eliminar(char szPalabras[][TAMTOKEN], int& elementos, int iEstadisticas[]) {
	int i = 0;
	while (i < elementos) {
		int duplicado = 0; // 0 significa que no hay duplicado, 1 significa que se encontr� uno.
		for (int j = i + 1; j < elementos; j++) {
			if (strcmp(szPalabras[i], szPalabras[j]) == 0) {
				iEstadisticas[i] += iEstadisticas[j]; // Combinar frecuencias
				for (int k = j; k < elementos - 1; k++) {
					strcpy_s(szPalabras[k], TAMTOKEN, szPalabras[k + 1]); // Shift izquierda
					iEstadisticas[k] = iEstadisticas[k + 1];
				}
				elementos--;
				for (int k = 0; k < TAMTOKEN; k++) { // Limpieza �ltima posici�n
					szPalabras[elementos][k] = '\0';
				}
				iEstadisticas[elementos] = 0;
				duplicado = 1; // Indicar que se encontr� un duplicado.
				break;
			}
		}
		if (duplicado == 0) { // Si no hubo duplicados, avanzar al siguiente �ndice.
			i++;
		}
	}
}

void Ordenar(char szPalabras[][TAMTOKEN], int elementos, int iEstadisticas[], int opcion)
{
	int i, j, min_idx;
	char aux[TAMTOKEN];
	int auxNUM; 
	if (opcion == 0)
	{
		for (i = 0; i < elementos - 1; i++)
		{
			min_idx = i;
			for (j = i + 1; j < elementos; j++)
			{
				if (strcmp(szPalabras[j], szPalabras[min_idx]) < 0)
				{
					min_idx = j;
				}
			}
			strcpy_s(aux, TAMTOKEN, szPalabras[min_idx]);
			strcpy_s(szPalabras[min_idx], TAMTOKEN, szPalabras[i]);
			strcpy_s(szPalabras[i], TAMTOKEN, aux);

			auxNUM = iEstadisticas[min_idx];
			iEstadisticas[min_idx] = iEstadisticas[i];
			iEstadisticas[i] = auxNUM;
		}	
	}
	else
	{
		if (opcion == 1)
		{
			for (i = 0; i < elementos - 1; i++)
			{
				min_idx = i;
				for (j = i + 1; j < elementos; j++)
				{
					if (iEstadisticas[j] < iEstadisticas[min_idx])
					{
						min_idx = j;
					}

				}
				auxNUM = iEstadisticas[min_idx];
				iEstadisticas[min_idx] = iEstadisticas[i];
				iEstadisticas[i] = auxNUM; 

				strcpy_s(aux, TAMTOKEN, szPalabras[min_idx]);
				strcpy_s(szPalabras[min_idx], TAMTOKEN, szPalabras[i]);
				strcpy_s(szPalabras[i], TAMTOKEN, aux);
			}
		}
	}

}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
{
	int longitud = strlen(szPalabraLeida);
	int posicion = 0;
	int jPalabra = 0;
	char letras[] = "abcdefghijklmnopqrstuvwxyz";
	char letrasEspeciales[] = "������";
	char szPalabraLeidaCopia[TAMTOKEN];
	char szPalabraLeidaCopia2[TAMTOKEN];
	char palabraFinal[TAMTOKEN];
	char auxPalabra1[TAMTOKEN];
	char auxPalabra2[TAMTOKEN];

	for (int i = 0; i < longitud; i++)
	{
		for (size_t j = 0; j < strlen(letras); j++)
		{
			if (szPalabraLeida[i] == letrasEspeciales[0])
			{
				posicion = i;
			}
		}
	}
	if (posicion != 0)
	{
		int apuntador = 0;
		for (int i = 0; i < longitud; i++)
		{
			if (szPalabraLeida[i] != letrasEspeciales[1])
			{
				palabraFinal[apuntador++] = szPalabraLeida[i];
			}
		}
		palabraFinal[apuntador] = '\0';
		strcpy_s(szPalabraLeida, TAMTOKEN, palabraFinal);
		longitud--;
	}

	for (int i = 0; i < longitud; i++)
	{
		int apuntador = 0;
		for (int j = 0; j < longitud; j++)
		{
			if (i != j)
			{
				palabraFinal[apuntador++] = szPalabraLeida[j];
			}
		}
		palabraFinal[apuntador] = '\0';
		jPalabra = revertir(posicion, palabraFinal, szPalabrasSugeridas, jPalabra, longitud);
	}
	for (int j = 1; j < longitud; j++)
	{
		strcpy_s(szPalabraLeidaCopia2, szPalabraLeida);
		for (int i = 0; i < longitud - j ; i++)
		{
			char temp = szPalabraLeidaCopia2[i + j];
			szPalabraLeidaCopia2[i + j] = szPalabraLeidaCopia2[i + (j - 1)];
			szPalabraLeidaCopia2[i + (j - 1)] = temp;
			jPalabra = revertir(posicion, szPalabraLeidaCopia2, szPalabrasSugeridas, jPalabra, longitud);
		}
	}
	for (int i = 0; i < longitud; i++)
	{
		strcpy_s(szPalabraLeidaCopia, szPalabraLeida);
		for (size_t j = 0; j < strlen(letras); j++)
		{
			szPalabraLeidaCopia[i] = letras[j];
			jPalabra = revertir(posicion, szPalabraLeidaCopia, szPalabrasSugeridas, jPalabra, longitud);
		}
	}
	for (int i = 0; i < longitud; i++)
	{
		for (size_t j = 0; j < strlen(letrasEspeciales); j++)
		{
			for (int k = 0; k < TAMTOKEN; k++)
			{
				auxPalabra1[k] = '\0';
				auxPalabra2[k] = '\0';

			}
			strncpy_s(auxPalabra1, szPalabraLeida, i);
			strcpy_s(auxPalabra2, szPalabraLeida + i + 1);
			auxPalabra1[i] = letrasEspeciales[j];
			strcat_s(auxPalabra1, auxPalabra2);
			jPalabra = revertir(posicion, auxPalabra1, szPalabrasSugeridas, jPalabra, longitud);
		}
	}
	for (int i = 0; i <= longitud; i++)
	{
		for (size_t j = 0; j < strlen(letras); j++)
		{
			if (jPalabra >= NUMPALABRAS)
			{
				return;
			}
			for (int k=0;; k < TAMTOKEN; k++)
			{
				auxPalabra1[k] = '\0';
			}
			strncpy_s(auxPalabra1, szPalabraLeida, i);
			auxPalabra1[i] = letras[j];
			strncpy_s(auxPalabra1 + i + 1, TAMTOKEN - i - 1, szPalabraLeida + i, TAMTOKEN - i - 1);
			jPalabra = revertir(posicion, auxPalabra1, szPalabrasSugeridas, jPalabra, longitud);
		}

}


	int revertir(int posicion, char szPalabraLeidaCopia2[TAMTOKEN], char szPalabrasSugeridas[][TAMTOKEN], int jPalabra, int longitud)
	{
		int posicion2 = -1; // Variable para guardar la posici�n del car�cter especial
		char letrasEspeciales[] = "������";

		// Validar si la longitud es mayor que 0
		if (longitud > 0) {
			// Buscar la posici�n del primer car�cter especial
			for (int k = 0; k < longitud; k++) {
				if (strchr(letrasEspeciales, szPalabraLeidaCopia2[k])) {
					posicion2 = k;
					break;
				}
			}

			// Si encontramos un car�cter especial
			if (posicion2 != -1) {
				// Generar palabras excluyendo el car�cter especial
				for (int m = 0; m < longitud; m++) {
					if (m != posicion2) { // Evitar incluir la posici�n del car�cter especial
						char auxPalabra[TAMTOKEN] = { '\0' };

						// Copiar la parte inicial hasta la posici�n 'm'
						strncpy_s(auxPalabra, szPalabraLeidaCopia2, m);

						// Concatenar el resto de la palabra despu�s de la posici�n 'm'
						strcat_s(auxPalabra, TAMTOKEN, szPalabraLeidaCopia2 + m + 1);

						// A�adir la palabra generada a la lista, respetando el l�mite
						if (jPalabra < NUMPALABRAS) {
							strcpy_s(szPalabrasSugeridas[jPalabra], TAMTOKEN, auxPalabra);
							jPalabra++;
						}
						else {
							return jPalabra; // Si se alcanza el l�mite, salir inmediatamente
						}
					}
				}
			}
			else {
				// Si no hay caracteres especiales, simplemente agregar la palabra original
				if (jPalabra < NUMPALABRAS) {
					strcpy_s(szPalabrasSugeridas[jPalabra], TAMTOKEN, szPalabraLeidaCopia2);
					jPalabra++;
				}
			}
		}

		return jPalabra; // Retornar el n�mero actualizado de palabras sugeridas

	}

