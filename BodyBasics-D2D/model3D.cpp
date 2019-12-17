#include "model3D.h"
#include <cstdio>
#include <cstdlib>
#include <windows.h>



// Por ahora, las acciones de la API muestran el gesto realizado, impreso a la consola de DEBUG
// usando OutputDebugString

void model3D::desplazar(float desp_x, float desp_y) {
	char texto_gesto[40];
	sprintf(texto_gesto, "<Desplazar> %.2f, %.2f\n", desp_x, desp_y);

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;
	
	OutputDebugString(ptr);
}

void model3D::cambiarAnio(int year_inc) {
	actYear += year_inc;

	// Compruebo que actYear se encuentre en el intervalo [1000, 2019]
	if (actYear < minYear)
		actYear = minYear;
	else if (actYear > maxYear)
		actYear = maxYear;

	char texto_gesto[40];
	sprintf(texto_gesto, "<Anio actual> %d\n", actYear);

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}

void model3D::zoom(float factor) {
	char texto_gesto[40];
	sprintf(texto_gesto, "<Zoom> %.2f\n", factor);

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}


void model3D::rotar(int grados) {
	char texto_gesto[40];
	sprintf(texto_gesto, "<Rotar> %d\n", grados);

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}