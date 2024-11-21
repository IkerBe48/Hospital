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

    Paciente(int id, string nombre, string fechaIngreso)
        : id(id), nombre(nombre), fechaIngreso(fechaIngreso), historialClinico("") {
    }

    void agregarHistorial(string info) {
        historialClinico += info + "\n";
    }

    void mostrarInfo() const {
        cout << "Nombre: " << nombre << ", ID: " << id << ", Fecha de Ingreso: " << fechaIngreso << "\n";
        cout << "Historial Clínico: " << historialClinico << "\n";
    }

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

    public:
        void agregarPaciente(int id, string nombre, string fechaIngreso) {
            pacientes.emplace_back(make_unique<Paciente>(id, nombre, fechaIngreso));
    }

};

int main()
{
    Hospital hospital;
    int opcion;

    while (true) {
        cout << "------Menu------\n";
        cout << "1. Agregar paciente\n";
        cout << "2. Salir\n";
        cout << "\nIntroduce un numero: ";
        cin >> opcion;

        switch (opcion) {

        case 1: {
            int id; 
            string nombre, fechaIngreso;
            cout << "Ingrese nombre y fecha de ingreso del paciente: ";
            cin >> id >> nombre >> fechaIngreso;
            hospital.agregarPaciente(id, nombre, fechaIngreso);
            break;
        }
        case 2:
            return 0;
        default:
            cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }

    return 0;
}


