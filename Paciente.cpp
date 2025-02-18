#include "Paciente.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <sstream>
#include <stdexcept> // Para std::runtime_error
#include <filesystem>
#include <regex>

std::vector<std::unique_ptr<Paciente>> Paciente::pacientes; // Inicializar el vector de pacientes

Paciente::Paciente(int id, const std::string& nombre, const std::string& fechaIngreso)
    : id(id), nombre(nombre), fechaIngreso(fechaIngreso) {
}

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
                std::cout << "\n Encabezados escritos en el archivo vacio.\n";
            }
            else {
                std::cout << "\n No se pudo abrir el archivo para agregar encabezados.\n";
            }
        }
        else {
            std::cout << "\n El archivo ya existe y no esta vacio, no se escribieron encabezados.\n";
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

void Paciente::buscarPaciente() {
    std::ifstream archivo("Pacientes.csv");
    std::string linea;
    bool encontrado = false;

    // Validar el formato de la fecha (YYYY-MM-DD)
    std::regex fechaRegex(R"(^\d{4}-\d{2}-\d{2}$)");

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    int opcion;
    std::cout << "Seleccione el campo por el que desea buscar:\n";
    std::cout << "1. Buscar por Nombre\n";
    std::cout << "2. Buscar por ID\n";
    std::cout << "3. Buscar por Fecha de Ingreso\n";
    std::cout << "Opcion: ";
    std::cin >> opcion;

    std::string criterioBuscado;
    switch (opcion) {
    case 1:
        std::cout << "Ingrese el nombre a buscar: ";
        std::cin.ignore(); // Limpiar el buffer
        std::getline(std::cin, criterioBuscado);
        break;
    case 2:
        std::cout << "Ingrese el ID a buscar: ";
        std::cin.ignore(); // Limpiar el buffer
        std::getline(std::cin, criterioBuscado);
        break;
    case 3:
        std::cout << "Ingrese la fecha de ingreso a buscar (YYYY-MM-DD): ";
        std::cin.ignore(); // Limpiar el buffer
        std::getline(std::cin, criterioBuscado);

        if (!std::regex_match(criterioBuscado, fechaRegex)) {
            std::cout << "\n Error: La fecha de ingreso debe estar en el formato YYYY-MM-DD.\n";
            return;
        }

        // Validar que la fecha cumpla con las caracteristicas (A�o bisiesto, dia 31 en meses que corresponde...)
        if (!esFechaValida(criterioBuscado)) {
            std::cout << "\n Error: La fecha de ingreso no es valida.\n";
            return;
        }
        break;
    default:
        std::cout << "Opcion no valida." << std::endl;
        archivo.close();
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre, fechaIngreso;

        std::getline(stream, id, ',');
        std::getline(stream, nombre, ',');
        std::getline(stream, fechaIngreso, ',');

        // Se comprueba si coincide con el criterio de b�squeda
        if (nombre == criterioBuscado || id == criterioBuscado || fechaIngreso == criterioBuscado) {
            encontrado = true;
            std::cout << "\n Paciente encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n Paciente no encontrado." << std::endl;
    }

    archivo.close();
}

bool Paciente::esAnioBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

bool Paciente::esFechaValida(const std::string& fecha) {
    int dia, mes, anio;
    char separador1, separador2;

    std::istringstream fechaStream(fecha);
    fechaStream >> anio >> separador1 >> mes >> separador2 >> dia;

    // Verificar que se hayan leido correctamente los separadores
    if (separador1 != '-' || separador2 != '-') {
        return false;
    }

    // Validar el rango de mes
    if (mes < 1 || mes > 12) {
        return false;
    }

    // Validar el rango de dias segun el mes
    switch (mes) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: // Meses con 31 dias
        return (dia >= 1 && dia <= 31);
    case 4: case 6: case 9: case 11: // Meses con 30 dias
        return (dia >= 1 && dia <= 30);
    case 2: // Febrero
        if (esAnioBisiesto(anio)) {
            return (dia >= 1 && dia <= 29); // 29 dias en a�o bisiesto
        }
        else {
            return (dia >= 1 && dia <= 28); // 28 dias en a�o no bisiesto
        }
    default:
        return false; // No deberia llegar aqui
    }
}

void Paciente::agregarPaciente(const std::string& nombre, const std::string& fechaIngreso) {
    try {
        // Validar los datos de entrada
        if (nombre.empty()) {
            std::cout << "\n Error: El nombre no puede estar vacio.\n";
            return;
        }

        if (fechaIngreso.empty()) {
            std::cout << "\n Error: La fecha de ingreso no puede estar vacia.\n";
            return;
        }

        // Validar el formato de la fecha (YYYY-MM-DD)
        std::regex fechaRegex(R"(^\d{4}-\d{2}-\d{2}$)");
        if (!std::regex_match(fechaIngreso, fechaRegex)) {
            std::cout << "\n Error: La fecha de ingreso debe estar en el formato YYYY-MM-DD.\n";
            return;
        }

        // Validar que la fecha cumpla con las caracteristicas (A�o bisiesto, dia 31 en meses que corresponde...)
        if (!esFechaValida(fechaIngreso)) {
            std::cout << "\n Error: La fecha de ingreso no es valida.\n";
            return;
        }

        crearPacientesCSV();

        int nuevoId = obtenerMaxId() + 1;
        auto nuevoPaciente = std::make_unique<Paciente>(nuevoId, nombre, fechaIngreso);

        // Verificar que el nuevo paciente se haya creado correctamente
        if (!nuevoPaciente) {
            std::cout << "\n Error: No se pudo crear el nuevo paciente.\n";
            return;
        }

        // Guardar el paciente en CSV y agregarlo al vector solo si se guardo correctamente
        if (guardarPacienteEnCSV(*nuevoPaciente)) {
            pacientes.emplace_back(std::move(nuevoPaciente));
            std::cout << "\n Paciente agregado correctamente con ID: " << nuevoId << "\n";
        }
        else {
            std::cout << "\n Error al agregar el paciente. Intente nuevamente.\n";
        }

        // Opcional: Limpiar punteros nulos en el vector (si es necesario)
        pacientes.erase(std::remove_if(pacientes.begin(), pacientes.end(),
            [](const std::unique_ptr<Paciente>& ptr) { return ptr == nullptr; }), pacientes.end());
    }
    catch (const std::exception& e) {
        std::cout << "\n Se produjo una excepcion: " << e.what() << "\n";
    }
    catch (...) {
        std::cout << "\n Se produjo una excepcion desconocida.\n";
    }
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

    archivo.close(); // Cerrar el archivo despues de leer

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

    archivo.close(); // Cerrar el archivo despues de leer

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

namespace fs = std::filesystem;

void Paciente::crearBackupPacientesCSV() {

    std::string nombreArchivo = "Pacientes.csv";

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
    std::string carpetaPacientes = "Backups/Pacientes";
    if (!fs::exists(carpetaPacientes)) {
        fs::create_directory(carpetaPacientes);
        std::cout << "Carpeta 'Pacientes' creada." << std::endl;
    }
    else {
        std::cout << "Carpeta 'Pacientes' no hace falta crearla porque ya existe." << std::endl;
    }
    // Se obtiene la fecha y hora actual para el nombre del backup
    std::time_t tiempoActual = std::time(nullptr);
    std::tm* tm = std::localtime(&tiempoActual);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tm);

    
    std::string nombreBackup = carpetaPacientes + "/BCK_Pacientes_" + std::string(buffer) + ".csv";

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

void Paciente::exportarPacientes() {

    std::string nombreCSV = "Pacientes.csv";
    //Nombre del report que se quiere generar
    std::string nombreTXT = "Pacientes_Report.txt";

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
    archivoTXT << "Reporte de Pacientes\n";
    archivoTXT << "=====================\n";
    archivoTXT << "ID\tNombre\tFecha de Ingreso\n"; 

    std::string linea;
    while (std::getline(archivoCSV, linea)) {
        archivoTXT << linea << std::endl; // Escribir cada linea del CSV en el archivo TXT
    }

    std::cout << "Contenido exportado a: " << nombreTXT << std::endl;

    // Cerrar los archivos
    archivoCSV.close();
    archivoTXT.close();
}

void Paciente::buscarPacientesPorFechaIngreso(const std::string& fechaInicio, const std::string& fechaFin) {
    std::ifstream archivo("Pacientes.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }
    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, nombre, fechaIngreso;

        std::getline(stream, id, ','); 
        std::getline(stream, nombre, ','); 
        std::getline(stream, fechaIngreso, ','); 

        if (fechaInicio.empty()) {
            std::cout << "\n Error: La fecha de inicio no puede estar vacia.\n";
            return;
        }

        if (fechaFin.empty()) {
            std::cout << "\n Error: La fecha de inicio no puede estar vacia.\n";
            return;
        }

        // Validar el formato de la fecha (DD-MM-YYYY)
        std::regex fechaRegex(R"(^\d{4}-\d{2}-\d{2}$)");
        if (!std::regex_match(fechaInicio, fechaRegex)) {
            std::cout << "\n Error: La fecha de inicio debe estar en el formato YYYY-MM-DD.\n";
            return;
        }

        if (!std::regex_match(fechaFin, fechaRegex)) {
            std::cout << "\n Error: La fecha fin debe estar en el formato YYYY-MM-DD.\n";
            return;
        }
        // Validar que la fecha cumpla con las caracteristicas (A�o bisiesto, dia 31 en meses que corresponde...)
        if (!esFechaValida(fechaInicio)) {
            std::cout << "\n Error: La fecha de inicio no es valida.\n";
            return;
        }

        // Validar que la fecha cumpla con las caracteristicas (A�o bisiesto, dia 31 en meses que corresponde...)
        if (!esFechaValida(fechaFin)) {
            std::cout << "\n Error: La fecha fin no es valida.\n";
            return;
        }

        // Comparar la fecha de ingreso con el rango
        if (fechaIngreso >= fechaInicio && fechaIngreso <= fechaFin) {
            encontrado = true;
            std::cout << " Paciente encontrado: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron pacientes en el rango de fechas." << std::endl;
    }

    archivo.close();
}

void Paciente::interfazPacientes() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar paciente\n";
        std::cout << "2. Buscar paciente\n";
        std::cout << "3. Eliminar paciente por nombre\n";
        std::cout << "4. Modificar nombre de paciente\n";
        std::cout << "5. Generar BackUp de Pacientes\n";
        std::cout << "6. Generar Fichero de Pacientes\n";
        std::cout << "7. Buscar pacientes entre dos fechas\n";
        std::cout << "8. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        // Control de error para verificar que la entrada es un n�mero
        while (std::cin.fail() || opcion < 1 || opcion > 8 ) {
            std::cin.clear(); // Limpiar el estado de error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            std::cout << "Entrada no valida. Introduce un numero entre 1 y 8: ";
            std::cin >> opcion;
        }

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
            Paciente::buscarPaciente();
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
            Paciente::crearBackupPacientesCSV();
            break;
        case 6:
            Paciente::exportarPacientes();
            break;
        case 7: {
            std::string fechaInicio, fechaFin;
            std::cin.ignore();
            std::cout << "Ingrese la fecha inicio ";
            std::getline(std::cin, fechaInicio);
            std::cout << "Ingrese la fecha fin ";
            std::getline(std::cin, fechaFin);
            Paciente::buscarPacientesPorFechaIngreso(fechaInicio, fechaFin);
            break;
        }
        case 8:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}