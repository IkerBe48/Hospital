#include "Medico.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <sstream>

std::vector<std::unique_ptr<Medico>> Medico::medicos; // Inicializar el vector de médicos

Medico::Medico(int id, const std::string& nombre, const std::string& especialidad)
    : id(id), nombre(nombre), especialidad(especialidad) {
}

void Medico::crearMedicosCSV() {
    std::ifstream archivo("Medicos.csv");

    if (!archivo) {
        std::ofstream archivoSalida("Medicos.csv");
        if (archivoSalida.is_open()) {
            archivoSalida << "ID,Nombre,Especialidad\n"; // AGREGAR CAMPO DISPONIBLE BOOL Y QUE SI EL MEDICO TIENE 
            // DISPONIBLE FALSE, MUESTRE ERROR QUE NO SE PUEDE INTRODUCIR CITA CON DICHO MEDICO
            std::cout << "\n Archivo creado y encabezados escritos.\n";
            archivoSalida.close();
        }
        else {
            std::cout << "\n No se pudo abrir el archivo para crear.\n";
        }
    }
    else {
        if (archivo.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream archivoSalida("Medicos.csv", std::ios::app);
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Nombre,Especialidad\n";
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

int Medico::obtenerMaxIdMedicos() {
    std::ifstream archivo("Medicos.csv");
    std::string linea;
    int maxId = 0;

    while (std::getline(archivo, linea)) {
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

bool Medico::guardarMedicoEnCSV(const Medico& medico) {
    std::ofstream archivo("Medicos.csv", std::ios::app);
    if (archivo.is_open()) {
        archivo << medico.id << "," << medico.nombre << "," << medico.especialidad << "\n";
        std::cout << "\n\n Datos del Medico escritos: | " << medico.id << " | " << medico.nombre << " | " << medico.especialidad << " |\n";
        archivo.close();
        return true;
    }
    else {
        std::cout << "\n No se pudo abrir el archivo para escribir.\n";
        return false;
    }
}

void Medico::buscarMedico(const std::string& nombreBuscado) {
    std::ifstream archivo("Medicos.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');

        if (nombre == nombreBuscado) {
            encontrado = true;
            std::cout << "\n\n Medico encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n Medico no encontrado." << std::endl;
    }

    archivo.close();
}

void Medico::agregarMedico(const std::string& nombre, const std::string& especialidad) {
    crearMedicosCSV();

    int nuevoId = obtenerMaxIdMedicos() + 1;
    auto nuevoMedico = std::make_unique<Medico>(nuevoId, nombre, especialidad);
    if (guardarMedicoEnCSV(*nuevoMedico)) {
        medicos.emplace_back(std::move(nuevoMedico));
        std::cout << "\n\n Medico agregado correctamente con ID: " << nuevoId << "\n";
    }
    else {
        std::cout << "\n Error al agregar el medico. Intente nuevamente.\n";
    }
}

void Medico::buscarMedicoPorNombre(const std::string& nombreBuscado) {
    buscarMedico(nombreBuscado);
}

void Medico::eliminarMedico(const std::string& nombreBuscado) {
    std::ifstream archivo("Medicos.csv");
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');

        if (nombre != nombreBuscado) {
            lineas.push_back(linea);
        }
        else {
            encontrado = true;
        }
    }

    archivo.close();

    if (encontrado) {
        std::ofstream archivoSalida("Medicos.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "\n\n Medico | " << nombreBuscado << " | eliminado correctamente." << std::endl;
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n Medico | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Medico::modificarNombreMedico(const std::string& nombreBuscado, const std::string& nuevoNombre) {
    std::ifstream archivo("Medicos.csv");
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre, especialidad;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');
        std::getline(stream, especialidad);

        if (nombre == nombreBuscado) {
            nombre = nuevoNombre;
            encontrado = true;
        }

        lineas.push_back(id + "," + nombre + "," + especialidad);
    }

    archivo.close();

    if (encontrado) {
        std::ofstream archivoSalida("Medicos.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "\n\n Nombre del medico | " << nombreBuscado << " | modificado a: |" << nuevoNombre << " | correctamente." << std::endl;
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n Medico | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Medico::interfazMedicos() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar medico\n";
        std::cout << "2. Buscar medico por nombre\n";
        std::cout << "3. Eliminar medico por nombre\n";
        std::cout << "4. Modificar nombre de medico\n";
        std::cout << "5. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            std::string nombre, especialidad;
            std::cin.ignore();
            std::cout << "Ingrese nombre del medico: ";
            std::getline(std::cin, nombre);
            std::cout << "Ingrese la especialidad del medico: ";
            std::getline(std::cin, especialidad);
            Medico::agregarMedico(nombre, especialidad);
            break;
        }
        case 2: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del medico a buscar: ";
            std::getline(std::cin, nombreBuscado);
            Medico::buscarMedicoPorNombre(nombreBuscado);
            break;
        }
        case 3: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del medico a eliminar: ";
            std::getline(std::cin, nombreBuscado);
            Medico::eliminarMedico(nombreBuscado);
            break;
        }
        case 4: {
            std::string nombreBuscado, nuevoNombre;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del medico a modificar: ";
            std::getline(std::cin, nombreBuscado);
            std::cout << "Ingrese el nuevo nombre del medico: ";
            std::getline(std::cin, nuevoNombre);
            Medico::modificarNombreMedico(nombreBuscado, nuevoNombre);
            break;
        }
        case 5:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}