#include "ApiGestos.h"

#include "BodyBasics.h"

/* MANO */

void Mano::setEstado(HandState handState) {
	if (handState == HandState_Open) {
		estado = ABIERTA;
	}
	else if (handState == HandState_Closed) {
		estado = CERRADA;
	}
	else {
		estado = NOT_TRACKED;
	}
}

void Mano::setPos(float pos_mano, float pos_cintura, float pos_cabeza, float alfa) {
	if (pos_mano < pos_cintura)
		pos = ABAJO;
	else if (pos_mano < pos_cabeza + alfa)
		pos = ENMEDIO;
	else
		pos = ARRIBA;
}

/* DIAGRAMA ESTADOS */

void AutomataEstados::transicionEstado(Mano mano_izquierda, Mano mano_derecha) {
	// Si el estado de alguna de las dos manos es NOT_TRACKED, estado_actual pasa a ser ESTADO_INICIAL
	if (mano_izquierda.getEstado() == NOT_TRACKED || mano_derecha.getEstado() == NOT_TRACKED) {
		estado_actual = ESTADO_INICIAL;
		return;
	}

	// Transiciones desde el estado inicial
	if (estado_actual == ESTADO_INICIAL) {
		// Gesto desplazar
		if (mano_izquierda.getEstado() == ABIERTA && mano_derecha.getEstado() == CERRADA &&
			mano_derecha.getPos() == ENMEDIO)
			estado_actual = GESTO_DESPLAZAR;

		// Gesto rotar
		else if (mano_izquierda.getEstado() == CERRADA && mano_izquierda.getPos() == ENMEDIO &&
			mano_derecha.getEstado() == CERRADA && mano_derecha.getPos() == ENMEDIO)
			estado_actual = GESTO_ROTAR;

		// Gesto zoom
		else if (mano_izquierda.getEstado() == ABIERTA && mano_izquierda.getPos() == ENMEDIO &&
			mano_derecha.getEstado() == ABIERTA && mano_derecha.getPos() == ENMEDIO)
			estado_actual = GESTO_ZOOM;

		// Gesto cambiar año
		else if (mano_izquierda.getEstado() == ABIERTA && mano_derecha.getEstado() == CERRADA &&
			mano_derecha.getPos() == ARRIBA)
			estado_actual = GESTO_CAMBIAR_ANIO;
	}

	// Transiciones desde el estado Desplazar
	else if (estado_actual == GESTO_DESPLAZAR) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == ABIERTA && mano_derecha.getEstado() == CERRADA)) {
			estado_actual = ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}

	// Transiciones desde el estado Rotar
	else if (estado_actual == GESTO_ROTAR) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == CERRADA && mano_derecha.getEstado() == CERRADA)) {
			estado_actual = ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}

	// Transiciones desde el estado Zoom
	else if (estado_actual == GESTO_ZOOM) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == ABIERTA && mano_derecha.getEstado() == ABIERTA)) {
			estado_actual = ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}

	// Transiciones desde el estado Cambiar año
	else if (estado_actual == GESTO_CAMBIAR_ANIO) {
		// Veo si una de las dos manos ha cambiado de estado (la posición me da igual). En ese caso, paso al estado inicial.
		if (!(mano_izquierda.getEstado() == ABIERTA && mano_derecha.getEstado() == CERRADA)) {
			estado_actual = ESTADO_INICIAL;

			// Vuelvo a aplicar una transición en caso de ser necesario
			transicionEstado(mano_izquierda, mano_derecha);
		}
	}
}