// Hospital.cpp: define el punto de entrada de la aplicación.
//

#include "Hospital.h"
#include <iostream>
#include <vector>

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

// Clase Cita
class Cita {
public:
    unique_ptr<Paciente> paciente;
    unique_ptr<Medico> medico;
    string fecha;
    int urgencia; // 1: baja, 2: media, 3: alta

};

// Clase Hospital
class Hospital {
private:
    vector<unique_ptr<Paciente>> pacientes;
    vector<unique_ptr<Medico>> medicos;
    vector<unique_ptr<Cita>> citas;

};

int main()
{
    int opcion;
    while (true) {
        cout << "------Menu------\n";
        cout << "1. Salir\n";
        cout << "\nIntroduce un numero: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            return 0;
        default:
            cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }

    return 0;
}


