/*
* Programador: Felipe de Jesús Martínez Alfaro.
* Fecha: 2024-06-10
* 
* Descripción: Programa principal para generar números aleatorios con distribuciones exponenciales.
* Mandar a llamar desde la terminal con los siguientes parámetros:
* main.exe [tipo de distribución] [cantidad] [tope] [lambda] [p] [k]
*					  1				  2			3		4	  5   6
*		   [tipo de distribución]... 1: exponencial, 2: Erlang, 3: Hyper-exp, 4: Coxiar.
*/

#include "Distribuciones.h"
#include "Excel.h"
#include "chi-test.h"

int main(int argc, char* argv[])
{
	// Parametros y variables necesarias para generar los números aleatorios...
	int f = argc > 1 ? std::stoi(argv[1]) : 1;				// Tipo de distribución a utilizar. Por defecto es 1 (exponencial).
	unsigned int cantidad= argc > 2? std::stoi(argv[2]) : 10000;	// Cantidad de números a generar. Por defecto es 1.
	unsigned int inicio = 0;								// Inicio del histograma. Siempre es 0.
	unsigned int tope = argc > 3 ? std::stoi(argv[3]) : 3;	// Si se pasa un tercer argumento, se toma como el tope del histograma. Sino, el tope es 3.
	tope *= 10;												// Multiplica el tope por 10 para que el histograma tenga un rango de 0 a 30 en lugar de 0 a 3.
	float lambda = argc > 4 ? std::stof(argv[4]) : 4.0f;	// Parámetro lambda de la distribución exponencial. Por defecto es 2.
	double p = argc > 5 ? std::stof(argv[5]) : 0.5f;		// Probabilidad de éxito para la distribución Erlang, Hyper-exp y Coxiar. Por defecto es 0.5.
	unsigned int k = argc > 6 ? std::stoi(argv[6]) : 2;		// Número de eventos para la distribución Erlang. Por defecto es 2.
	Distribuciones distribucion(lambda,p);					// Inicializa la distribución exponencial.
	double numero;											// Guarda el número que se genera.
	unsigned int rango;										// Guarda el rango del número que se genera.
	double q;												// Guarda el valor de q que se genera, para Chi-square test.

	// Indico que función de distribución se va a utilizar...
	switch (f) {
		case(2):
			std::cout << "Distribucion: Erlang. k = " << k << std::endl;
			break;
		case(3):
			std::cout << "Distribucion: Hyper-exponencial." << std::endl;
			break;
		case(4):
			std::cout << "Distribucion: Coxiar." << std::endl;
			break;
		default:
			std::cout << "Distribucion: Exponencial." << std::endl;
	}

	// Genera el histograma...
	std::cout << "Generando " << cantidad << " numeros con lambda = " << lambda << std::endl;
	if (f > 1) {
		std::cout << "p = " << p << std::endl;
	}
	std::vector<int> histograma(tope);						// Conteo de numeros para el histograma. 0.1 = 1 y 3.0 = 30.
	std::cout << "Maximo numero en el histograma: " << tope/10 << std::endl;
	for (unsigned int i = inicio; i <= tope; i++) {
		histograma[i] = 0;									// Inicializa el histograma en 0s.
	}

	// Genera los números aleatorios y los guarda en el histograma...
	for (unsigned int i = 0; i < cantidad; i++) {			// Genera los números de acuerdo a la cantidad.
		switch (f) {										// Selecciona la distribución a utilizar.
			case(2):
				numero = distribucion.erlang(k);			// Genera el número con una distribución Erlang.
				break;
			case(3):
				numero = distribucion.hyperExponencial();			// Genera el número con una distribución Hyper-exponencial.
				break;
			case(4):
				numero = distribucion.coxiar();				// Genera el número con una distribución Coxiar.
				break;
			default:
				numero = distribucion.exp();				// Genera el número con una districión exponencial.
		}
		rango=static_cast<unsigned int>(numero * 10.0f);	// Redondea el número a un decimal. Ejemplo: 1.23 -> 1.2
		if (rango >= inicio && rango <= tope) {
			histograma[rango]++;							// Incrementa el contador del rango correspondiente.
		}
	}

	// Guarda los números en un archivo CSV...
	Excel::guardarLista(histograma, "histo-exp.csv");
	/*for (unsigned int i = 0; i < tope; i++) {
			std::cout << static_cast<float>(i) / 10.0f << "," << histograma.at(i) << "\n";
	}//*/

	// Realiza la prueba de chi-cuadrado...
	std::cout << " Realizando prueba de chi-cuadrado..." << std::endl;
	switch(f) {
		case(2):
			q = X2test::getQErlang(histograma, tope, lambda, k, cantidad);	// Realiza la prueba de chi-cuadrado con el histograma y el valor de lambda.
			break;
		case(3):
			q = X2test::getQHyperExponencial(histograma, tope, lambda, p, cantidad);// Realiza la prueba de chi-cuadrado con el histograma y el valor de lambda.
			break;
		case(4):
			q = X2test::getQCoxiar(histograma, tope, lambda, p, cantidad);	// Realiza la prueba de chi-cuadrado con el histograma y el valor de lambda.
			break;
		default:
			q = X2test::getQExp(histograma, tope, lambda, cantidad);		// Realiza la prueba de chi-cuadrado con el histograma y el valor de lambda.
	}
	std::cout << "\tq = " << q << std::endl;								// Muestra el valor de q en la consola.
	std::cout << " La distribucion con "<< tope<<" grados de libertad es exponencial?..." << std::endl;
	std::cout << "\t" << ( X2test::validarQExp(q, tope/10) ? "Si" : "No") << std::endl;//*/
    return 0;
}