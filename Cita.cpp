#include "Cita.h"
#include "Paciente.h" 
#include "Medico.h"   
#include <vector>
#include <fstream> // Para manejar archivos
#include <memory>  // Para usar unique_ptr
#include <string>  // Para usar std::string y getline
#include <limits>  // Para usar std::numeric_limits
#include <sstream> // Para usar std::istringstream

std::vector<std::unique_ptr<Cita>> Cita::citas;

Cita::Cita(std::unique_ptr<Paciente> paciente, std::unique_ptr<Medico> medico, const std::string& fecha, int urgencia)
    : paciente(std::move(paciente)), medico(std::move(medico)), fecha(fecha), urgencia(urgencia) {
}

void Cita::crearCitasCSV() {
    std::ifstream archivo("Citas.csv");

    if (!archivo) {
        std::ofstream archivoSalida("Citas.csv");
        if (archivoSalida.is_open()) {
            archivoSalida << "ID,Paciente,Medico,fecha de entrada,urgencia\n";
            std::cout << "\n Archivo creado y encabezados escritos.\n";
            archivoSalida.close();
        }
        else {
            std::cout << "\n No se pudo abrir el archivo para crear.\n";
        }
    }
    else {
        if (archivo.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream archivoSalida("Citas.csv", std::ios::app);
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Paciente,Medico,fecha de entrada,urgencia\n";
                std::cout << "\n Encabezados escritos en el archivo vacío.\n";
                archivoSalida.close();
            }
            else {
                std::cout << "\n No se pudo abrir el archivo para agregar encabezados.\n";
            }
        }
        else {
            std::cout << "\n El archivo ya existe y no está vacío, no se escribieron encabezados.\n";
        }
    }

    archivo.close();
}

int Cita::obtenerMaxIdCitas() {
    std::ifstream archivo("Citas.csv");
    std::string linea;
    int maxId = 0;

    while (getline(archivo, linea)) {
        if (linea.empty() || linea.find("ID") != std::string::npos) {
            continue;
        }
        int id;
        sscanf(linea.c_str(), "%d", &id);
        if (id > maxId) {
            maxId = id;
        }
    }

    archivo.close();
    return maxId;
}

bool Cita::guardarCitaEnCSV(const Cita& cita) {
    std::ofstream archivo("Citas.csv", std::ios::app);
    if (archivo.is_open()) {
        std::cout << "\n Archivo de citas abierto correctamente.\n";
        // Supongamos que agregas un campo id en la clase Cita
        archivo << cita.urgencia << "," << cita.paciente->getNombre() << "," << cita.medico->getNombre() << "," << cita.fecha << "\n";
        std::cout << "\n Datos de la cita escritos: | " << cita.paciente->getNombre() << " | con | " << cita.medico->getNombre() << " en | " << cita.fecha << " | \n\n";
        archivo.close();
        return true;
    }
    else {
        std::cout << "No se pudo abrir el archivo para escribir.\n";
        return false;
    }
}

bool Cita::buscarPacienteEnCSV(const std::string& nombreBuscado) {
    std::ifstream archivo("Pacientes.csv");
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "\nError al abrir el archivo de pacientes." << std::endl;
        return false;
    }

    while (getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        getline(stream, id, ',');
        getline(stream, nombre, ',');

        if (nombre == nombreBuscado) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

bool Cita::buscarMedicoEnCSV(const std::string& nombreBuscado) {
    std::ifstream archivo("Medicos.csv");
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "\nError al abrir el archivo de médicos." << std::endl;
        return false;
    }

    while (getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        getline(stream, id, ',');
        getline(stream, nombre, ',');

        if (nombre == nombreBuscado) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

void Cita::agregarCita(const std::string& nombrePaciente, const std::string& nombreMedico, const std::string& fecha, int urgencia) {
    crearCitasCSV();

    if (!buscarPacienteEnCSV(nombrePaciente)) {
        std::cout << "\nEl paciente | " << nombrePaciente << " | no existe en el sistema. No se puede agregar la cita." << std::endl;
        return;
    }

    if (!buscarMedicoEnCSV(nombreMedico)) {
        std::cout << "\nEl médico | " << nombreMedico << " | no existe en el sistema. No se puede agregar la cita." << std::endl;
        return;
    }

    int nuevoId = obtenerMaxIdCitas() + 1;
    auto paciente = std::make_unique<Paciente>(nuevoId, nombrePaciente, "");
    auto medico = std::make_unique<Medico>(nuevoId, nombreMedico, "");
    auto nuevaCita = std::make_unique<Cita>(std::move(paciente), std::move(medico), fecha, urgencia);

    if (guardarCitaEnCSV(*nuevaCita)) {
        citas.emplace_back(std::move(nuevaCita));
        std::cout << "\n\n Cita agregada correctamente con ID: " << nuevoId << " \n\n";
    }
    else {
        std::cout << "\nError al agregar la cita. Intente nuevamente.\n";
    }
}

void Cita::interfazCitas() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar Cita\n";
        std::cout << "2. Buscar Cita por nombre (PENDIENTE)\n";
        std::cout << "3. Eliminar Cita por nombre (PENDIENTE)\n";
        std::cout << "4. Modificar Cita (PENDIENTE)\n";
        std::cout << "5. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            std::string nombrePaciente, nombreMedico, fecha;
            int urgencia;
            std::cin.ignore();
            std::cout << "Ingrese nombre del paciente: ";
            std::getline(std::cin, nombrePaciente);
            std::cout << "Ingrese nombre del medico: ";
            std::getline(std::cin, nombreMedico);
            std::cout << "Ingrese fecha de la cita: ";
            std::getline(std::cin, fecha);
            while (true) {
                std::cout << "Ingrese urgencia (1: baja, 2: media, 3: alta): ";
                std::cin >> urgencia;

                // Verificar si la entrada es válida
                if (std::cin.fail() || urgencia < 1 || urgencia > 3) {
                    std::cin.clear(); // Limpiar el estado de error
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
                    std::cout << "Urgencia inválida. Por favor, ingrese un numero del 1 al 3." << std::endl;
                }
                else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar el buffer
                    break; // Salir del bucle si la entrada es válida
                }
            }
            Cita::agregarCita(nombrePaciente, nombreMedico, fecha, urgencia);
            break;
        }
        case 5:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}