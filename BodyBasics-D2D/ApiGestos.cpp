#include "stdafx.h"
#include "ApiGestos.h"

/* MANO */

void Mano::setEstado(HandState handState, TrackingState trackingState) {

	// Si la articulación está trackeada, la mano estará abierta o cerrada
	if (trackingState == TrackingState_Tracked) {
		if (handState == HandState_Closed) {
			estado = EstadoMano::CERRADA;
		}
		else {
			estado = EstadoMano::ABIERTA;
		}
	}
	else { // TrackingState es Inferred o Not_tracked
		estado = EstadoMano::NOT_TRACKED;
	}
}

void Mano::setPos(float pos_mano, float pos_cintura, float pos_cabeza, float alfa) {
	if (pos_mano < pos_cintura)
		pos = PosicionMano::ABAJO;
	else if (pos_mano < pos_cabeza + alfa)
		pos = PosicionMano::ENMEDIO;
	else
		pos = PosicionMano::ARRIBA;
}

/* DIAGRAMA ESTADOS */

void AutomataEstados::transicionEstado(Mano mano_izquierda, Mano mano_derecha) {
	// Si el estado de alguna de las dos manos es NOT_TRACKED, estado_actual pasa a ser ESTADO_INICIAL
	if (mano_izquierda.getEstado() == EstadoMano::NOT_TRACKED || mano_derecha.getEstado() == EstadoMano::NOT_TRACKED) {
		estado_actual = EstadoGestos::ESTADO_INICIAL;
		return;
	}

	// Transiciones desde el estado inicial
	if (estado_actual == EstadoGestos::ESTADO_INICIAL) {
		// Gesto desplazar
		if (mano_izquierda.getEstado() == EstadoMano::ABIERTA && mano_derecha.getEstado() == EstadoMano::CERRADA &&
			mano_derecha.getPos() == PosicionMano::ENMEDIO)
			estado_actual = EstadoGestos::GESTO_DESPLAZAR;

		// Gesto rotar
		else if (mano_izquierda.getEstado() == EstadoMano::CERRADA && mano_izquierda.getPos() == PosicionMano::ENMEDIO &&
			mano_derecha.getEstado() == EstadoMano::CERRADA && mano_derecha.getPos() == PosicionMano::ENMEDIO)
			estado_actual = EstadoGestos::GESTO_ROTAR;

		// Gesto zoom
		else if (mano_izquierda.getEstado() == EstadoMano::ABIERTA && mano_izquierda.getPos() == PosicionMano::ENMEDIO &&
			mano_derecha.getEstado() == EstadoMano::ABIERTA && mano_derecha.getPos() == PosicionMano::ENMEDIO)
			estado_actual = EstadoGestos::GESTO_ZOOM;

		// Gesto cambiar año
		else if (mano_izquierda.getEstado() == EstadoMano::ABIERTA && mano_derecha.getEstado() == EstadoMano::CERRADA &&
			mano_derecha.getPos() == PosicionMano::ARRIBA)
			estado_actual = EstadoGestos::GESTO_CAMBIAR_ANIO;
	}

	// Transiciones desde el estado Desplazar
	else if (estado_actual == EstadoGestos::GESTO_DESPLAZAR) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == EstadoMano::ABIERTA && mano_derecha.getEstado() == EstadoMano::CERRADA)) {
			estado_actual = EstadoGestos::ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}

	// Transiciones desde el estado Rotar
	else if (estado_actual == EstadoGestos::GESTO_ROTAR) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == EstadoMano::CERRADA && mano_derecha.getEstado() == EstadoMano::CERRADA)) {
			estado_actual = EstadoGestos::ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}

	// Transiciones desde el estado Zoom
	else if (estado_actual == EstadoGestos::GESTO_ZOOM) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == EstadoMano::ABIERTA && mano_derecha.getEstado() == EstadoMano::ABIERTA)) {
			estado_actual = EstadoGestos::ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}

	// Transiciones desde el estado Cambiar año
	else if (estado_actual == EstadoGestos::GESTO_CAMBIAR_ANIO) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == EstadoMano::ABIERTA && mano_derecha.getEstado() == EstadoMano::CERRADA)) {
			estado_actual = EstadoGestos::ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}
}