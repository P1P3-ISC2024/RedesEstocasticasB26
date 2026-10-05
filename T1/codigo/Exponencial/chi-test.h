#ifndef CHI_TEST_H
#define CHI_TEST_H

#include <vector>

class X2test
{
private:
	static double Eexp(int interval, double lambda, int N) {
		double a = static_cast<double>(interval) / 10.0;	// Convierte el índice del intervalo a un valor real.
		double b = static_cast<double>(interval + 1) / 10.0;// fin del intervalo.
		return N*( exp(-lambda * a) - exp(-lambda * b) );	// Probabilidad de que un valor caiga en el intervalo [a, b) según la distribución exponencial.
	}

public:

	// realiza el cálculo de q de acruerdo a chi test con la distribución exponencial.
	static double getQExp(const std::vector<int>& elementos, const int gradosLibertad, const double lambda, const int N) {
		double q = 0.0;
		double esperado = 0.0;
		for (unsigned int i = 0; i < elementos.size(); i++) {
			esperado = Eexp(i, lambda, N);
			//std::cout << "E_exp(" << i << ") = " << esperado << std::endl;
			q += (static_cast<double>(elementos[i]) - esperado) * (static_cast<double>(elementos[i]) - esperado) / esperado;
		}
		return q;
	}

	// Para alfa = 0.005 verifica si q es menor que el valor crítico de chi-cuadrado.
	// solo está disponible para grados de libertad entre 1 y 10.
	// (me dió hueva entender bien como sacar la CDF de la chi-cuadrada xD así que uso valores predeterminados.)
	static bool validarQExp(const double q, const int gradosLibertad) {
		// Valores de X^2_{1-0.005}(grados de libertad)...
		double chiCritico[10] = {
			25.1881,
			39.9969,
			53.6719,
			66.7660,
			79.4898,
			91.9518,
			104.2148,
			116.3209,
			128.2987,
			140.1697,
		};
		//std::cout << q << " < " << chiCritico[gradosLibertad - 1] << std::endl;
		return q < chiCritico[gradosLibertad-1];
	}
};

#endif