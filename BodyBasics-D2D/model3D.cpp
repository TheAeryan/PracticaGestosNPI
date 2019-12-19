#include "model3D.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


// Por ahora, las acciones de la API muestran el gesto realizado, impreso a la consola de DEBUG
// usando OutputDebugString

/* EFECTOS DE LOS GESTOS */

void model3D::desplazar(float desp_x, float desp_y) {
    const float FACTOR = 3.0;
	char texto_gesto[40];
	sprintf(texto_gesto, "<Desplazar> %.2f, %.2f\n", desp_x, desp_y);

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;
	
	OutputDebugString(ptr);

    // Actualizamos el desplazamiento plano XY multiplicado por un factor
    desplazamiento_x += desp_x * FACTOR;
    desplazamiento_y += desp_y * FACTOR;
}

void model3D::cambiarAnio(int year_inc) {
    const float FACTOR = 0.0055;
    
	// Compruebo que actYear se encuentre en el intervalo [1000, 2019]
    if (actYear + year_inc < minYear)
        year_inc = minYear - actYear;
    else if (actYear + year_inc > maxYear)
        year_inc = maxYear - actYear;
 
    // Actualizamos el año actual
    actYear += year_inc;
    
    // Desplazamos la marca según el nº de años
    desplazamiento_marca += FACTOR * year_inc;
  
	char texto_gesto[40];
	sprintf(texto_gesto, "<Anio actual> %d\n", actYear);

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}

void model3D::zoom(float factor) {
    const float FACTOR = 2.0,

	char texto_gesto[40];
	sprintf(texto_gesto, "<Zoom> %.2f\n", factor);

    // Aplicamos el cambio de escala por un factor
    escala += factor * FACTOR;

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}

void model3D::rotar(int grados) {
	char texto_gesto[40];
	sprintf(texto_gesto, "<Rotar> %d\n", grados);
    
    // Aplica la rotación
    ang_rotacion += grados;

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}

/* DIBUJAR ESCENA */

void model3D::setColor(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void model3D::drawObject()
{
    // Cubo base
    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

    glPushMatrix();
    // Traslación
    glTranslatef(0.0 + desplazamiento_x, -0.5 + desplazamiento_y, 0.0);
    // Escala
    glScalef(this->escala, this->escala, this->escala);
    // Cambiamos de modelo según el intervalo de tiempo
    if (actYear > 1800) {
        glScalef(3.0, 0.5, 0.3);
    }
    else if (actYear < 1200) {
        glScalef(0.4, 0.8, 1.0);
    }
    // Rotamos
    glRotatef(ang_rotacion, 0, 1, 0);
    
    // Dibujamos de un calor
    glColor3f(r, g, b);
    
    // Inicializamos
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    // Dibujamos los puntos
    glDrawArrays(GL_QUADS, 0, 24);
    // Desactivamos
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}

void model3D::display(GLFWwindow* window)
{
        // Escalamos la ventana
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Preparamos la escena
        glClearColor(0.0, 0.8, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -5);

        // Dibujamos la escena
        drawObject();
        drawLineaTemporal();
        drawMarcaTemporal();

        // Actualizamos la ventana
        glfwSwapBuffers(window);

        // Esperamos para eventos
        glfwPollEvents();
}

void model3D::drawLineaTemporal() {

    // Cubo de base
    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

    // Colores
    GLfloat colors[] =
    {
        0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
        1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
        0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
        0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
        0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
        0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
    };


    glPushMatrix();

    // Traslación y escala
    glTranslatef(0.0, 2.3, 0.0);
    glScalef(3.5, 0.2, 0.01);

    // Inicializamos
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    // Dibujamos
    glDrawArrays(GL_QUADS, 0, 24);

    // Finalizamos
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
 
    glPopMatrix();
}


void model3D::drawMarcaTemporal() {

    // Cubo de base
    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

    // Colores
    GLfloat colors[] =
    {
        0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
        1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
        0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
        0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
        0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
        0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
    };

    glPushMatrix();

    // Traslación y escala
    glTranslatef(desplazamiento_marca, 1.83, 1.0);
    glScalef(0.05, 0.3, 0.01);

    // Inicializamos
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    // Dibujamos
    glDrawArrays(GL_QUADS, 0, 24);

    // Desactivamos
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}
