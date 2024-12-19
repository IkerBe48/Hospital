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
        //Se agrega el id calculado
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

void Servicio::buscarHistorialClinico(const std::string& nombrePaciente) {
    std::ifstream archivo("Servicios.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    // Escribir encabezado en la terminal
    std::cout << "Historial clinico del Paciente: " + nombrePaciente + "\n";
    std::cout << "=====================\n";
    std::cout << "ID\tPaciente\t\tFecha\tDescripcion\n";

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, fecha, descripcion;

        std::getline(stream, id, ',');
        std::getline(stream, paciente, ',');
        std::getline(stream, fecha, ',');
        std::getline(stream, descripcion);

        // Comparar el nombre del paciente con el nombre introducido
        if (paciente == nombrePaciente) {
            encontrado = true;
            std::cout << id << "\t" << paciente << "\t\t" << fecha << "\t" << descripcion << std::endl; // Mostrar en la terminal
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron servicios para el paciente: " << nombrePaciente << std::endl;
    }
    else {
        std::cout << "\nSe han mostrado los servicios para el paciente: " << nombrePaciente << std::endl;
    }

    archivo.close(); // Cerrar el archivo
}

void Servicio::modificarServiciosDescripcion(const std::string& nombreBuscado, const std::string& nuevaDesc) {
    std::ifstream archivo("Servicios.csv");
    std::vector<std::string> lineas;
    std::string linea;
    std::vector<std::string> registrosEncontrados;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, fecha, descripcion;

        std::getline(stream, id, ','); // Leer ID
        std::getline(stream, paciente, ','); // Leer nombre
        std::getline(stream, fecha, ','); // Leer fecha de ingreso
        std::getline(stream, descripcion); // Leer descripcion

        if (paciente == nombreBuscado) {
            registrosEncontrados.push_back(linea); // Almacenar el registro encontrado
            encontrado = true; // Marcar que encontramos el paciente
        }

        lineas.push_back(id + "," + paciente + "," + fecha + "," + descripcion);
    }

    archivo.close(); // Cerrar el archivo después de leer

    if (!encontrado) {
        std::cout << "\n Paciente | " << nombreBuscado << " | no encontrado." << std::endl;
        return;
    }

    // Mostrar los registros encontrados
    std::cout << "\nSe encontraron los siguientes registros para el paciente: " << nombreBuscado << std::endl;
    for (const auto& registro : registrosEncontrados) {
        std::istringstream stream(registro);
        std::string id, paciente, fecha, descripcion;

        std::getline(stream, id, ',');
        std::getline(stream, paciente, ',');
        std::getline(stream, fecha, ',');
        std::getline(stream, descripcion);

        std::cout << "ID: " << id << ", Paciente: " << paciente << ", Fecha: " << fecha << ", Descripción: " << descripcion << std::endl;
    }

    // Solicitar al usuario que ingrese el ID del registro que desea modificar
    std::string idSeleccionado;
    std::cout << "Por favor, introduce el ID del registro que deseas modificar: ";
    std::cin >> idSeleccionado;

    // Modificar el registro correspondiente al ID ingresado
    bool registroModificado = false;
    for (size_t i = 0; i < lineas.size(); ++i) {
        std::istringstream stream(lineas[i]);
        std::string id, paciente, fecha, descripcion;

        std::getline(stream, id, ',');
        std::getline(stream, paciente, ',');
        std::getline(stream, fecha, ',');
        std::getline(stream, descripcion);

        if (id == idSeleccionado) {
            // Modificar la descripción
            lineas[i] = id + "," + paciente + "," + fecha + "," + nuevaDesc;
            registroModificado = true;
            break; // Salir del bucle una vez que se ha encontrado y modificado el registro
        }
    }

    if (registroModificado) {
        std::ofstream archivoSalida("Servicios.csv");
        if (archivoSalida.is_open()) {
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "\n\n Descripción del Servicio del paciente | " << nombreBuscado << " | modificada a: | " << nuevaDesc << " | correctamente." << std::endl;
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n No se encontró un registro con el ID: " << idSeleccionado << std::endl;
    }
}

namespace fs = std::filesystem;

void Servicio::crearBackupServiciosCSV() {

    std::string nombreArchivo = "Servicios.csv";

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
    std::string carpetaPacientes = "Backups/Servicios";
    if (!fs::exists(carpetaPacientes)) {
        fs::create_directory(carpetaPacientes);
        std::cout << "Carpeta 'Servicios' creada." << std::endl;
    }
    else {
        std::cout << "Carpeta 'Servicios' no hace falta crearla porque ya existe." << std::endl;
    }
    // Se obtiene la fecha y hora actual para el nombre del backup
    std::time_t tiempoActual = std::time(nullptr);
    std::tm* tm = std::localtime(&tiempoActual);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tm);


    std::string nombreBackup = carpetaPacientes + "/BCK_Servicios_" + std::string(buffer) + ".csv";

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

void Servicio::exportarServicios() {

    std::string nombreCSV = "Servicios.csv";
    //Nombre del report que se quiere generar
    std::string nombreTXT = "Servicios_Report.txt";

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
    archivoTXT << "Reporte de Servicios\n";
    archivoTXT << "=====================\n";
    archivoTXT << "ID\tPaciente\t\tFecha\tDescripcion\n"; 

    std::string linea;
    while (std::getline(archivoCSV, linea)) {
        archivoTXT << linea << std::endl; // Escribir cada línea del CSV en el archivo TXT
    }

    std::cout << "Contenido exportado a: " << nombreTXT << std::endl;

    // Cerrar los archivos
    archivoCSV.close();
    archivoTXT.close();
}

void Servicio::exportarServiciosPorPaciente(const std::string& nombrePaciente) {
    std::ifstream archivo("Servicios.csv");
    std::ofstream archivoSalida("Historial_de_" + nombrePaciente + ".txt");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    if (!archivoSalida.is_open()) {
        std::cerr << "\n Error al crear el archivo de salida." << std::endl;
        return;
    }

    // Escribir encabezado
    archivoSalida << "Historial clinico del Paciente: " + nombrePaciente + "\n";
    archivoSalida << "=====================\n";
    archivoSalida << "ID\tPaciente\t\tFecha\tDescripcion\n"; 

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, fecha, descripcion;

        std::getline(stream, id, ','); 
        std::getline(stream, paciente, ',');
        std::getline(stream, fecha, ','); 
        std::getline(stream, descripcion); 

        // Comparar el nombre del médico con el nombre introducido
        if (paciente == nombrePaciente) {
            encontrado = true;
            archivoSalida << linea << std::endl; // Escribir en el archivo de salida
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron servicios para el paciente: " << nombrePaciente << std::endl;
    }
    else {
        std::cout << "\nSe ha generado el reporte con servicios para el paciente: " << nombrePaciente << std::endl;
    }

    archivo.close();
    archivoSalida.close(); // Cerrar el archivo de salida
}

void Servicio::interfazServicios() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar Servicio\n";
        std::cout << "2. Mostrar Historial Clinico por paciente\n";
        std::cout << "3. Modificar descripcion de un Servicio\n";
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
        case 2: {
            std::string nombrePaciente;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente ";
            std::getline(std::cin, nombrePaciente);
            Servicio::buscarHistorialClinico(nombrePaciente);
            break;
        }
        case 3: {
            std::string nombreBuscado, nuevaDesc;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a buscar: ";
            std::getline(std::cin, nombreBuscado);
            std::cout << "Ingrese la nueva descripcion del Servicio: ";
            std::getline(std::cin, nuevaDesc);
           Servicio::modificarServiciosDescripcion(nombreBuscado, nuevaDesc);
            break;
        }
        case 4:
            Servicio::crearBackupServiciosCSV();
            break;
        case 5:
            Servicio::exportarServicios();
            break;
        case 6: {
            std::string nombrePaciente;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del Paciente ";
            std::getline(std::cin, nombrePaciente);
            Servicio::exportarServiciosPorPaciente(nombrePaciente);
            break;
        }
        case 7:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}

