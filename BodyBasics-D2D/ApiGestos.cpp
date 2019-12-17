#include "stdafx.h"
#include "ApiGestos.h"
#include "model3D.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265


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

float AccionGestoZoom::distancia(float p1, float p2) { return abs(p1 - p2); }

AccionGestoZoom::AccionGestoZoom(Mano mano_izd_, Mano mano_der_, model3D& interfaz_grafica_) : AccionGesto(mano_izd_, mano_der_, interfaz_grafica_) {
	// Obtenemos el centro inicial (punto medio de las manos)
	centro.x = (mano_izd_.getX() + mano_der_.getX()) / 2;
	centro.y = (mano_izd_.getY() + mano_der_.getY()) / 2;
}

void AccionGestoZoom::continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva) {
	// Calculamos la separación de las manos respecto cada eje
	float separacion_Z = distancia(mano_izd_nueva.getZ(), mano_der_nueva.getZ());
	float separacion_Y = distancia(mano_izd_nueva.getY(), mano_der_nueva.getY());
	float distancia_X_izq = distancia(mano_izd_nueva.getX(), centro.x);
	float distancia_X_der = distancia(mano_der_nueva.getX(), centro.x);

	// Las manos deben estar en la misma recta del eje X
	if (separacion_Z <= umbral_separacion && separacion_Y <= umbral_separacion && distancia(distancia_X_izq, distancia_X_der) < umbral_distancia) {
		float cambio_distancia = distancia(mano_izd_nueva.getX(), centro.x) - distancia(mano_izd.getX(), centro.x);
		if (cambio_distancia > umbral_accion) {
			mano_izd = mano_izd_nueva;
			mano_der = mano_der_nueva;
			interfaz_grafica.zoom(cambio_distancia);
		}
	}
	else {
		mano_izd = mano_izd_nueva;
		mano_der = mano_der_nueva;
	}

}

float AccionGestoRotar::distancia(float p1, float p2) { return abs(p1 - p2); }

float AccionGestoRotar::distancia2D(float x1, float y1, float x2, float y2) {
	float dx = x1 - x2;
	float dy = y1 - y2;
	return sqrt(dx * dx + dy * dy);
}

AccionGestoRotar::AccionGestoRotar(Mano mano_izd_, Mano mano_der_, model3D& interfaz_grafica_) :
	AccionGesto(mano_izd_, mano_der_, interfaz_grafica_) {
	centro.x = (mano_izd_.getX() + mano_der_.getX()) / 2;
	centro.z = (mano_izd_.getZ() + mano_der_.getZ()) / 2;
	radio = distancia2D(mano_izd_.getX(), mano_izd_.getZ(), centro.x, centro.z);
}

bool AccionGestoRotar::AccionGestoRotar::manoFuera(Mano mano) {
	float radio_nuevo = distancia2D(mano.getX(), mano.getZ(), centro.x, centro.z);
	return distancia(radio_nuevo, radio) > umbral_circunferencia;
}

float AccionGestoRotar::angulo(Mano mano1, Mano mano2) {
	float distancia_cuadrado = distancia2D(mano1.getX(), mano1.getZ(), mano2.getX(), mano2.getZ()) * 
		distancia2D(mano1.getX(), mano1.getZ(), mano2.getX(), mano2.getZ());
	return acos(1 - (distancia_cuadrado / (2 * radio * radio)));
}

void AccionGestoRotar::continuarGesto(Mano mano_izd_nueva, Mano mano_der_nueva) {

	float separacion_Y = distancia(mano_izd_nueva.getY(), mano_der.getY());

	bool dentroY = separacion_Y <= umbral_separacion;
	bool manosDentro = !manoFuera(mano_izd_nueva) && !manoFuera(mano_der_nueva);
	bool mismosAngulos = distancia(angulo(mano_izd_nueva, mano_izd), angulo(mano_der_nueva, mano_der)) <= umbral_angulo;
	bool direccionContraria = (mano_izd_nueva.getZ() - mano_izd.getZ()) * (mano_der_nueva.getZ() - mano_der.getZ()) <= 0;



	if (dentroY && manosDentro && mismosAngulos && direccionContraria) {
		int angulo_grad = angulo(mano_izd_nueva, mano_izd) * 180.0 / PI;
		if (angulo_grad >= umbral_grados) {

			if (mano_izd_nueva.getZ() - mano_izd.getZ() < 0) 
				angulo_grad = -angulo_grad;

			mano_izd = mano_izd_nueva;
			mano_der = mano_der_nueva;
			
			interfaz_grafica.rotar(angulo_grad);
		}
	}
	else {
		mano_izd = mano_izd_nueva;
		mano_der = mano_der_nueva;
	}
}
