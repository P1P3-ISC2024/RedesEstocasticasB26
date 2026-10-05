#ifndef DISTRIBUCION_EXPONENCIAL_H
#define DISTRIBUCION_EXPONENCIAL_H

#include "DistribucionUniforme.h"

class DistribucionExponencial : public DistribucionUniforme
{
private:
	float lambda; // Parámetro lambda de la distribución exponencial.
public:
    // Inicializa la distribución exponencial utilizando
    // los límites definidos por la distribución uniforme.
    DistribucionExponencial(int min, int max, float lambda)
        : DistribucionUniforme(min, max){
        this->lambda = lambda;
    }

    // Genera un número utilizando actualmente
    // la implementación de la distribución uniforme.
    int generar()
    {
        return DistribucionUniforme::generar();
    }

	// Utilizo la inversa de la función de distribución acumulada (CDF) de la distribución exponencial para generar un número aleatorio.
	// La inversa me dará el valor x que correponde a la frecuencia acumulada u, que es un número aleatorio entre 0 y 1.
	// Así mismo la integral de la función de densidad de probabilidad (PDF) de la distribución exponencial (lambda * e^(-lambda * x))
	// me da la probabilidad de que un evento ocurra en un intervalo entre 0 y x.
    double exp() {
		double u = this->u();
		return -1.0f / lambda * log(1.0f - u);
    }
    double exp(double u) {
		return -1.0f / lambda * log(1.0f - u);
    }
};

#endif