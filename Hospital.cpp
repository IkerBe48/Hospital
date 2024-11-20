// Hospital.cpp: define el punto de entrada de la aplicación.
//

#include "Hospital.h"
#include <iostream>

using namespace std;

// Clase Paciente
class Paciente {
public:

    int id;
    string nombre;
    string fechaIngreso;
    string historialClinico;

};

// Clase Medico
class Medico {
public:

    int id;
    string nombre;
    string especialidad;
    bool disponible;

};

int main()
{
    int opcion;
    while (true) {
        cout << "------Menu------\n";
        cout << "1. Salir\n";
        cin >> opcion;

        switch (opcion) {
        case 1:
            return 0;
        default:
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
    }

    return 0;
}


