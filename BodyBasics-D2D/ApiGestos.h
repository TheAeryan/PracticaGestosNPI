#pragma once

#include "stdafx.h"
#include "model3D.h"

#include <chrono>
using namespace std::chrono;


/* MANO */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Estado actual de la mano. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class EstadoMano{
	NOT_TRACKED,
	ABIERTA,
	CERRADA
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Posici�n actual de la mano. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class PosicionMano{
	ABAJO,
	ENMEDIO,
	ARRIBA
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Clase para guardar el estado y posici�n de la mano (izquierda o derecha). </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class Mano {
private:
	EstadoMano estado;
	PosicionMano pos;
	float X;
	float Y;
	float Z;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Constructor. El estado inicial de la mano es NOT_TRACKED y la posici�n, ABAJO.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Mano() : estado(EstadoMano::NOT_TRACKED), pos(PosicionMano::ABAJO) {}
	EstadoMano getEstado() { return estado; }
	PosicionMano getPos() { return pos; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Establece el estado de la mano a partir del estado seg�n la clase HandState.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="handState">		State of the hand. </param>
	/// <param name="trackingState">	State of the tracking. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void setEstado(HandState handState, TrackingState trackingState);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Establece la posici�n de la mano seg�n las posiciones (y) de la cintura, la cabeza y el valor
	/// alfa. Si pos_mano &lt; pos_cintura, pos=ABAJO. Si pos_mano &gt;= pos_cintura &amp;&amp;
	/// pos_mano &lt; pos_cabeza + alfa, pos=ENMEDIO. Si pos_mano &gt;= pos_cabeza + alfa, pos=ARRIBA.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="pos_mano">   	The position mano. </param>
	/// <param name="pos_cintura">	The position cintura. </param>
	/// <param name="pos_cabeza"> 	The position cabeza. </param>
	/// <param name="alfa">		  	The alfa. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void setPos(float pos_mano, float pos_cintura, float pos_cabeza, float alfa);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Establece la posici�n de la mano en el espacio de la c�mara. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="pos_camara">	The position camara. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void setXYZ(CameraSpacePoint pos_camara);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Devuelve la posici�n X. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <returns>	The x coordinate. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float getX() { return X; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Devuelve la posici�n Y. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <returns>	The y coordinate. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float getY() { return Y; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Devuelve la posici�n Z. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <returns>	The z coordinate. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float getZ() { return Z; }
};

/* DIAGRAMA ESTADOS */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Estado actual del diagrama de estados de los gestos. Seg�n este estado, se llevar� a cabo un
/// gesto u otro, o ninguno.
/// </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class EstadoGestos{
	ESTADO_INICIAL, // Estado donde no se realiza ning�n gesto (estado de espera).
	GESTO_DESPLAZAR,
	GESTO_ROTAR,
	GESTO_ZOOM,
	GESTO_CAMBIAR_ANIO
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Clase que guarda el estado actual del sistema y realiza las transiciones seg�n las nuevas
/// posiciones y estados de las manos.
/// </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AutomataEstados {
private:
	EstadoGestos estado_actual;
	time_point<high_resolution_clock> ultimo_tiempo;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. El estado inicial del sistema es ESTADO_INICIAL. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AutomataEstados() : estado_actual(EstadoGestos::ESTADO_INICIAL) { ultimo_tiempo = high_resolution_clock::now(); }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Devuelve el estado actual del sistema. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <returns>	The estado. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	EstadoGestos getEstado() { return estado_actual; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Ejecuta la transici�n dada por el diagrama de estados, cambiando el estado del sistema si es
	/// necesario. Devuelve true si el estado ha cambiado y false, si el estado es el mismo.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izquierda">	The mano izquierda. </param>
	/// <param name="mano_derecha">  	The mano derecha. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool transicionEstado(Mano mano_izquierda, Mano mano_derecha);
};

/* ACCIONES GESTOS */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Clase abstracta de la que heredan las clases que implementan los gestos y que define su
/// funcionalidad com�n.
/// </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AccionGesto{
protected:

	/// <summary>	La mano izquierda. </summary>
	Mano mano_izd;

	/// <summary>	La mano derecha. </summary>
	Mano mano_der;
	
	/// <summary>	La interfaz grafica. </summary>
	model3D* interfaz_grafica;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor por defecto. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AccionGesto() {}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Constructor. Se llama cuando se inicial el gesto y guarda las posiciones iniciales de las
	/// manos, as� como una puntero a la interfaz gr�fica.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_">			The mano izd. </param>
	/// <param name="mano_der_">			The mano der. </param>
	/// <param name="interfaz_grafica_">	[in,out] If non-null, the interfaz grafica. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AccionGesto(Mano mano_izd_, Mano mano_der_, model3D* interfaz_grafica_) :
		mano_izd(mano_izd_), mano_der(mano_der_), interfaz_grafica(interfaz_grafica_) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// M�todo llamado para ejecutar la acci�n del gesto, una vez que ya ha empezado. Se compara la
	/// informaci�n nueva de las manos con la informaci�n previa, guardada en la clase.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_nueva">	The mano izd nueva. </param>
	/// <param name="mano_der_nueva">	The mano der nueva. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Clase que implementa la funcionalidad asociada al gesto GESTO_DESPLAZAR. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AccionGestoDesplazar : public AccionGesto {
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Umbral de desplazamiento. Cuanto menor sea este umbral, mayor ser� la sensibilidad del gesto
	/// al movimiento de las manos.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float umbral_desp = 0.005; 
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Constructor. Se llama cuando se inicial el gesto y guarda las posiciones iniciales de las
	/// manos, as� como una puntero a la interfaz gr�fica.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_">			The mano izd. </param>
	/// <param name="mano_der_">			The mano der. </param>
	/// <param name="interfaz_grafica_">	[in,out] If non-null, the interfaz grafica. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AccionGestoDesplazar(Mano mano_izd_, Mano mano_der_, model3D* interfaz_grafica_) :
		AccionGesto(mano_izd_, mano_der_, interfaz_grafica_) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// M�todo llamado para ejecutar la acci�n del gesto, una vez que ya ha empezado. Se compara la
	/// posici�n de la mano derecha actual y anterior y si el desplazamiento en el plano XY no supera
	/// un umbral (umbral_desp), no se tiene en cuenta.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_nueva">	The mano izd nueva. </param>
	/// <param name="mano_der_nueva">	The mano der nueva. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Clase que implementa la funcionalidad asociada al gesto GESTO_CAMBIAR_ANIO.
/// </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AccionGestoCambiarAnio : public AccionGesto {
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Umbral de desplazamiento. Cuanto menor sea este umbral, mayor ser� la sensibilidad del gesto
	/// al movimiento de las manos.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float umbral_desp = 0.00;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Factor por el que se multiplica el desplazamiento en X de la mano derecha para cambiar el a�o
	/// actual. Cuanto mayor sea este valor, mayor ser� el cambio en a�o para el mismo movimiento con
	/// la mano.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int factor_cambio_anio = 3000;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Constructor. Se llama cuando se inicial el gesto y guarda las posiciones iniciales de las
	/// manos, as� como una puntero a la interfaz gr�fica.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_">			The mano izd. </param>
	/// <param name="mano_der_">			The mano der. </param>
	/// <param name="interfaz_grafica_">	[in,out] If non-null, the interfaz grafica. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AccionGestoCambiarAnio(Mano mano_izd_, Mano mano_der_, model3D* interfaz_grafica_) :
		AccionGesto(mano_izd_, mano_der_, interfaz_grafica_) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// M�todo llamado para ejecutar la acci�n del gesto, una vez que ya ha empezado. Se compara la
	/// posici�n de la mano derecha actual y anterior y si el desplazamiento en el eje X no supera un
	/// umbral (umbral_desp), no se tiene en cuenta.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_nueva">	The mano izd nueva. </param>
	/// <param name="mano_der_nueva">	The mano der nueva. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Clase que implementa la funcionalidad asociada al gesto GESTO_ZOOM. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AccionGestoZoom : public AccionGesto {
	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Umbral de separaci�n, ajusta como de lejos se pueden alejar las manos del plano XY.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float umbral_separacion = 0.2;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Umbral de distancia. Cuanto m�s valga, menos tienen que ir a la par moviendose las manos.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float umbral_distancia = 0.3;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Umbral de acci�n. Cuanto m�s valga, m�s tiene que cambiar la distancia para que se realize el
		/// gesto.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float umbral_accion = 0.00;

		/// <summary>	El centro, punto medio de la posici�n inicial de las manos (eje X) </summary>
		float centro_x;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calcula la distancia 1 (valor absoluto de la diferencia) </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="p1">	The first float. </param>
		/// <param name="p2">	The second float. </param>
		///
		/// <returns>	A float. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float distancia(float p1, float p2);

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Constructor. Se llama cuando se inicial el gesto y guarda las posiciones iniciales de las
		/// manos, as� como una puntero a la interfaz gr�fica. Toma el punto medio en el eje X de las
		/// manos.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="mano_izd_">			The mano izd. </param>
		/// <param name="mano_der_">			The mano der. </param>
		/// <param name="interfaz_grafica_">	[in,out] If non-null, the interfaz grafica. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		AccionGestoZoom(Mano mano_izd_, Mano mano_der_, model3D* interfaz_grafica_);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// M�todo para continuar el gesto. Comprueba que las manos no se separen mucho en el plano YZ y
		/// que la distancia que separan las manos del centro se separan lo mismo. Actualiza el valor de
		/// escala del modelo.
		/// </summary>
		///
		/// <remarks>	Mike, 19/12/2019. </remarks>
		///
		/// <param name="mano_izd_nueva">	The mano izd nueva. </param>
		/// <param name="mano_der_nueva">	The mano der nueva. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva);

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Clase que implementa la funcionalidad asociada al gesto GESTO_ZOOM. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AccionGestoRotar : public AccionGesto {
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Umbral de separaci�n, ajusta como de lejos se pueden alejar las manos del eje Y.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float umbral_separacion = 0.2;

	/// <summary>
	// Umbral del �ngulo que forman las manos en la circunferencia. Cuanto m�s
	/// vale, m�s pueden ser diferentes los �ngulos que se forman.
	/// </summary>
	/// <summary>	The umbral angulo. </summary>
	float umbral_angulo = 0.12;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Umbral de grados m�nimos para aplicar la rotaci�n, la sensibilidad, cuanto m�s alto m�s
	/// �ngulo debe formarse para aplicar la acci�n del gesto.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float umbral_grados = 1.0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Umbral de la circunferencia, cuanto m�s alto m�s se pueden alejar las manos de la
	/// circunferencia que se considera.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float umbral_circunferencia = 0.2;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representa un punto en el plano XZ. </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct Punto {
		float x, z;
	};

	/// <summary>	El centro de la circunferencia, punto medio en el plano XZ. </summary>
	Punto centro;

	/// <summary>	Radio de la circunferencia. </summary>
	float radio;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Calcula la distancia 1 (valor absoluto de la diferencia) </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="p1">	The first float. </param>
	/// <param name="p2">	The second float. </param>
	///
	/// <returns>	A float. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float distancia(float p1, float p2);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Calcula la distancia 2 (valor euclideo) </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="x1">	The first x value. </param>
	/// <param name="y1">	The first y value. </param>
	/// <param name="x2">	The second x value. </param>
	/// <param name="y2">	The second y value. </param>
	///
	/// <returns>	A float. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float distancia2D(float x1, float y1, float x2, float y2);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Devuelve si la mano que se le pasa est� fuera de la circunferencia con un cierto umbral.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano">	The mano. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool manoFuera(Mano mano);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Devuewlve el �ngulo que forman dos manos, se usa con la posici�n de la mano anterior y la
	/// nueva para calcular el �ngulo que ha formado una mano.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano1">	The first mano. </param>
	/// <param name="mano2">	The second mano. </param>
	///
	/// <returns>	A float. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float angulo(Mano mano1, Mano mano2);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Constructor. Inicializamos las manos iniciales, la interfaz gr�fica, y se establece el centro
	/// XZ con el punto medio en el plano XZ (centro de circunferencia)
	/// y el radio.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_">			The mano izd. </param>
	/// <param name="mano_der_">			The mano der. </param>
	/// <param name="interfaz_grafica_">	[in,out] If non-null, the interfaz grafica. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AccionGestoRotar(Mano mano_izd_, Mano mano_der_, model3D* interfaz_grafica_);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// M�todo para continuar el gesto. Se comprueba que las manos no se salgan del eje Y, est�n en
	/// la circunferencia, formen el mismo �ngulo y en sentido opuesto entre ellos. Si se cumple todo
	/// se rota el objeto, conforme el �ngulo rotado.
	/// </summary>
	///
	/// <remarks>	Mike, 19/12/2019. </remarks>
	///
	/// <param name="mano_izd_nueva">	The mano izd nueva. </param>
	/// <param name="mano_der_nueva">	The mano der nueva. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva);
};