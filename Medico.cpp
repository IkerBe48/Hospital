#include "Medico.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <sstream>
#include <filesystem>

std::vector<std::unique_ptr<Medico>> Medico::medicos; // Inicializar el vector de medicos

Medico::Medico(int id, const std::string& nombre, const std::string& especialidad, const std::string& disponible)
    : id(id), nombre(nombre), especialidad(especialidad), disponible (disponible){
}

void Medico::crearMedicosCSV() {
    std::ifstream archivo("Medicos.csv");

    if (!archivo) {
        std::ofstream archivoSalida("Medicos.csv");
        if (archivoSalida.is_open()) {
            archivoSalida << "ID,Nombre,Especialidad,Disponible\n"; 
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
                archivoSalida << "ID,Nombre,Especialidad,Disponible\n";
                std::cout << "\n Encabezados escritos en el archivo vacio.\n";
                archivoSalida.close();
            }
            else {
                std::cout << "\n No se pudo abrir el archivo para agregar encabezados.\n";
            }
        }
        else {
            std::cout << "\n El archivo ya existe y no esta vacio, no se escribieron encabezados.\n";
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
        archivo << medico.id << "," << medico.nombre << "," << medico.especialidad << "," << medico.disponible << "\n";
        std::cout << "\n\n Datos del Medico escritos: | " << medico.id << " | " << medico.nombre << " | " << medico.especialidad << "," << medico.disponible << " |\n";
        archivo.close();
        return true;
    }
    else {
        std::cout << "\n No se pudo abrir el archivo para escribir.\n";
        return false;
    }
}

