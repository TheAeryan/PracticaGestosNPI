#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Esta clase se corresponde con el modelo 3D de la Alhambra
// Se deben implementar todos estos métodos, que serán llamados
// como consecuencia de gestos

class model3D {
	private:
		int actYear; // Año actual. En función de este año se mostrará el modelo de una forma u otra.
		int minYear = 1000; // Año mínimo que puede valer actYear.
		int maxYear = 2019; // Año máximo que puede valer actYear.
		float ang_rotacion = 0; // Ángulo de rotación del modelo.
		float escala = 1.0; // Factor de escala del modelo.
		float desplazamiento_marca = 0.0; // Desplazamiento en el eje X de la marca de la línea temporal.
		float desplazamiento_x = 0.0; // Desplazamiento en el eje X del modelo.
		float desplazamiento_y = 0.0; // Desplazamiento en el eje Y del modelo.
		float r = 1.0; // Valor R del color del modelo
		float g = 1.0; // Valor G del color del modelo
		float b = 1.0; // Valor B del color del modelo
		
	public:

		// Constructor
		model3D() : actYear(1510) { }

		// Establece el color actual del modelo
		void setColor(float r, float g, float b);

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

		// Imprime en la ventana que se le pasa la escena de los objetos. Se colorea el fondo
		// de verde, el objeto, la línea temporal y la marca temporal.
		void display(GLFWwindow* window);

		// Dibuja el objeto en la escena, y le aplica las transformaciones correspondientes
		// según los valores que tenga guardados actualmente.
		void drawObject();

		// Dibuja la línea temporal, un rectángulo por la parte de arriba de la ventana.
		void drawLineaTemporal();

		// Dibuja la marca temporal, que empieza en el centro de la línea temporal.
		void drawMarcaTemporal();
};

