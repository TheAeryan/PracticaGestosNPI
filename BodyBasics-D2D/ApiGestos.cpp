#include "stdafx.h"
#include "ApiGestos.h"
#include "model3D.h"
#include <cmath>


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

void Mano::setXYZ(CameraSpacePoint pos_camara) {
	X = pos_camara.X;
	Y = pos_camara.Y;
	Z = pos_camara.Z;
}

/* DIAGRAMA ESTADOS */

bool AutomataEstados::transicionEstado(Mano mano_izquierda, Mano mano_derecha) {
	EstadoGestos estado_previo = this->estado_actual; // Guardo el estado antes de aplicar la transición

	// Si el estado de alguna de las dos manos es NOT_TRACKED, estado_actual pasa a ser ESTADO_INICIAL
	if (mano_izquierda.getEstado() == EstadoMano::NOT_TRACKED || mano_derecha.getEstado() == EstadoMano::NOT_TRACKED) {
		estado_actual = EstadoGestos::ESTADO_INICIAL;

		return (estado_actual != estado_previo);
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

	EstadoGestos estado_nuevo = this->estado_actual; // Guardo el estado después de aplicar la transición

	// Devuelvo si ha cambiado el estado o no
	return (estado_nuevo != estado_previo);
}

/* ACCIONES GESTOS */

void AccionGestoDesplazar::continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva) {	
	// Calculo la distancia en el plano XY entre la posición nueva de la mano derecha y la antigua
	float desp_x = mano_der_nueva.getX() - mano_der.getX();
	float desp_y = mano_der_nueva.getY() - mano_der.getY();

	float desp_xy = sqrt(desp_x * desp_x + desp_y * desp_y);

	// Si el desplazamiento es mayor que el umbral, ejecuto la acción Desplazar de la interfaz
	// y guardo las nuevas posiciones de las manos. Si no, no hago nada.
	if (desp_xy > umbral_desp) {
		mano_izd = mano_izd_nueva;
		mano_der = mano_der_nueva;

		interfaz_grafica.desplazar(desp_x, desp_y);
	}
}

void AccionGestoCambiarAnio::continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva) {
	// Calculo la distancia en el eje X entre la posición nueva de la mano derecha y la antigua
	float desp_x = mano_der_nueva.getX() - mano_der.getX();

	// Si el desplazamiento (en valor absoluto) es mayor que el umbral, ejecuto la acción cambiarAnio de la interfaz
	// y guardo las nuevas posiciones de las manos. Si no, no hago nada.
	if (abs(desp_x) > umbral_desp) {
		mano_izd = mano_izd_nueva;
		mano_der = mano_der_nueva;

		interfaz_grafica.cambiarAnio(int(desp_x * factor_cambio_anio));
	}
}