void Medico::buscarMedico() {
    std::ifstream archivo("Medicos.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    int opcion;
    std::cout << "Seleccione el campo por el que desea buscar:\n";
    std::cout << "1. Nombre\n";
    std::cout << "2. Especialidad\n";
    std::cout << "3. Disponibilidad (S para disponible, N para no disponible)\n";
    std::cout << "Opcion: ";
    std::cin >> opcion;

    std::string criterioBuscado;
    std::string disponibilidadBuscada;

    switch (opcion) {
    case 1:
        std::cout << "Ingrese el nombre a buscar: ";
        std::cin.ignore(); // Limpiar el buffer
        std::getline(std::cin, criterioBuscado);
        break;
    case 2:
        std::cout << "Ingrese la especialidad a buscar: ";
        std::cin.ignore(); // Limpiar el buffer
        std::getline(std::cin, criterioBuscado);
        break;
    case 3:
        std::cout << "Ingrese la disponibilidad a buscar (S para disponible, N para no disponible): ";
        std::cin >> disponibilidadBuscada;
        // Convertir a may�sculas para evitar problemas de entrada
        for (auto& c : disponibilidadBuscada) c = toupper(c);

        // Validar la entrada
        while (disponibilidadBuscada != "S" && disponibilidadBuscada != "N") {
            std::cout << "Entrada no valida. Por favor, ingrese 'S' para disponible o 'N' para no disponible: ";
            std::cin >> disponibilidadBuscada;

            // Convertir a may�sculas nuevamente
            for (auto& c : disponibilidadBuscada) c = toupper(c);
        }
        break;
    default:
        std::cout << "Opcion no v�lida." << std::endl;
        archivo.close();
        return;
    }
    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre, especialidad, disponible;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');
        std::getline(stream, especialidad, ',');
        std::getline(stream, disponible, ',');

        if ((opcion == 1 && nombre == criterioBuscado) ||
            (opcion == 2 && especialidad == criterioBuscado) ||
            (opcion == 3 && disponible == disponibilidadBuscada)) {
            encontrado = true;
            std::cout << "\n\n Medico encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n Medico no encontrado." << std::endl;
    }

    archivo.close();
}

void Medico::agregarMedico(const std::string& nombre, const std::string& especialidad, const std::string& disponible) {
    crearMedicosCSV();

    int nuevoId = obtenerMaxIdMedicos() + 1;
    auto nuevoMedico = std::make_unique<Medico>(nuevoId, nombre, especialidad, disponible);
    if (guardarMedicoEnCSV(*nuevoMedico)) {
        medicos.emplace_back(std::move(nuevoMedico));
        std::cout << "\n\n Medico agregado correctamente con ID: " << nuevoId << "\n";
    }
    else {
        std::cout << "\n Error al agregar el medico. Intente nuevamente.\n";
    }
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
        std::string id, nombre, especialidad, disponible;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');
        std::getline(stream, especialidad, ',');
        std::getline(stream, disponible);

        if (nombre == nombreBuscado) {
            nombre = nuevoNombre;
            encontrado = true;
        }

        lineas.push_back(id + "," + nombre + "," + especialidad + "," + disponible);
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

void Medico::buscarMedicoPorEspecialidad(const std::string& especialidadbuscada) {
    std::ifstream archivo("Medicos.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre, especialidad, disponible;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');
        std::getline(stream, especialidad, ',');
        std::getline(stream, disponible);

        if (especialidad == especialidadbuscada) {
            encontrado = true;
            std::cout << "\n\n Medico encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n Medico no encontrado." << std::endl;
    }

    archivo.close();
}

namespace fs = std::filesystem;

void Medico::crearBackupMedicosCSV() {

    std::string nombreArchivo = "Medicos.csv";

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
    std::string carpetaPacientes = "Backups/Medicos";
    if (!fs::exists(carpetaPacientes)) {
        fs::create_directory(carpetaPacientes);
        std::cout << "Carpeta 'Medicos' creada." << std::endl;
    }
    else {
        std::cout << "Carpeta 'Medicos' no hace falta crearla porque ya existe." << std::endl;
    }
    // Se obtiene la fecha y hora actual para el nombre del backup
    std::time_t tiempoActual = std::time(nullptr);
    std::tm* tm = std::localtime(&tiempoActual);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tm);


    std::string nombreBackup = carpetaPacientes + "/BCK_Medicos_" + std::string(buffer) + ".csv";

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

void Medico::exportarMedicos() {

    std::string nombreCSV = "Medicos.csv";
    //Nombre del report que se quiere generar
    std::string nombreTXT = "Medicos_Report.txt";

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
    archivoTXT << "Reporte de Medicos\n";
    archivoTXT << "=====================\n";
    archivoTXT << "ID\tNombre\tEspecialidad\tDisponible\n"; 

    std::string linea;
    while (std::getline(archivoCSV, linea)) {
        archivoTXT << linea << std::endl; // Escribir cada linea del CSV en el archivo TXT
    }

    std::cout << "Contenido exportado a: " << nombreTXT << std::endl;

    // Cerrar los archivos
    archivoCSV.close();
    archivoTXT.close();
}


void Medico::interfazMedicos() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar medico\n";
        std::cout << "2. Buscar medico por nombre, especialidad o disponibilidad\n";
        std::cout << "3. Eliminar medico por nombre\n";
        std::cout << "4. Modificar nombre de medico\n";
        std::cout << "5. Generar BackUp de Medicos\n";
        std::cout << "6. Generar Fichero de Medicos\n";
        std::cout << "7. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        // Control de error para verificar que la entrada es un n�mero
        while (std::cin.fail() || opcion < 1 || opcion > 7) {
            std::cin.clear(); // Limpiar el estado de error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            std::cout << "Entrada no valida. Introduce un numero entre 1 y 7: ";
            std::cin >> opcion;
        }

        switch (opcion) {
        case 1: {
            std::string nombre, especialidad, disponible;
            std::cin.ignore();
            std::cout << "Ingrese nombre del medico: ";
            std::getline(std::cin, nombre);
            std::cout << "Ingrese la especialidad del medico: ";
            std::getline(std::cin, especialidad);
            std::cout << "Ingrese la disponibilidad del medico (S/N): ";
            std::cin >> disponible;

            Medico::agregarMedico(nombre, especialidad, disponible);
            break;
        }
        case 2: {
         
            Medico::buscarMedico();
            break;
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
            Medico::crearBackupMedicosCSV();
            break;
        case 6:
            Medico::exportarMedicos();
            break;
        case 7:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}