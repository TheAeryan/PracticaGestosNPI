#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A model 3d. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class model3D {
	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A�o actual. En funci�n de este a�o se mostrar� el modelo de una forma u otra.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		int actYear;
		/// <summary>	A�o m�nimo que puede valer actYear. </summary>
		int minYear = 1000;
		/// <summary>	A�o m�ximo que puede valer actYear. </summary>
		int maxYear = 2019;
		/// <summary>	�ngulo de rotaci�n del modelo. </summary>
		float ang_rotacion = 0;
		/// <summary>	Factor de escala del modelo. </summary>
		float escala = 1.0;
		/// <summary>	Desplazamiento en el eje X de la marca de la l�nea temporal. </summary>
		float desplazamiento_marca = 0.0;
		/// <summary>	Desplazamiento en el eje X del modelo. </summary>
		float desplazamiento_x = 0.0;
		/// <summary>	Desplazamiento en el eje Y del modelo. </summary>
		float desplazamiento_y = 0.0;
		/// <summary>	Valor R del color del modelo. </summary>
		float r = 1.0;
		/// <summary>	Valor G del color del modelo. </summary>
		float g = 1.0;
		/// <summary>	Valor B del color del modelo. </summary>
		float b = 1.0;
		
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		model3D() : actYear(1510) { }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Establece el color actual del modelo. </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="r">	A float to process. </param>
		/// <param name="g">	A float to process. </param>
		/// <param name="b">	A float to process. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void setColor(float r, float g, float b);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gesto de mover el modelo. Se puede mover en el eje x e y. </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="desp_x">	The desp x coordinate. </param>
		/// <param name="desp_y">	The desp y coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void desplazar(float desp_x, float desp_y);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Gesto de agrandar/achicar (hacer zoom). Si 'factor' est� entre 0 y 1, se achica y si es mayor
		/// que 1 se agranda seg�n 'factor'.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="factor">	The factor. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void zoom(float factor);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Gesto para rotar (seg�n el eje Y). Si 'grados' es positivo, se rota en el sentido de las
		/// agujas del reloj y si es negativo, se rota en sentido contrario. Los grados son sexagesimales.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="grados">	The grados. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void rotar(int grados);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Gesto para avanzar/retroceder el a�o actual (actYear). Si 'year_inc' es positivo se avanza
		/// ese n�mero de a�os y si es negativo se retrocede ese n�mero de a�os. Hace falta comprobar
		/// despu�s, que actYear est� dentro del rango [1000, 2019].
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="year_inc">	The year increment. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void cambiarAnio(int year_inc);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Imprime en la ventana que se le pasa la escena de los objetos. Se colorea el fondo de verde,
		/// el objeto, la l�nea temporal y la marca temporal.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="window">	[in,out] If non-null, the window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void display(GLFWwindow* window);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Dibuja el objeto en la escena, y le aplica las transformaciones correspondientes seg�n los
		/// valores que tenga guardados actualmente.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void drawObject();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Dibuja la l�nea temporal, un rect�ngulo por la parte de arriba de la ventana.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void drawLineaTemporal();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Dibuja la marca temporal, que empieza en el centro de la l�nea temporal. </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void drawMarcaTemporal();
};

