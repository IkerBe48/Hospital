#include "Paciente.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <sstream>
#include <stdexcept> // Para std::runtime_error

std::vector<std::unique_ptr<Paciente>> Paciente::pacientes; // Inicializar el vector de m�dicos

Paciente::Paciente(int id, const std::string& nombre, const std::string& fechaIngreso)
    : id(id), nombre(nombre), fechaIngreso(fechaIngreso) {
}

/*void Paciente::agregarHistorial(std::string info) {
    historialClinico += info + "\n";
}

void Paciente::mostrarInfo() const {
    std::cout << "Nombre: " << nombre << ", ID: " << id << ", Fecha de Ingreso: " << fechaIngreso << "\n";
    std::cout << "Historial Cl�nico: " << historialClinico << "\n";
}*/

void Paciente::crearPacientesCSV() {
    std::ifstream archivo("Pacientes.csv");

    if (!archivo) {
        std::ofstream archivoSalida("Pacientes.csv");
        if (archivoSalida.is_open()) {
            archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; //A�ADIR CAMPO FECHA SALIDA Y QUE AL AGREGAR SEA NULO. 
            //USANDO ALGO PARECIDO AL DE MODIFICAR NOMBRE PACIENTE, AGREGAR FECHA SALIDA Y QUE NO PUEDA SER MENOR A LA FECHA INGRESO
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
                std::cout << "\n Encabezados escritos en el archivo vac�o.\n";
            }
            else {
                std::cout << "\n No se pudo abrir el archivo para agregar encabezados.\n";
            }
        }
        else {
            std::cout << "\n El archivo ya existe y no est� vac�o, no se escribieron encabezados.\n";
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
        std::cout << "\n\n Datos del paciente escritos: | " << paciente.id << " | " << paciente.nombre << " | " << paciente.fechaIngreso << " | \n\n";
        return true;
    }
    else {
        std::cout << "\n No se pudo abrir el archivo para escribir.\n";
        return false;
    }
}

void Paciente::buscarPaciente(const std::string& nombreBuscado) {
    std::ifstream archivo("Pacientes.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre;

        std::getline(stream, id, ','); // Asumiendo que el ID es la primera columna
        std::getline(stream, nombre, ','); // Asumiendo que el nombre es la segunda columna

        if (nombre == nombreBuscado) {
            encontrado = true;
            std::cout << "\n Paciente encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n Paciente no encontrado." << std::endl;
    }

    archivo.close();
}


void Paciente::agregarPaciente(const std::string& nombre, const std::string& fechaIngreso) {
    // Validar los datos de entrada
    if (nombre.empty()) {
        std::cout << "\n Error: El nombre no puede estar vac�o.\n";
        return;
    }

    if (fechaIngreso.empty()) {
        std::cout << "\n Error: La fecha de ingreso no puede estar vac�a.\n";
        return;
    }
    crearPacientesCSV();

    int nuevoId = obtenerMaxId() + 1;
    auto nuevoPaciente = std::make_unique<Paciente>(nuevoId, nombre, fechaIngreso);
    if (guardarPacienteEnCSV(*nuevoPaciente)) {
        pacientes.emplace_back(std::move(nuevoPaciente));
        std::cout << "\n Paciente agregado correctamente con ID: " << nuevoId << "\n";
    }
    else {
        std::cout << "\n Error al agregar el paciente. Intente nuevamente.\n";
    }
}

void Paciente::buscarPacientePorNombre(const std::string& nombreBuscado) {
    buscarPaciente(nombreBuscado); // Llamar al m�todo de b�squeda
}

void Paciente::eliminarPaciente(const std::string& nombreBuscado) {
    std::ifstream archivo("Pacientes.csv");
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

        std::getline(stream, id, ','); // Leer ID
        std::getline(stream, nombre, ','); // Leer nombre

        if (nombre != nombreBuscado) {
            lineas.push_back(linea);
        }
        else {
            encontrado = true; // Marcar que encontramos el paciente
        }
    }

    archivo.close(); // Cerrar el archivo despu�s de leer

    if (encontrado) {
        std::ofstream archivoSalida("Pacientes.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "\n\n Paciente | " << nombreBuscado << " | eliminado correctamente." << std::endl;
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n Paciente | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Paciente::modificarNombrePaciente(const std::string& nombreBuscado, const std::string& nuevoNombre) {
    std::ifstream archivo("Pacientes.csv");
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
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

    archivo.close(); // Cerrar el archivo despu�s de leer

    if (encontrado) {
        std::ofstream archivoSalida("Pacientes.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "\n\n Nombre del paciente | " << nombreBuscado << " | modificado a: | " << nuevoNombre << " | correctamente." << std::endl;
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n Paciente | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Paciente::interfazPacientes() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar paciente\n";
        std::cout << "2. Buscar paciente por nombre\n";
        std::cout << "3. Eliminar paciente por nombre\n";
        std::cout << "4. Modificar nombre de paciente\n";
        std::cout << "5. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            std::string nombre, fechaIngreso;
            std::cin.ignore();
            std::cout << "Ingrese nombre del paciente: ";
            std::getline(std::cin, nombre);
            std::cout << "Ingrese fecha de ingreso del paciente: ";
            std::getline(std::cin, fechaIngreso);
            Paciente::agregarPaciente(nombre, fechaIngreso);
            break;
        }
        case 2: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a buscar: ";
            std::getline(std::cin, nombreBuscado);
            Paciente::buscarPacientePorNombre(nombreBuscado);
            break;
        }
        case 3: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a eliminar: ";
            std::getline(std::cin, nombreBuscado);
            Paciente::eliminarPaciente(nombreBuscado);
            break;
        }
        case 4: {
            std::string nombreBuscado, nuevoNombre;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a modificar: ";
            std::getline(std::cin, nombreBuscado);
            std::cout << "Ingrese el nuevo nombre del paciente: ";
            std::getline(std::cin, nuevoNombre);
            Paciente::modificarNombrePaciente(nombreBuscado, nuevoNombre);
            break;
        }
        case 5:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}