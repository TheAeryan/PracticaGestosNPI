#include "model3D.h"
#include <windows.h>

void model3D::desplazar(float desp_x, float desp_y) {
	//sprintf(texto_gesto, "<Desplazar> %f, %f", desp_x, desp_y);
	LPCWSTR str = L"Desplazando";
	OutputDebugString(str);
}