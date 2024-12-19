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

Cita::Cita(int id, std::unique_ptr<Paciente> paciente, std::unique_ptr<Medico> medico, const std::string& fecha, int urgencia)
    : id(id), paciente(std::move(paciente)), medico(std::move(medico)), fecha(fecha), urgencia(urgencia) {
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
        // Se agrega el id calculado
        archivo << cita.id << "," << cita.paciente->getNombre() << "," << cita.medico->getNombre() << "," << cita.fecha << "," << cita.urgencia << "\n";
        std::cout << "\n Datos de la cita escritos: | " << cita.paciente->getNombre() << " | con | " << cita.medico->getNombre() << " en | " << cita.fecha << " | con nivel de urgencia de | " << cita.urgencia << "\n\n";
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

bool Cita::esAnioBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

bool Cita::esFechaValida(const std::string& fecha) {
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

    int nuevoId = obtenerMaxIdCitas() + 1;
    auto paciente = std::make_unique<Paciente>(nuevoId, nombrePaciente, "");
    auto medico = std::make_unique<Medico>(nuevoId, nombreMedico, "");
    auto nuevaCita = std::make_unique<Cita>(nuevoId, std::move(paciente), std::move(medico), fecha, urgencia);

    if (guardarCitaEnCSV(*nuevaCita)) {
        citas.emplace_back(std::move(nuevaCita));
        std::cout << "\n\n Cita agregada correctamente con ID: " << nuevoId << " \n\n";
    }
    else {
        std::cout << "\nError al agregar la cita. Intente nuevamente.\n";
    }
}

void Cita::eliminarCita(const std::string& nombreBuscado) {
    std::ifstream archivo("Citas.csv");
    std::vector<std::string> lineas;
    std::vector<std::string> citasEncontradas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    // Leer encabezados
    std::string encabezado;
    std::getline(archivo, encabezado); // Leer la primera línea (encabezados)
    lineas.push_back(encabezado); // Almacenar encabezados

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente;

        std::getline(stream, id, ','); // Leer ID
        std::getline(stream, paciente, ','); // Leer paciente

        if (paciente != nombreBuscado) {
            lineas.push_back(linea);
        }
        else {
            encontrado = true; // Marcar que encontramos el paciente
            citasEncontradas.push_back(linea); // Almacenar la cita encontrada
        }
    }

    archivo.close(); // Cerrar el archivo después de leer

    if (encontrado) {
        // Mostrar las citas encontradas
        std::cout << "\nCitas encontradas para | " << nombreBuscado << " |:\n";
        for (const auto& cita : citasEncontradas) {
            std::istringstream stream(cita);
            std::string id, paciente;
            std::getline(stream, id, ',');
            std::getline(stream, paciente, ',');
            std::cout << "ID: " << id << ", Paciente: " << paciente << std::endl;
        }

        // Solicitar al usuario que seleccione un ID para eliminar
        std::string idSeleccionado;
        std::cout << "Ingrese el ID de la cita que desea eliminar: ";
        std::cin >> idSeleccionado;

        // Filtrar las citas para eliminar la seleccionada
        std::vector<std::string> citasRestantes; // Para almacenar las citas restantes
        for (const auto& cita : citasEncontradas) {
            std::istringstream stream(cita);
            std::string id, paciente;
            std::getline(stream, id, ',');
            std::getline(stream, paciente, ',');

            if (id != idSeleccionado) {
                citasRestantes.push_back(cita); // Mantener la cita si no es la seleccionada
            }
        }

        // Escribir las citas restantes de nuevo en el archivo
        std::ofstream archivoSalida("Citas.csv");
        if (archivoSalida.is_open()) {
            // Escribir encabezados
            archivoSalida << encabezado << "\n";
            // Escribir citas restantes
            for (const auto& l : citasRestantes) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
            std::cout << "\nCita con ID | " << idSeleccionado << " | eliminada correctamente." << std::endl;
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n Cita de | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Cita::modificarFechaCita(const std::string& nombreBuscado, const std::string& nuevaFecha) {
    std::ifstream archivo("Citas.csv");
    std::vector<std::string> lineas;
    std::vector<std::string> citasEncontradas;
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    // Validar el formato de la fecha (YYYY-MM-DD)
    std::regex fechaRegex(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(nuevaFecha, fechaRegex)) {
        std::cout << "\n Error: La fecha de la cita debe estar en el formato YYYY-MM-DD.\n";
        return;
    }

    // Validar que la fecha cumpla con las características (Año bisiesto, día 31 en meses que corresponde...)
    if (!esFechaValida(nuevaFecha)) {
        std::cout << "\n Error: La fecha de la cita no es válida.\n";
        return;
    }

    // Leer el archivo y buscar citas
    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, medico, fechaEntrada, urgencia;

        std::getline(stream, id, ','); // Leer ID
        std::getline(stream, paciente, ','); // Leer paciente
        std::getline(stream, medico, ','); // Leer médico
        std::getline(stream, fechaEntrada, ','); // Leer fecha de entrada
        std::getline(stream, urgencia); // Leer urgencia
        if (paciente == nombreBuscado) {
            citasEncontradas.push_back(linea); // Almacenar la cita encontrada
            encontrado = true; // Marcar que encontramos el paciente
        }

        lineas.push_back(linea); // Almacenar la línea original
    }

    archivo.close(); // Cerrar el archivo después de leer

    if (encontrado) {
        // Mostrar las citas encontradas
        std::cout << "\nCitas encontradas para | " << nombreBuscado << " |:\n";
        for (const auto& cita : citasEncontradas) {
            std::istringstream stream(cita);
            std::string id, paciente, medico, fechaEntrada, urgencia;
            std::getline(stream, id, ',');
            std::getline(stream, paciente, ',');
            std::getline(stream, medico, ',');
            std::getline(stream, fechaEntrada, ',');
            std::getline(stream, urgencia, ',');
            std::cout << "ID: " << id << ", Paciente: " << paciente << ", Médico: " << medico << ", Fecha: " << fechaEntrada << ", Urgencia: " << urgencia << std::endl;
        }

        // Solicitar al usuario que seleccione un ID para modificar
        std::string idSeleccionado;
        std::cout << "Ingrese el ID de la cita que desea modificar: ";
        std::cin >> idSeleccionado;

        // Modificar la fecha de la cita seleccionada
        for (auto& cita : lineas) {
            std::istringstream stream(cita);
            std::string id, paciente, medico, fechaEntrada, urgencia;
            std::getline(stream, id, ',');
            std::getline(stream, paciente, ',');
            std::getline(stream, medico, ',');
            std::getline(stream, fechaEntrada, ',');
            std::getline(stream, urgencia, ',');

            if (id == idSeleccionado) {
                fechaEntrada = nuevaFecha; // Cambiar la fecha a la nueva
                cita = id + "," + paciente + "," + medico + "," + fechaEntrada + "," + urgencia; // Actualizar la línea
                std::cout << "\nLa cita con ID | " << idSeleccionado << " | ha sido modificada a la nueva fecha: | " << nuevaFecha << " | correctamente." << std::endl;
            }
        }

        // Escribir las citas actualizadas de nuevo en el archivo
        std::ofstream archivoSalida("Citas.csv");
        if (archivoSalida.is_open()) {
            // Escribir todas las líneas (incluyendo encabezados)
            for (const auto& l : lineas) {
                archivoSalida << l << "\n";
            }
            archivoSalida.close();
        }
        else {
            std::cerr << "\n Error al abrir el archivo para escribir." << std::endl;
        }
    }
    else {
        std::cout << "\n Cita del paciente | " << nombreBuscado << " | no encontrado." << std::endl;
    }
}

void Cita::buscarCitasPorFecha(const std::string& fechaInicio, const std::string& fechaFin) {
    std::ifstream archivo("Citas.csv");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }
    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente,medico, fecha;

        std::getline(stream, id, ','); 
        std::getline(stream, paciente, ','); 
        std::getline(stream, medico, ','); 
        std::getline(stream, fecha, ','); 

        if (fechaInicio.empty()) {
            std::cout << "\n Error: La fecha de inicio no puede estar vacía.\n";
            return;
        }

        if (fechaFin.empty()) {
            std::cout << "\n Error: La fecha de inicio no puede estar vacía.\n";
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
        // Validar que la fecha cumpla con las caracteristicas (Año bisiesto, dia 31 en meses que corresponde...)
        if (!esFechaValida(fechaInicio)) {
            std::cout << "\n Error: La fecha de inicio no es válida.\n";
            return;
        }

        // Validar que la fecha cumpla con las caracteristicas (Año bisiesto, dia 31 en meses que corresponde...)
        if (!esFechaValida(fechaFin)) {
            std::cout << "\n Error: La fecha fin no es válida.\n";
            return;
        }

        // Comparar la fecha de ingreso con el rango
        if (fecha >= fechaInicio && fecha <= fechaFin) {
            encontrado = true;
            std::cout << "\nCita encontrada: " << linea << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron citas en el rango de fechas." << std::endl;
    }

    archivo.close();
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
    archivoTXT << "ID\tPaciente\tMedico\tFecha de entrada\tUrgencia\n"; 

    std::string linea;
    while (std::getline(archivoCSV, linea)) {
        archivoTXT << linea << std::endl; // Escribir cada línea del CSV en el archivo TXT
    }

    std::cout << "Contenido exportado a: " << nombreTXT << std::endl;

    // Cerrar los archivos
    archivoCSV.close();
    archivoTXT.close();
}

void Cita::exportarCitasPorFecha(const std::string& fechaInicio, const std::string& fechaFin) {
    std::ifstream archivo("Citas.csv");
//Nombre del report que se quiere generar
    std::ofstream archivoSalida("Citas_" + fechaInicio + "_" + fechaFin + ".txt");
    std::string linea;
    bool encontrado = false;

    if (!archivo.is_open()) {
        std::cerr << "\n Error al abrir el archivo." << std::endl;
        return;
    }

    // Crear o abrir el archivo de salida
    
    if (!archivoSalida.is_open()) {
        std::cerr << "\n Error al crear el archivo de salida." << std::endl;
        return;
    }
    // Escribir encabezado
    archivoSalida << "Reporte de Citas entre " + fechaInicio + " y " + fechaFin+"\n";
    archivoSalida << "=====================\n";
    archivoSalida << "ID\tPaciente\tMedico\tFecha de entrada\tUrgencia\n";

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, medico, fecha;

        std::getline(stream, id, ','); 
        std::getline(stream, paciente, ',');
        std::getline(stream, medico, ','); 
        std::getline(stream, fecha, ','); 

        if (fechaInicio.empty()) {
            std::cout << "\n Error: La fecha de inicio no puede estar vacía.\n";
            return;
        }

        if (fechaFin.empty()) {
            std::cout << "\n Error: La fecha de fin no puede estar vacía.\n";
            return;
        }

        // Validar el formato de la fecha (YYYY-MM-DD)
        std::regex fechaRegex(R"(^\d{4}-\d{2}-\d{2}$)");
        if (!std::regex_match(fechaInicio, fechaRegex)) {
            std::cout << "\n Error: La fecha de inicio debe estar en el formato YYYY-MM-DD.\n";
            return;
        }

        if (!std::regex_match(fechaFin, fechaRegex)) {
            std::cout << "\n Error: La fecha fin debe estar en el formato YYYY-MM-DD.\n";
            return;
        }

        // Validar que la fecha cumpla con las características (Año bisiesto, día 31 en meses que corresponde...)
        if (!esFechaValida(fechaInicio)) {
            std::cout << "\n Error: La fecha de inicio no es válida.\n";
            return;
        }

        if (!esFechaValida(fechaFin)) {
            std::cout << "\n Error: La fecha fin no es válida.\n";
            return;
        }

        // Comparar la fecha de ingreso con el rango
        if (fecha >= fechaInicio && fecha <= fechaFin) {
            encontrado = true;
            archivoSalida << linea << std::endl; // Escribir en el archivo de salida
            std::cout << "\n Se ha generado el reporte con citas en el rango de fechas." << std::endl;
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron citas en el rango de fechas." << std::endl;
    }

    archivo.close();
    archivoSalida.close(); // Cerrar el archivo de salida
}

void Cita::exportarCitasPorMedico(const std::string& nombreMedico) {
    std::ifstream archivo("Citas.csv");
    std::ofstream archivoSalida("Citas_de_" + nombreMedico + ".txt");
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
    archivoSalida << "Reporte de Citas para el Médico: " + nombreMedico + "\n";
    archivoSalida << "=====================\n";
    archivoSalida << "ID\tPaciente\tMedico\tFecha de entrada\tUrgencia\n"; 

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, medico, fecha, urgencia;

        std::getline(stream, id, ','); 
        std::getline(stream, paciente, ',');
        std::getline(stream, medico, ','); 
        std::getline(stream, fecha, ','); 
        std::getline(stream, urgencia); 

        // Comparar el nombre del médico con el nombre introducido
        if (medico == nombreMedico) {
            encontrado = true;
            archivoSalida << linea << std::endl; // Escribir en el archivo de salida
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron citas para el médico: " << nombreMedico << std::endl;
    }
    else {
        std::cout << "\n Se ha generado el reporte con citas para el médico: " << nombreMedico << std::endl;
    }

    archivo.close();
    archivoSalida.close(); // Cerrar el archivo de salida
}
void Cita::exportarCitasCronicas() {
    std::ifstream archivo("Citas.csv");
    std::ofstream archivoSalida("Citas_Pacientes_Cronicos.txt");
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
    archivoSalida << "Reporte de Citas de Pacientes Cronicos.\n";
    archivoSalida << "=====================\n";
    archivoSalida << "ID\tPaciente\tMedico\tFecha de entrada\tUrgencia\n"; 

    while (std::getline(archivo, linea)) {
        std::istringstream stream(linea);
        std::string id, paciente, medico, fecha, urgencia;

        std::getline(stream, id, ','); 
        std::getline(stream, paciente, ','); 
        std::getline(stream, medico, ',');
        std::getline(stream, fecha, ','); 
        std::getline(stream, urgencia); 

        // Comparar el nombre del médico con el nombre introducido
        if (urgencia == "3") {
            encontrado = true;
            archivoSalida << linea << std::endl; // Escribir en el archivo de salida
        }
    }

    if (!encontrado) {
        std::cout << "\n No se encontraron citas con Pacientes cronicos." << std::endl;
    }
    else {
        std::cout << "\n Se ha generado el reporte de citas con Pacientes cronicos. " << std::endl;
    }

    archivo.close();
    archivoSalida.close(); // Cerrar el archivo de salida
}

void Cita::interfazCitas() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Agregar Cita\n";
        std::cout << "2. Mostrar Citas por fecha\n";
        std::cout << "3. Eliminar Cita por Paciente\n";
        std::cout << "4. Modificar fecha de Cita\n";
        std::cout << "5. Generar BackUp de Citas\n";
        std::cout << "6. Generar Fichero de Citas\n";
        std::cout << "7. Generar Fichero de Citas en rango de fechas\n";
        std::cout << "8. Generar Fichero de Citas por Medico\n";
        std::cout << "9. Generar Fichero de Pacientes graves\n";
        std::cout << "10. Salir\n";
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
        case 2: {
            std::string fechaInicio, fechaFin;
            std::cin.ignore();
            std::cout << "Ingrese la fecha inicio ";
            std::getline(std::cin, fechaInicio);
            std::cout << "Ingrese la fecha fin ";
            std::getline(std::cin, fechaFin);
            Cita::buscarCitasPorFecha(fechaInicio, fechaFin);
            break;
        }
        case 3: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente que quieras eliminar su cita: ";
            std::getline(std::cin, nombreBuscado);
            Cita::eliminarCita(nombreBuscado);
            break;
        }
        case 4: {
            std::string nombreBuscado, nuevaFecha;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a buscar: ";
            std::getline(std::cin, nombreBuscado);
            std::cout << "Ingrese la nueva fecha de la cita (YYYY-MM-DD): ";
            std::getline(std::cin, nuevaFecha);
            Cita::modificarFechaCita(nombreBuscado, nuevaFecha);
            break;
        }
        case 5:
            Cita::crearBackupCitasCSV();
            break;
        case 6:
            Cita::exportarCitas();
            break;
        case 7: {
            std::string fechaInicio, fechaFin;
            std::cin.ignore();
            std::cout << "Ingrese la fecha inicio ";
            std::getline(std::cin, fechaInicio);
            std::cout << "Ingrese la fecha fin ";
            std::getline(std::cin, fechaFin);
            Cita::exportarCitasPorFecha(fechaInicio, fechaFin);
            break;
        }
        case 8: {
            std::string nombreMedico;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del Medico ";
            std::getline(std::cin, nombreMedico);
            Cita::exportarCitasPorMedico(nombreMedico);
            break;
        }
        case 9: {
            Cita::exportarCitasCronicas();
            break;
        }
        case 10:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}