#pragma once

#include "stdafx.h"

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