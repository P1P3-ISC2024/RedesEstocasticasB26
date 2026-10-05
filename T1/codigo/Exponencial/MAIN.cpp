#include "DistribucionExponencial.h"
#include "Excel.h"
#include "chi-test.h"

int main(int argc, char* argv[])
{
	int cantidad = 1;										// Cantidad de números a generar. Por defecto es 1.
	float lambda = argc > 3 ? std::stof(argv[3]) : 2.0f;	// Parámetro lambda de la distribución exponencial. Por defecto es 2.
	DistribucionExponencial distribucion(0, 1000000, lambda);// Inicializa la distribución exponencial con un rango de 0 a 1,000,000 en el uniforme y un parámetro lambda de 2.
	// si cambia la cantidad de numeros...
	if (argc > 1) {
		cantidad = std::stoi(argv[1]);						// stoi convierte un string a int. y std:: es el espacio de nombres de la biblioteca estándar.
	}
	std::cout << "Generando " << cantidad << " numeros con lambda = " << lambda << std::endl;
	/*/ Genera los numeros...
	for (int i = 0; i < cantidad; i++) {
		double numero = distribucion.exp();
		//std::cout << numero << std::endl;
		numeros.push_back(numero);
	}*/

	// Genera los numeros y el histograma...
	int inicio = 0;
	unsigned int tope = argc > 2 ? std::stoi(argv[2]) : 3;			// Si se pasa un tercer argumento, se toma como el tope del histograma. Sino, el tope es 3.
	tope *= 10;												// Multiplica el tope por 10 para que el histograma tenga un rango de 0 a 30 en lugar de 0 a 3.
	std::vector<int> histograma(tope);						// Conteo de numeros para el histograma. 0.1 = 1 y 3.0 = 30.
	std::cout << "Maximo numero en el histograma: " << tope/10 << std::endl;
	for (unsigned int i = inicio; i <= tope; i ++) {
		histograma[i] = 0;
	}
	for (unsigned int i = 0; i < cantidad; i++) {
		double numero = distribucion.exp();					// Genera el número con una districión exponencial con lambda = 2
		int rango = static_cast<int>(numero * 10.0f);		// Redondea el número a un decimal. Ejemplo: 1.23 -> 1.2
		if (rango >= inicio && rango <= tope) {
			histograma[rango]++;							// Incrementa el contador del rango correspondiente.
		}
	}

	// Guarda los números en un archivo CSV si la cantidad es mayor a 1...
	guardarLista(histograma, "histo-exp.csv");
	/*if (cantidad >1) {
		for (unsigned int i = 0; i < tope; i++)
		{
			std::cout << static_cast<float>(i) / 10.0f << "," << histograma.at(i) << "\n";
			//arch << static_cast<float>(i)/10.0f << "," << elementos.at(i) << "\n";
		}
	}//*/

	// Realiza la prueba de chi-cuadrado...
	std::cout << " Realizando prueba de chi-cuadrado..." << std::endl;
	double q = X2test::getQExp(histograma, tope, lambda, cantidad);		// Realiza la prueba de chi-cuadrado con el histograma y el valor de lambda.
	std::cout << "\tq = " << q << std::endl;							// Muestra el valor de q en la consola.
	std::cout << " La distribucion con "<< tope<<" grados de libertad es exponencial?..." << std::endl;
	std::cout << "\t" << ( X2test::validarQExp(q, tope/10) ? "Si" : "No") << std::endl;
    return 0;
}