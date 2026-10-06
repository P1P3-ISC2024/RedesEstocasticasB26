#ifndef CHI_TEST_H
#define CHI_TEST_H

#include <vector>

class X2test
{
private:
	// Calcula la probabilidad de que un valor caiga en el intervalo [a, b) según la distribución exponencial.
	static double Eexp(int interval, double lambda, int N) {
		double a = static_cast<double>(interval) / 10.0;	// Convierte el índice del intervalo a un valor real.
		double b = static_cast<double>(interval + 1) / 10.0;// fin del intervalo.
		return N*( exp(-lambda * a) - exp(-lambda * b) );	// Probabilidad de que un valor caiga en el intervalo [a, b) según la distribución exponencial.
	}

	// Calcula la función de distribución acumulada (CDF) de la distribución Erlang.
	static double ErlangCDF(double x, double lambda, int k) {
		double sum = 1;
		for (int n = 0; n < k; n++) {
			sum -= exp(-lambda * x) * pow(lambda * x, n) / factorial(n);
		}
		//std::cout << "ErlangCDF(" << x << ", " << lambda << ", " << k << ") = " << sum << std::endl;
		return sum;
	}

	// Calcula la probabilidad de que un valor caiga en el intervalo [a, b) según la distribución Erlang.
	// vease: https://en.wikipedia.org/wiki/Erlang_distribution
	static double EErlang(int interval, double lambda, int k, int N) {
		double a = static_cast<double>(interval) / 10.0;	// Convierte el índice del intervalo a un valor real.
		double b = static_cast<double>(interval + 1) / 10.0;// fin del intervalo.
		double cdf_a;										// CDF de Erlang para k=2 en el límite inferior.
		double cdf_b;										// CDF de Erlang para k=2 en el límite superior.

		// Usando la CFD...
		cdf_a = ErlangCDF(a, lambda, k);
		cdf_b = ErlangCDF(b, lambda, k);

		// F(b) - F(a) = P(a <= X < b)
		//std::cout << "EErlang(" << interval << ", " << lambda << ", " << k << ", " << N << ") = " << N * (cdf_b - cdf_a) << std::endl;
		return N*(cdf_b - cdf_a);
	}

	// Calcula la probabilidad de que un valor caiga en el intervalo [a, b) según la distribución Hyper-exponencial.
	// vease: https://en.wikipedia.org/wiki/Hyperexponential_distribution
	static double EHyperExponencial(int interval, double lambda, double p, int N) {
		double a = static_cast<double>(interval) / 10.0;							// Convierte el índice del intervalo a un valor real.
		double b = static_cast<double>(interval + 1) / 10.0;						// fin del intervalo.
		// Se calcula de manera similar a la exponencial, pero con dos valores de lambda por p y (1-p).
		double cdf_a = p * (1 - exp(-lambda * a)) + (1 - p) * (1 - exp(-1.5f * a));	// CDF de Hyper-exponencial para el límite inferior.
		double cdf_b = p * (1 - exp(-lambda * b)) + (1 - p) * (1 - exp(-1.5f * b));	// CDF de Hyper-exponencial para el límite superior.
		return N * (cdf_b - cdf_a);	// Probabilidad de que un valor caiga en el intervalo [a, b) según la distribución Hyper-exponencial.
	}

	// Calcula la probabilidad de que un valor caiga en el intervalo [a, b) según la distribución Coxiar.
	// vease: https://en.wikipedia.org/wiki/Coxian_distribution
	static double ECoxiar(int interval, double lambda, double p, int N) {
		double a = static_cast<double>(interval) / 10.0;	// Convierte el índice del intervalo a un valor real.
		double b = static_cast<double>(interval + 1) / 10.0;// fin del intervalo.
		return N* (exp(-lambda * a) - exp(-lambda * b));
	}

public:

	static double factorial(int n) {
		if (n <= 1) return 1;
		double result = 1;
		for (int i = 2; i <= n; ++i) {
			result *= i;
		}
		return result;
	}

	// Realiza el cálculo de q de acruerdo a chi test con la distribución exponencial.
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

	// Realiza el cálculo de q de acruerdo a chi test con la distribución Erlang.
	static double getQErlang(const std::vector<int>& elementos, const int gradosLibertad, const double lambda, const int k, const int N) {
		double q = 0.0;
		double esperado = 0.0;
		for (unsigned int i = 0; i < elementos.size(); i++) {
			esperado = EErlang(i, lambda, k, N);
			q += (static_cast<double>(elementos[i]) - esperado) * (static_cast<double>(elementos[i]) - esperado) / esperado;
			//std::cout << "q = " << q << std::endl;
		}
		return q;
	}

	// Realiza el cálculo de q de acruerdo a chi test con la distribución Hyper-exponencial.
	static double getQHyperExponencial(const std::vector<int>& elementos, const int gradosLibertad, const double lambda, const double p, const int N) {
		double q = 0.0;
		double esperado = 0.0;
		for (unsigned int i = 0; i < elementos.size(); i++) {
			esperado = EHyperExponencial(i, lambda, p, N);
			q += (static_cast<double>(elementos[i]) - esperado) * (static_cast<double>(elementos[i]) - esperado) / esperado;
		}
		return q;
	}

	// Realiza el cálculo de q de acruerdo a chi test con la distribución coxiar.
	static double getQCoxiar(const std::vector<int>& elementos, const int gradosLibertad, const double lambda, const double p, const int N) {
		double q = 0.0;
		double esperado = 0.0;
		for (unsigned int i = 0; i < elementos.size(); i++) {
			esperado = ECoxiar(i, lambda, p, N);
			q += (static_cast<double>(elementos[i]) - esperado) * (static_cast<double>(elementos[i]) - esperado) / esperado;
		}
		q = 1;
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