#include "Cita.h"
#include "Paciente.h" 
#include "Medico.h"   
#include <vector>
#include <fstream> // Para manejar archivos
#include <memory>  // Para usar unique_ptr
#include <string>  // Para usar std::string y getline
#include <limits>  // Para usar std::numeric_limits
#include <sstream> // Para usar std::istringstream
#include <filesystem>
#include <regex>

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
namespace fs = std::filesystem;

void Cita::crearBackupCitasCSV() {

    std::string nombreArchivo = "Citas.csv";

    // Verificar si el archivo CSV existe
    if (!fs::exists(nombreArchivo)) {
        std::cerr << "El archivo " << nombreArchivo << " no existe." << std::endl;
        return;
    }

    // Crear la carpeta "Backups" 
    std::string carpetaBackup = "Backups";
    if (!fs::exists(carpetaBackup)) {
        fs::create_directory(carpetaBackup);
        std::cout << "Carpeta 'Backups' creada." << std::endl;
    }
    else {
        std::cout << "Carpeta 'Backups' no hace falta crearla porque ya existe." << std::endl;
    }

    // Crear la carpeta "Pacientes" dentro de la carpeta anterior 
    std::string carpetaPacientes = "Backups/Citas";
    if (!fs::exists(carpetaPacientes)) {
        fs::create_directory(carpetaPacientes);
        std::cout << "Carpeta 'Citas' creada." << std::endl;
    }
    else {
        std::cout << "Carpeta 'Citas' no hace falta crearla porque ya existe." << std::endl;
    }
    // Se obtiene la fecha y hora actual para el nombre del backup
    std::time_t tiempoActual = std::time(nullptr);
    std::tm* tm = std::localtime(&tiempoActual);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tm);


    std::string nombreBackup = carpetaPacientes + "/BCK_Citas_" + std::string(buffer) + ".csv";

    // Aqui se lee el contenido del csv original y se escribe en el archivo de backup
    std::ifstream archivoOriginal(nombreArchivo);
    std::ofstream archivoBackup(nombreBackup);

    if (!archivoBackup) {
        std::cerr << "No se pudo crear el archivo de backup." << std::endl;
        return;
    }
    std::string linea;
    while (std::getline(archivoOriginal, linea)) {
        archivoBackup << linea << std::endl;
    }

    std::cout << "Backup creado: " << nombreBackup << std::endl;

    //Se cierran los archivos
    archivoOriginal.close();
    archivoBackup.close();
}

void Cita::exportarCitas() {

    std::string nombreCSV = "Citas.csv";
    //Nombre del report que se quiere generar
    std::string nombreTXT = "Citas_Report.txt";

    // Verificar si el CSV de Pacientes existe
    if (!fs::exists(nombreCSV)) {
        std::cerr << "El archivo " << nombreCSV << " no existe." << std::endl;
        return;
    }
    // Abrir el archivo CSV para lectura
    std::ifstream archivoCSV(nombreCSV);
    // Abrir el archivo TXT para escritura
    std::ofstream archivoTXT(nombreTXT);

    if (!archivoTXT) {
        std::cerr << "No se pudo crear el archivo de texto." << std::endl;
        return;
    }

    // Escribir encabezado
    archivoTXT << "Reporte de Citas\n";
    archivoTXT << "=====================\n";
    archivoTXT << "ID\tPaciente\tMedico\tFecha de entrada\tUrgencia\n"; // Suponiendo que esas son las columnas

    std::string linea;
    while (std::getline(archivoCSV, linea)) {
        archivoTXT << linea << std::endl; // Escribir cada línea del CSV en el archivo TXT
    }

    std::cout << "Contenido exportado a: " << nombreTXT << std::endl;

    // Cerrar los archivos
    archivoCSV.close();
    archivoTXT.close();
}

void Cita::interfazCitas() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar Cita\n";
        std::cout << "2. Buscar Cita por nombre (PENDIENTE)\n";
        std::cout << "3. Eliminar Cita por nombre (PENDIENTE)\n";
        std::cout << "4. Modificar Cita (PENDIENTE)\n";
        std::cout << "5. Generar BackUp de Citas\n";
        std::cout << "6. Generar Fichero de Citas\n";
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
            Cita::crearBackupCitasCSV();
        case 6:
            Cita::exportarCitas();
        case 7:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}