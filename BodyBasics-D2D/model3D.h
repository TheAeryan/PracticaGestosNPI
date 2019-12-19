#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Esta clase se corresponde con el modelo 3D de la Alhambra
// Se deben implementar todos estos métodos, que serán llamados
// como consecuencia de gestos

class model3D {
	private:
		int actYear; // Año actual. En función de este año se mostrará el modelo de una forma u otra.
		int minYear = 1000; // Año mínimo que puede valer actYear
		int maxYear = 2019; // Año máximo que puede valer actYear
		  // Cubo
		float alpha = 0;
		float scale = 1.0;
		float barra_x = 0.0;

		
	public:

		// Constructor
		model3D() : actYear(1510) { }

		// Gesto de mover el modelo. Se puede mover en el eje x e y
		void desplazar(float desp_x, float desp_y);
		
		// Gesto de agrandar/achicar (hacer zoom). Si 'factor' está entre 0 y 1, se achica
		// y si es mayor que 1 se agranda según 'factor'.
		void zoom(float factor);

		// Gesto para rotar (según el eje Y). Si 'grados' es positivo, se rota en el sentido
		// de las agujas del reloj y si es negativo, se rota en sentido contrario. Los grados
		// son sexagesimales.
		void rotar(int grados);

		// Gesto para avanzar/retroceder el año actual (actYear). Si 'year_inc' es positivo
		// se avanza ese número de años y si es negativo se retrocede ese número de años.
		// Hace falta comprobar después, que actYear está dentro del rango [1000, 2019].
		void cambiarAnio(int year_inc);

		void display(GLFWwindow* window);
		void drawCube();
		void drawLineaTemporal();
		void drawMarcaTemporal();
};

