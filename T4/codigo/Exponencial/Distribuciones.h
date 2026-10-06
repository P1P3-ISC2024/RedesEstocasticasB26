#ifndef DISTRIBUCIONES_H
#define DISTRIBUCIONES_H

#include <iostream>
#include <random>

class Distribuciones
{
private:
    int min;
    int max;
	double lambda; // Parámetro lambda de la distribución exponencial.
	double p;      // Probabilidad de éxito para la distribución Erlang, Hyper-exp y Coxiar.
    std::uniform_int_distribution<int> int_distribution_uniform;

    // Generador de números pseudoaleatorios.
    // Produce enteros de 32 bits entre 0 y 2^32 - 1.
    std::mt19937 generador;

public:
    // Inicializa los límites de la distribución.
    Distribuciones(double lambda, double p)
    {
        this->min = 0;
        this->max = 1000000;
        this->lambda = lambda;
        this->p = p;

        // Inicializa el generador con una semilla aleatoria.
        generador.seed(std::random_device{}());
        int_distribution_uniform = std::uniform_int_distribution<int>(this->min, this->max);
    }

    // Genera un número entero entre min y max.
    int generar()
    {
        // Define una distribución uniforme en [min, max].
        //std::uniform_int_distribution<int> distribucion(min, max);

        // Genera el número utilizando el generador.
        //return distribucion(generador);
		return int_distribution_uniform(generador);
    }

	// genera un número flotante entre 0 y 1.
    double u() {
        int u = this->generar();
        return static_cast<double>(u) / static_cast<double>(this->max+1);   // +1 por contar el 0.
    }

	// Utilizo la inversa de la función de distribución acumulada (CDF) de la distribución exponencial para generar un número aleatorio.
    double exp() {
        return -1.0f / this->lambda * log(1.0f - this->u());
    }

	// Sobrecarga de la función exp para permitir pasar un valor de lambda diferente al definido en el constructor.
    double exp(double lambda) {
        return -1.0f / lambda * log(1.0f - this->u());
    }

	// Genera un número aleatorio con distribución Erlang, que es la suma de k variables aleatorias exponenciales independientes.
	double erlang(unsigned int k) {
		double sum = 0.0f;
		for (unsigned int i = 0; i < k; i++) {
			sum += this->exp();
		}
		return sum;
	}

	// Genera un número aleatorio con distribución Hyper-exponencial, que es una mezcla de dos distribuciones exponenciales con diferentes tasas.
	double hyperExponencial() {
		double u = this->u();
		if (u < p) {                            // Utilliza el parametro lamda.
			return this->exp();
		}
		else {                                  // Genera con lambda = 1.5.
			return this->exp(1.5f);
		}
	}

	// Genera un número aleatorio con distribución Coxiar, similar a Erlang e Hyper-exponencial.
	double coxiar() {
        double sum = 0.0f;
        do{
            sum += this->exp();
        }while(this->u() < p);
        return sum;
	}
};

#endif