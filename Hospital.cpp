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

    void crearArchivoCSV() {
        // Intentar abrir el archivo en modo lectura
        ifstream archivo("Pacientes.csv");

        // Si el archivo no existe, lo creamos y escribimos los encabezados
        cout << "1\n";
        if (!archivo) {
            cout << "2\n";
            ofstream archivoSalida("Pacientes.csv");
            if (archivoSalida.is_open()) {
                cout << "3\n";
                archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; // Escribir encabezados
                cout << "Archivo creado y encabezados escritos.\n"; // Mensaje de depuración
                archivoSalida.close();
                cout << "4\n";
            }
            else {
                cout << "5\n";
                cout << "No se pudo abrir el archivo para crear.\n";
            }
        }
        else {
            cout << "6\n";
            // Verificar si el archivo está vacío
            if (archivo.peek() == std::ifstream::traits_type::eof()) {
                cout << "7\n";
                // Si está vacío, escribir el encabezado
                ofstream archivoSalida("Pacientes.csv", ios::app);
                cout << "8\n";
                if (archivoSalida.is_open()) {
                    cout << "9\n";
                    archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; // Escribir encabezados
                    cout << "Encabezados escritos en el archivo vacío.\n"; // Mensaje de depuración
                    archivoSalida.close();
                    cout << "10\n";
                }
                else {
                    cout << "11\n";
                    cout << "No se pudo abrir el archivo para agregar encabezados.\n";
                    cout << "12\n";
                }
            }
            else {
                cout << "13\n";
                cout << "El archivo ya existe y no está vacío, no se escribieron encabezados.\n"; // Mensaje si el archivo ya existe y no está vacío
                cout << "14\n";
            }
        }

        archivo.close(); // Cerrar el archivo si se abrió
    }

    int obtenerMaxId() {
        ifstream archivo("Pacientes.csv");
        string linea;
        int maxId = 0;

        // Leer el archivo línea por línea
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find("ID") != string::npos) {
                continue; // Ignorar la línea de encabezado y líneas vacías
            }
            int id;
            sscanf(linea.c_str(), "%d", &id); // Extraer el ID de la línea
            if (id > maxId) {
                maxId = id; // Actualizar maxId si se encuentra un ID mayor
            }
        }

        archivo.close();
        return maxId;
    }

    bool guardarPacienteEnCSV(const Paciente& paciente) {
    ofstream archivo("Pacientes.csv", ios::app); 
    if (archivo.is_open()) {
        cout << "Archivo abierto correctamente.\n"; 
        // Escribir encabezados solo si el archivo está vacío
        /*if (archivo.tellp() == 0) {
            archivo << "ID,Nombre,Fecha de Ingreso\n";
            cout << "Encabezados escritos.\n"; // Mensaje de depuración 
        }*/
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
        void agregarPaciente(const string& nombre, const string& fechaIngreso) {
            // Verifica si el archivo CSV existe y crea si no existe
            crearArchivoCSV();

            int nuevoId = obtenerMaxId() + 1; // Obtener el máximo ID y sumarle 1
            auto nuevoPaciente = make_unique<Paciente>(nuevoId, nombre, fechaIngreso);
            if (guardarPacienteEnCSV(*nuevoPaciente)) { // Guardar en CSV
                pacientes.emplace_back(move(nuevoPaciente)); // Agregar paciente a la lista
                cout << "Paciente agregado correctamente con ID: " << nuevoId << "\n";
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
            string nombre, fechaIngreso;

            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese nombre del paciente: ";
            getline(cin, nombre); // Leer nombre completo
            cout << "Ingrese fecha de ingreso del paciente: ";
            getline(cin, fechaIngreso); // Leer fecha de ingreso

            hospital.agregarPaciente(nombre, fechaIngreso); // Llamar al método sin id
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


