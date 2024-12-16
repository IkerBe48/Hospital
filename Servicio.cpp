#include "Servicio.h"
#include "Paciente.h" 
#include <vector>
#include <fstream> // Para manejar archivos
#include <memory>  // Para usar unique_ptr
#include <string>  // Para usar std::string y getline
#include <limits>  // Para usar std::numeric_limits
#include <sstream> // Para usar std::istringstream
#include <filesystem>
#include <regex>

std::vector<std::unique_ptr<Servicio>> Servicio::servicios;

Servicio::Servicio(int id, std::unique_ptr<Paciente> paciente, const std::string& fecha, const std::string& descripcion)
    : id(id), paciente(std::move(paciente)), fecha(fecha), descripcion(descripcion) {
}

void Servicio::crearServiciosCSV() {
    std::ifstream archivo("Servicios.csv");

    if (!archivo) {
        std::ofstream archivoSalida("Servicios.csv");
        if (archivoSalida.is_open()) {
            archivoSalida << "ID,Paciente,fecha,descripcion\n";
            std::cout << "\n Archivo creado y encabezados escritos.\n";
            archivoSalida.close();
        }
        else {
            std::cout << "\n No se pudo abrir el archivo para crear.\n";
        }
    }
    else {
        if (archivo.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream archivoSalida("Servicios.csv", std::ios::app);
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Paciente,fecha,descripcion\n";
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

int Servicio::obtenerMaxIdServicios() {
    std::ifstream archivo("Servicios.csv");
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

bool Servicio::buscarPacienteEnCSV(const std::string& nombreBuscado) {
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

bool Servicio::esAnioBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

bool Servicio::esFechaValida(const std::string& fecha) {
    int dia, mes, anio;
    char separador1, separador2;

    std::istringstream fechaStream(fecha);
    fechaStream >> anio >> separador1 >> mes >> separador2 >> dia;

    // Verificar que se hayan leído correctamente los separadores
    if (separador1 != '-' || separador2 != '-') {
        return false;
    }

    // Validar el rango de mes
    if (mes < 1 || mes > 12) {
        return false;
    }

    // Validar el rango de días según el mes
    switch (mes) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: // Meses con 31 días
        return (dia >= 1 && dia <= 31);
    case 4: case 6: case 9: case 11: // Meses con 30 días
        return (dia >= 1 && dia <= 30);
    case 2: // Febrero
        if (esAnioBisiesto(anio)) {
            return (dia >= 1 && dia <= 29); // 29 días en año bisiesto
        }
        else {
            return (dia >= 1 && dia <= 28); // 28 días en año no bisiesto
        }
    default:
        return false; // No debería llegar aquí
    }
}

void Servicio::agregarServicio(const std::string& nombrePaciente, const std::string& fecha, const std::string& descripcion) {
    crearServiciosCSV();

    if (!buscarPacienteEnCSV(nombrePaciente)) {
        std::cout << "\nEl paciente | " << nombrePaciente << " | no existe en el sistema. No se puede agregar la cita." << std::endl;
        return;
    }

    if (fecha.empty()) {
        std::cout << "\n Error: La fecha de la cita no puede estar vacía.\n";
        return;
    }

    // Validar el formato de la fecha (DD-MM-YYYY)
    std::regex fechaRegex(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(fecha, fechaRegex)) {
        std::cout << "\n Error: La fecha de la cita debe estar en el formato YYYY-MM-DD.\n";
        return;
    }
    // Validar que la fecha cumpla con las caracteristicas (Año bisiesto, dia 31 en meses que corresponde...)
    if (!esFechaValida(fecha)) {
        std::cout << "\n Error: La fecha de la cita no es válida.\n";
        return;
    }

    int nuevoId = obtenerMaxIdServicios() + 1;
    auto paciente = std::make_unique<Paciente>(nuevoId, nombrePaciente, "");
    auto nuevoServicio = std::make_unique<Servicio>(nuevoId, std::move(paciente), fecha, descripcion);

    if (guardarServicioEnCSV(*nuevoServicio)) {
        servicios.emplace_back(std::move(nuevoServicio));
        std::cout << "\n\n Servicio agregado correctamente con ID: " << nuevoId << " \n\n";
    }
    else {
        std::cout << "\nError al agregar el servicio. Intente nuevamente.\n";
    }
}

bool Servicio::guardarServicioEnCSV(const Servicio& servicio) {
    std::ofstream archivo("Servicios.csv", std::ios::app);
    if (archivo.is_open()) {
        std::cout << "\n Archivo de citas abierto correctamente.\n";
        // Supongamos que agregas un campo id en la clase Cita
        archivo << servicio.id << "," << servicio.paciente->getNombre() << ","  << servicio.fecha << "," << servicio.descripcion << "\n";
        std::cout << "\n Datos del servicio escritos: | " << servicio.paciente->getNombre() << " | con fecha | " << servicio.fecha << " | motivo | " << servicio.descripcion << "\n\n";
        archivo.close();
        return true;
    }
    else {
        std::cout << "No se pudo abrir el archivo para escribir.\n";
        return false;
    }
}



void Servicio::interfazServicios() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar Servicio\n";
        std::cout << "2. Mostrar Servicio por paciente\n";
        std::cout << "3. Modificar Servicio\n";
        std::cout << "4. Generar BackUp de Servicios\n";
        std::cout << "5. Generar Fichero de Servicios\n";
        std::cout << "6. Generar Fichero de Servicios por Paciente\n";
        std::cout << "7. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            std::string nombrePaciente, fecha, descripcion;
            std::cin.ignore();
            std::cout << "Ingrese nombre del paciente: ";
            std::getline(std::cin, nombrePaciente);
            std::cout << "Ingrese fecha del servicio: ";
            std::getline(std::cin, fecha);
            std::cout << "Ingrese una descripcion: ";
            std::getline(std::cin, descripcion);
            
            Servicio::agregarServicio(nombrePaciente, fecha, descripcion); 
           
            break;
        }
        
        case 10:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}

