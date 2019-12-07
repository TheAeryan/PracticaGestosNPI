#pragma once

#include "stdafx.h"
#include "model3D.h"

/* MANO */

/// <summary>
/// Estado actual de la mano.
/// </summary>
enum class EstadoMano{
	NOT_TRACKED,
	ABIERTA,
	CERRADA
};

/// <summary>
/// Posici�n actual de la mano.
/// </summary>
enum class PosicionMano{
	ABAJO,
	ENMEDIO,
	ARRIBA
};

/// <summary>
/// Clase para guardar el estado y posici�n de la mano (izquierda o derecha).
/// </summary>
class Mano {
private:
	EstadoMano estado;
	PosicionMano pos;
	float X;
	float Y;
	float Z;
public:
	/// <summary>
	/// Constructor. El estado inicial de la mano es NOT_TRACKED y la posici�n, ABAJO.
	/// </summary>
	Mano() : estado(EstadoMano::NOT_TRACKED), pos(PosicionMano::ABAJO) {}
	EstadoMano getEstado() { return estado; }
	PosicionMano getPos() { return pos; }

	/// <summary>
	/// Establece el estado de la mano a partir del estado seg�n la clase HandState.
	/// </summary>
	void setEstado(HandState handState, TrackingState trackingState);

	/// <summary>
	/// Establece la posici�n de la mano seg�n las posiciones (y) de la cintura,
	/// la cabeza y el valor alfa.
	/// Si pos_mano < pos_cintura, pos=ABAJO.
	/// Si pos_mano >= pos_cintura && pos_mano < pos_cabeza + alfa, pos=ENMEDIO.
	/// Si pos_mano >= pos_cabeza + alfa, pos=ARRIBA.
	/// </summary>
	void setPos(float pos_mano, float pos_cintura, float pos_cabeza, float alfa);

	/// <summary>
	/// Establece la posici�n de la mano en el espacio de la c�mara.
	/// </summary>
	void setXYZ(CameraSpacePoint pos_camara);

	/// <summary>
	/// Devuelve la posici�n X.
	/// </summary>
	float getX() { return X; }

	/// <summary>
	/// Devuelve la posici�n Y.
	/// </summary>
	float getY() { return Y; }

	/// <summary>
	/// Devuelve la posici�n Z.
	/// </summary>
	float getZ() { return Z; }
};

/* DIAGRAMA ESTADOS */

/// <summary>
/// Estado actual del diagrama de estados de los gestos. Seg�n este estado, se llevar� a cabo un gesto u otro, o ninguno.
/// </summary>
enum class EstadoGestos{
	ESTADO_INICIAL, // Estado donde no se realiza ning�n gesto (estado de espera).
	GESTO_DESPLAZAR,
	GESTO_ROTAR,
	GESTO_ZOOM,
	GESTO_CAMBIAR_ANIO
};

/// <summary>
/// Clase que guarda el estado actual del sistema y realiza las transiciones seg�n las nuevas posiciones y estados de las manos.
/// </summary>
class AutomataEstados {
private:
	EstadoGestos estado_actual;
public:
	/// <summary>
	/// Constructor. El estado inicial del sistema es ESTADO_INICIAL.
	/// </summary>
	AutomataEstados() : estado_actual(EstadoGestos::ESTADO_INICIAL) {}

	/// <summary>
	/// Devuelve el estado actual del sistema.
	/// </summary>
	EstadoGestos getEstado() { return estado_actual; }

	/// <summary>
	/// Ejecuta la transici�n dada por el diagrama de estados, cambiando el estado del sistema si es necesario.
	/// </summary>
	void transicionEstado(Mano mano_izquierda, Mano mano_derecha);
};

/* ACCIONES GESTOS */

/// <summary>
/// Clase abstracta de la que heredan las clases que implementan los gestos y que define su funcionalidad com�n.
/// </summary>
class AccionGesto{
protected:
	Mano mano_izd;
	Mano mano_der;
	model3D interfaz_grafica;
public:

	/// <summary>
	/// Constructor. Se llama cuando se inicial el gesto y guarda las posiciones iniciales de las manos,
	/// as� como una referencia a la interfaz gr�fica.
	/// </summary>
	AccionGesto(Mano mano_izd_, Mano mano_der_, model3D& interfaz_grafica_) :
		mano_izd(mano_izd_), mano_der(mano_der_), interfaz_grafica(interfaz_grafica_) {}

	/// <summary>
	/// M�todo llamado para ejecutar la acci�n del gesto, una vez que ya ha empezado.
	/// Se compara la informaci�n nueva de las manos con la informaci�n previa, guardada en la clase.
	/// </summary>
	virtual void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva);
};

/// <summary>
/// Clase que implementa la funcionalidad asociada al gesto GESTO_DESPLAZAR.
/// </summary>
class AccionGestoDesplazar :  AccionGesto {
private:
	float umbral_desp = 0;
public:
	/// <summary>
	/// Constructor. Se llama cuando se inicial el gesto y guarda las posiciones iniciales de las manos,
	/// as� como una referencia a la interfaz gr�fica.
	/// </summary>
	AccionGestoDesplazar(Mano mano_izd_, Mano mano_der_, model3D& interfaz_grafica_) :
		AccionGesto(mano_izd_, mano_der_, interfaz_grafica_) {}

	/// <summary>
	/// M�todo llamado para ejecutar la acci�n del gesto, una vez que ya ha empezado.
	/// Se compara la posici�n de la mano derecha actual y anterior y si el desplazamiento
	/// en el plano XY no supera un umbral (umbral_desp), no se tiene en cuenta.
	/// </summary>
	void continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva);
};