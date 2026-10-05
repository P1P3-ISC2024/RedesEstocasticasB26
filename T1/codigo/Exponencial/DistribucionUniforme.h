#ifndef DISTRIBUCION_UNIFORME_H
#define DISTRIBUCION_UNIFORME_H

#include <iostream>
#include <random>

class DistribucionUniforme
{
private:
    int min;
    int max;

    // Generador de números pseudoaleatorios.
    // Produce enteros de 32 bits entre 0 y 2^32 - 1.
    std::mt19937 generador;

public:
    // Inicializa los límites de la distribución.
    DistribucionUniforme(int min, int max)
    {
        this->min = min;
        this->max = max;

        // Inicializa el generador con una semilla aleatoria.
        generador.seed(std::random_device{}());
    }

    // Genera un número entero entre min y max.
    int generar()
    {
        // Define una distribución uniforme en [min, max].
        std::uniform_int_distribution<int> distribucion(min, max);

        // Genera el número utilizando el generador.
        return distribucion(generador);
    }

	// genera un número flotante entre 0 y 1.
    double u() {
        int u = this->generar();
        return static_cast<double>(u) / static_cast<double>(this->max+1);
    }

    /*double u(bool flotante)
    {
        // Distribución uniforme de números reales en [0, 1).
        std::uniform_real_distribution<double> distribucion(0.0, 1.0);

        return distribucion(generador);
    }*/
};

#endif