#pragma once

#include "BodyBasics.h"

/* MANO */

/// <summary>
/// Estado actual de la mano.
/// </summary>
enum EstadoMano{
	NOT_TRACKED=0,
	ABIERTA=1,
	CERRADA=2
};

/// <summary>
/// Posición actual de la mano.
/// </summary>
enum PosicionMano{
	ABAJO=0,
	ENMEDIO=1,
	ARRIBA=2
};

/// <summary>
/// Clase para guardar el estado y posición de la mano (izquierda o derecha).
/// </summary>
class Mano {
private:
	EstadoMano estado;
	PosicionMano pos;
public:
	/// <summary>
	/// Constructor. El estado inicial de la mano es NOT_TRACKED y la posición, ABAJO.
	/// </summary>
	Mano() : estado(NOT_TRACKED), pos(ABAJO) {}
	EstadoMano getEstado() { return estado; }
	PosicionMano getPos() { return pos; }

	/// <summary>
	/// Establece el estado de la mano a partir del estado según la clase HandState.
	/// </summary>
	void setEstado(HandState handState);

	/// <summary>
	/// Establece la posición de la mano según las posiciones (y) de la cintura,
	/// la cabeza y el valor alfa.
	/// Si pos_mano < pos_cintura, pos=ABAJO.
	/// Si pos_mano >= pos_cintura && pos_mano < pos_cabeza + alfa, pos=ENMEDIO.
	/// Si pos_mano >= pos_cabeza + alfa, pos=ARRIBA.
	/// </summary>
	void setPos(float pos_mano, float pos_cintura, float pos_cabeza, float alfa);
};

/* DIAGRAMA ESTADOS */

/// <summary>
/// Estado actual del diagrama de estados de los gestos. Según este estado, se llevará a cabo un gesto u otro, o ninguno.
/// </summary>
enum EstadoGestos{
	ESTADO_INICIAL=0, // Estado donde no se realiza ningún gesto (estado de espera).
	GESTO_DESPLAZAR=1,
	GESTO_ROTAR=2,
	GESTO_ZOOM=3,
	GESTO_CAMBIAR_ANIO=4
};

/// <summary>
/// Clase que guarda el estado actual del sistema y realiza las transiciones según las nuevas posiciones y estados de las manos.
/// </summary>
class AutomataEstados {
private:
	EstadoGestos estado_actual;
public:
	/// <summary>
	/// Constructor. El estado inicial del sistema es ESTADO_INICIAL.
	/// </summary>
	AutomataEstados() : estado_actual(ESTADO_INICIAL) {}

	/// <summary>
	/// Devuelve el estado actual del sistema.
	/// </summary>
	EstadoGestos getEstado() { return estado_actual; }

	/// <summary>
	/// Ejecuta la transición dada por el diagrama de estados, cambiando el estado del sistema si es necesario.
	/// </summary>
	void transicionEstado(Mano mano_izquierda, Mano mano_derecha);
};