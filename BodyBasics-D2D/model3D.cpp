#include "model3D.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


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
    

	// Compruebo que actYear se encuentre en el intervalo [1000, 2019]
    if (actYear + year_inc < minYear) {
        year_inc = minYear - actYear;
    }
    else if (actYear + year_inc > maxYear)
        year_inc = maxYear - actYear;
 
    actYear += year_inc;
    
    barra_x += 0.0055 * year_inc;
  
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

    scale += factor * 2.0;

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}


void model3D::rotar(int grados) {
	char texto_gesto[40];
	sprintf(texto_gesto, "<Rotar> %d\n", grados);
    
    alpha += grados;

	// Lo paso de char* a LPWSTR para poder imprimirlo
	wchar_t wtext[40];
	mbstowcs(wtext, texto_gesto, strlen(texto_gesto) + 1);
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}

void model3D::drawCube()
{
    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

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
    glTranslatef(0.0, -0.5, 0.0);
    glScalef(this->scale, this->scale, this->scale);
    glRotatef(alpha, 0, 1, 0);

    
    //attempt to rotate cube
    
    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}

void model3D::display(GLFWwindow* window)
{

        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw stuff
        glClearColor(0.0, 0.8, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -5);

        drawCube();
        drawLineaTemporal();
        drawMarcaTemporal();

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
}

void model3D::drawLineaTemporal() {

    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

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

    glTranslatef(0.0, 2.3, 0.0);
    glScalef(3.5, 0.2, 0.01);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
 
    glPopMatrix();
}


void model3D::drawMarcaTemporal() {

    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

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

    glTranslatef(barra_x, 1.83, 1.0);
    glScalef(0.05, 0.3, 0.01);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}
