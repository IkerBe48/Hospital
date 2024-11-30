#include "Paciente.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <sstream>
#include <stdexcept> // Para std::runtime_error

std::vector<std::unique_ptr<Paciente>> Paciente::pacientes; // Inicializar el vector de médicos

Paciente::Paciente(int id, const std::string& nombre, const std::string& fechaIngreso)
    : id(id), nombre(nombre), fechaIngreso(fechaIngreso) {
}

/*void Paciente::agregarHistorial(std::string info) {
    historialClinico += info + "\n";
}

void Paciente::mostrarInfo() const {
    std::cout << "Nombre: " << nombre << ", ID: " << id << ", Fecha de Ingreso: " << fechaIngreso << "\n";
    std::cout << "Historial Clínico: " << historialClinico << "\n";
}*/

void Paciente::crearPacientesCSV() {
    std::ifstream archivo("Pacientes.csv");

    if (!archivo) {
        std::ofstream archivoSalida("Pacientes.csv");
        if (archivoSalida.is_open()) {
            archivoSalida << "ID,Nombre,Fecha de Ingreso\n";
            std::cout << "Archivo creado y encabezados escritos.\n";
        }
        else {
            std::cout << "No se pudo abrir el archivo para crear.\n";
        }
    }
    else {
        if (archivo.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream archivoSalida("Pacientes.csv", std::ios::app);
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Nombre,Fecha de Ingreso\n";
                std::cout << "Encabezados escritos en el archivo vacío.\n";
            }
            else {
                std::cout << "No se pudo abrir el archivo para agregar encabezados.\n";
            }
        }
        else {
            std::cout << "El archivo ya existe y no está vacío, no se escribieron encabezados.\n";
        }
    }
}

int Paciente::obtenerMaxId() {
    std::ifstream archivo("Pacientes.csv");
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

    return maxId;
}

bool Paciente::guardarPacienteEnCSV(const Paciente& paciente) {
    std::ofstream archivo("Pacientes.csv", std::ios::app);
    if (archivo.is_open()) {
        archivo << paciente.id << "," << paciente.nombre << "," << paciente.fechaIngreso << "\n";
        std::cout << "Datos del paciente escritos: | " << paciente.id << " | " << paciente.nombre << " | " << paciente.fechaIngreso << " | \n\n";
        return true;
    }
    else {
        std::cout << "No se pudo abrir el archivo para escribir.\n";
        return false;
    }
}

void Paciente::buscarPaciente(const std::string& nombreBuscado) {
    std::ifstream archivo("Pacientes.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        std::getline(stream, id, ','); // Asumiendo que el ID es la primera columna
        std::getline(stream, nombre, ','); // Asumiendo que el nombre es la segunda columna

        if (nombre == nombreBuscado) {
            encontrado = true;
            std::cout << "Paciente encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "Paciente no encontrado." << std::endl;
    }

    archivo.close();
}


void Paciente::agregarPaciente(const std::string& nombre, const std::string& fechaIngreso) {
    // Validar los datos de entrada
    if (nombre.empty()) {
        std::cout << "Error: El nombre no puede estar vacío.\n";
        return;
    }

    if (fechaIngreso.empty()) {
        std::cout << "Error: La fecha de ingreso no puede estar vacía.\n";
        return;
    }
    crearPacientesCSV();

    int nuevoId = obtenerMaxId() + 1;
    auto nuevoPaciente = std::make_unique<Paciente>(nuevoId, nombre, fechaIngreso);
    if (guardarPacienteEnCSV(*nuevoPaciente)) {
        pacientes.emplace_back(std::move(nuevoPaciente));
        std::cout << "Paciente agregado correctamente con ID: " << nuevoId << "\n";
    }
    else {
        std::cout << "Error al agregar el paciente. Intente nuevamente.\n";
    }
}

void Paciente::buscarPacientePorNombre(const std::string& nombreBuscado) {
    buscarPaciente(nombreBuscado); // Llamar al método de búsqueda
}

void Paciente::eliminarPaciente(const std::string& nombreBuscado) {
    std::ifstream archivo("Pacientes.csv");
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        std::getline(stream, id, ','); // Leer ID
        std::getline(stream, nombre, ','); // Leer nombre

        if (nombre != nombreBuscado) {
            lineas.push_back(linea);
        }
        else {
            encontrado = true; // Marcar que encontramos el paciente
        }
    }

    archivo.close(); // Cerrar el archivo después de leer

    if (encontrado) {
        std::ofstream archivoSalida("Pacientes.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "Paciente | " << nombreBuscado << " | eliminado correctamente." << std::endl;
        }
        else {
            std::cerr << "Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "Paciente | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Paciente::modificarNombrePaciente(const std::string& nombreBuscado, const std::string& nuevoNombre) {
    std::ifstream archivo("Pacientes.csv");
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre, fechaIngreso;

        std::getline(stream, id, ','); // Leer ID
        std::getline(stream, nombre, ','); // Leer nombre
        std::getline(stream, fechaIngreso); // Leer fecha de ingreso

        if (nombre == nombreBuscado) {
            nombre = nuevoNombre; // Cambiar el nombre al nuevo
            encontrado = true; // Marcar que encontramos el paciente
        }

        lineas.push_back(id + "," + nombre + "," + fechaIngreso);
    }

    archivo.close(); // Cerrar el archivo después de leer

    if (encontrado) {
        std::ofstream archivoSalida("Pacientes.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "Nombre del paciente | " << nombreBuscado << " | modificado a: | " << nuevoNombre << " | correctamente." << std::endl;
        }
        else {
            std::cerr << "Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "Paciente | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}