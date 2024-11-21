// Hospital.cpp: define el punto de entrada de la aplicación.
//

#include "Hospital.h"
#include <iostream>
#include <vector>
#include <fstream> // Para manejar archivos
#include <memory>  // Para usar unique_ptr
#include <string>  // Para usar std::string y getline
#include <limits>  // Para usar std::numeric_limits

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

    bool guardarPacienteEnCSV(const Paciente& paciente) {
    ofstream archivo("Pacientes.csv", ios::app); 
    if (archivo.is_open()) {
        cout << "Archivo abierto correctamente.\n"; 
        // Escribir encabezados solo si el archivo está vacío
        if (archivo.tellp() == 0) {
            archivo << "ID,Nombre,Fecha de Ingreso\n";
            cout << "Encabezados escritos.\n"; // Mensaje de depuración 
        }
        archivo << paciente.id << "," << paciente.nombre << "," << paciente.fechaIngreso << "\n";
        cout << "Datos del paciente escritos: " << paciente.id << ", " << paciente.nombre << ", " << paciente.fechaIngreso << "\n"; // Mensaje de depuración
        archivo.close();
        return true; // Indica que se guardó correctamente
    } else {
        cout << "No se pudo abrir el archivo para escribir.\n";
        return false; // Indica que hubo un error al abrir el archivo
    }
}
    public:
        void agregarPaciente(int id, const string& nombre, const string& fechaIngreso) {
            auto nuevoPaciente = make_unique<Paciente>(id, nombre, fechaIngreso);
            if (guardarPacienteEnCSV(*nuevoPaciente)) { // Guardar en CSV
                pacientes.emplace_back(move(nuevoPaciente)); // Agregar paciente a la lista
                cout << "Paciente agregado correctamente.\n";
            }
            else {
                cout << "Error al agregar el paciente. Intente nuevamente.\n";
            }
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

            cout << "Ingrese ID del paciente: ";
            while (!(cin >> id)) { // Verificar que la entrada sea un número
                cin.clear(); // Limpiar el estado de error
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar hasta el siguiente salto de línea
                cout << "Entrada inválida. Ingrese un número para el ID: ";
            }

            cin.ignore(); 
            cout << "Ingrese nombre del paciente: ";
            getline(cin, nombre); // Leer nombre completo
            cout << "Ingrese fecha de ingreso del paciente: ";
            getline(cin, fechaIngreso); // Leer fecha de ingreso
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


