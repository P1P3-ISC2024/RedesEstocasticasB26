#ifndef EXCEL_H
#define EXCEL_H


#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

class Excel
{
public:

	// Guarda una lista de elementos en un archivo CSV. Desde un vector de flotantes.
    static void guardarLista(
        const std::vector<int>& elementos,
        const std::string& nombreArchivo
    )
    {
		//std::string contenido = "New-Item -Path \"" + nombreArchivo + "\" -ItemType File";
		//std::string comando = "echo " + contenido;
		std::cout << "Guardando lista en archivo [" << nombreArchivo << "]" << std::endl;
		//system("pause");
        //system(comando.c_str());
		std::ofstream arch(nombreArchivo);
		std::cout << "Archivo abierto para escritura." << std::endl;
        // Escribe cada elemento en una fila...
		arch << "x,y\n";
        for (unsigned int i = 0; i < elementos.size(); i++)
        {
            //std::cout << static_cast<float>(i) / 10.0f << "," << elementos.at(i) << "\n";
            //arch << static_cast<float>(i)/10.0f << "," << elementos.at(i) << "\n";
        }//*/
        arch.close();
        std::cout << "Lista guardada. " << std::endl;
    }

	// Guarda una lista de elementos en un archivo CSV. Desde un diccionario de flotantes a enteros.
    static void guardarLista(
        const std::unordered_map<double, int>& elementos,
        const double tope,
        const std::string& nombreArchivo
    )
    {
        // Abre el archivo para escritura.
        std::ofstream archivo(nombreArchivo);

        // Escribe cada elemento en una fila.
		for (double i = 0; i <= tope; i += 0.1) {
			archivo << i << "," << elementos.at(i) << "\n";
		}

        archivo.close();
    }

    static void prueba() {
        std::cout << "A" << std::endl;

        std::ofstream archivo("prueba.csv");

        std::cout << "B" << std::endl;

        if (!archivo)
        {
            std::cout << "ERROR" << std::endl;
            return;
        }

        std::cout << "C" << std::endl;

        archivo << "Hola";

        std::cout << "D" << std::endl;

        archivo.close();

        std::cout << "E" << std::endl;
    }
};

#endif
