// Hospital.cpp: define el punto de entrada de la aplicación.
//

#include "Hospital.h"
#include <iostream>
#include <vector>
#include <fstream> // Para manejar archivos
#include <memory>  // Para usar unique_ptr
#include <string>  // Para usar std::string y getline
#include <limits>  // Para usar std::numeric_limits
#include <sstream> // Para usar std::istringstream

using namespace std;

// Clase Paciente
class Paciente {
public:

    int id;
    string nombre;
    string fechaIngreso;
    string historialClinico;

    Paciente(int id, string nombre, string fechaIngreso)
        : id(id), nombre(nombre), fechaIngreso(fechaIngreso), historialClinico("") {
    }

    void agregarHistorial(string info) {
        historialClinico += info + "\n";
    }

    void mostrarInfo() const {
        cout << "Nombre: " << nombre << ", ID: " << id << ", Fecha de Ingreso: " << fechaIngreso << "\n";
        cout << "Historial Clínico: " << historialClinico << "\n";
    }

};

// Clase Medico
class Medico {
public:

    int id;
    string nombre;
    string especialidad;
   // bool disponible; Comentado por el momento
    Medico(int id, string nombre, string especialidad)
        : id(id), nombre(nombre), especialidad(especialidad) {
    }
};

// Clase Cita
class Cita {
public:
    unique_ptr<Paciente> paciente;
    unique_ptr<Medico> medico;
    string fecha;
    int urgencia; // 1: baja, 2: media, 3: alta

    Cita(unique_ptr<Paciente> paciente, unique_ptr<Medico> medico, const string& fecha, int urgencia)
        : paciente(move(paciente)), medico(move(medico)), fecha(fecha), urgencia(urgencia) {
    }

};

// Clase Hospital
class Hospital {
private:
    vector<unique_ptr<Paciente>> pacientes;
    vector<unique_ptr<Medico>> medicos;
    vector<unique_ptr<Cita>> citas;

    //INICIO METODOS PACIENTES//
    void crearPacientesCSV() {
        // Intentar abrir el archivo en modo lectura
        ifstream archivo("Pacientes.csv");

        // Si el archivo no existe, lo creamos y escribimos los encabezados
        if (!archivo) {
            ofstream archivoSalida("Pacientes.csv");
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; // Escribir encabezados
                cout << "Archivo creado y encabezados escritos.\n"; // Mensaje de depuración
                archivoSalida.close();
            }
            else {
                cout << "No se pudo abrir el archivo para crear.\n";
            }
        }
        else {
            // Verificar si el archivo está vacío
            if (archivo.peek() == std::ifstream::traits_type::eof()) {
                // Si está vacío, escribir el encabezado
                ofstream archivoSalida("Pacientes.csv", ios::app);
                if (archivoSalida.is_open()) {
                    archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; // Escribir encabezados
                    cout << "Encabezados escritos en el archivo vacío.\n"; // Mensaje de depuración
                    archivoSalida.close();
                }
                else {
                    cout << "No se pudo abrir el archivo para agregar encabezados.\n";
                }
            }
            else {
                cout << "El archivo ya existe y no está vacío, no se escribieron encabezados.\n"; // Mensaje si el archivo ya existe y no está vacío
            }
        }

        archivo.close(); // Cerrar el archivo si se abrió
    }

    int obtenerMaxId() {
        ifstream archivo("Pacientes.csv");
        string linea;
        int maxId = 0;

        // Leer el archivo línea por línea
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find("ID") != string::npos) {
                continue; // Ignorar la línea de encabezado y líneas vacías
            }
            int id;
            sscanf(linea.c_str(), "%d", &id); // Extraer el ID de la línea
            if (id > maxId) {
                maxId = id; // Actualizar maxId si se encuentra un ID mayor
            }
        }

        archivo.close();
        return maxId;
    }

    bool guardarPacienteEnCSV(const Paciente& paciente) {
    ofstream archivo("Pacientes.csv", ios::app); 
        if (archivo.is_open()) {
            cout << "Archivo abierto correctamente.\n"; 
            // Escribir encabezados solo si el archivo está vacío
            /*if (archivo.tellp() == 0) {
                archivo << "ID,Nombre,Fecha de Ingreso\n";
                cout << "Encabezados escritos.\n"; // Mensaje de depuración 
            }*/
            archivo << paciente.id << "," << paciente.nombre << "," << paciente.fechaIngreso << "\n";
            cout << "Datos del paciente escritos: " << paciente.id << ", " << paciente.nombre << ", " << paciente.fechaIngreso << "\n"; // Mensaje de depuración
            archivo.close();
            return true; // Indica que se guardó correctamente
        } else {
            cout << "No se pudo abrir el archivo para escribir.\n";
            return false; // Indica que hubo un error al abrir el archivo
        }
    }

    void buscarPaciente(const std::string& nombreBuscado) {
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

        // Leer la primera columna (ID) y descartarla
        std::getline(stream, id, ','); // Asumiendo que el ID es la primera columna
        // Leer la segunda columna (nombre)
        std::getline(stream, nombre, ','); // Asumiendo que el nombre es la segunda columna

        if (nombre == nombreBuscado) {
            encontrado = true;
            std::cout << "Paciente encontrado: " << linea << std::endl;
            // No se utiliza 'break' para seguir buscando en el archivo
        }
    }

    if (!encontrado) {
        std::cout << "Paciente no encontrado." << std::endl;
    }

    archivo.close();
}
    //FIN METODOS PACIENTES//
    
    //INICIO METODOS MEDICOS//
    void crearMedicosCSV() {
        // Intentar abrir el archivo en modo lectura
        ifstream archivo("Medicos.csv");

        // Si el archivo no existe, lo creamos y escribimos los encabezados
        if (!archivo) {
            ofstream archivoSalida("Medicos.csv");
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Nombre,Especialidad\n"; // Escribir encabezados
                cout << "Archivo creado y encabezados escritos.\n"; // Mensaje de depuración
                archivoSalida.close();
            }
            else {
                cout << "No se pudo abrir el archivo para crear.\n";
            }
        }
        else {
            // Verificar si el archivo está vacío
            if (archivo.peek() == std::ifstream::traits_type::eof()) {
                // Si está vacío, escribir el encabezado
                ofstream archivoSalida("Medicos.csv", ios::app);
                if (archivoSalida.is_open()) {
                    archivoSalida << "ID,Nombre,Especialidad\n"; // Escribir encabezados
                    cout << "Encabezados escritos en el archivo vacío.\n"; // Mensaje de depuración
                    archivoSalida.close();
                }
                else {
                    cout << "No se pudo abrir el archivo para agregar encabezados.\n";
                }
            }
            else {
                cout << "El archivo ya existe y no está vacío, no se escribieron encabezados.\n"; // Mensaje si el archivo ya existe y no está vacío
            }
        }

        archivo.close(); // Cerrar el archivo si se abrió
    }

    int obtenerMaxIdMedicos() {
        ifstream archivo("Medicos.csv");
        string linea;
        int maxId = 0;

        // Leer el archivo línea por línea
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find("ID") != string::npos) {
                continue; // Ignorar la línea de encabezado y líneas vacías
            }
            int id;
            sscanf(linea.c_str(), "%d", &id); // Extraer el ID de la línea
            if (id > maxId) {
                maxId = id; // Actualizar maxId si se encuentra un ID mayor
            }
        }

        archivo.close();
        return maxId;
    }

    bool guardarMedicoEnCSV(const Medico& medico) {
        ofstream archivo("Medicos.csv", ios::app);
        if (archivo.is_open()) {
            cout << "Archivo abierto correctamente.\n";
            // Escribir encabezados solo si el archivo está vacío
            /*if (archivo.tellp() == 0) {
                archivo << "ID,Nombre,Fecha de Ingreso\n";
                cout << "Encabezados escritos.\n"; // Mensaje de depuración
            }*/
            archivo << medico.id << "," << medico.nombre << "," << medico.especialidad << "\n";
            cout << "Datos del Medico escritos: " << medico.id << ", " << medico.nombre << ", " << medico.especialidad << "\n"; // Mensaje de depuración
            archivo.close();
            return true; // Indica que se guardó correctamente
        }
        else {
            cout << "No se pudo abrir el archivo para escribir.\n";
            return false; // Indica que hubo un error al abrir el archivo
        }
    }

    void buscarMedico(const std::string& nombreBuscado) {
        std::ifstream archivo("Medicos.csv");
        std::string linea;
        bool encontrado = false;

        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo." << std::endl;
            return;
        }

        while (std::getline(archivo, linea)) {
            std::istringstream stream(linea);
            std::string id, nombre;

            // Leer la primera columna (ID) y descartarla
            std::getline(stream, id, ','); // Asumiendo que el ID es la primera columna
            // Leer la segunda columna (nombre)
            std::getline(stream, nombre, ','); // Asumiendo que el nombre es la segunda columna

            if (nombre == nombreBuscado) {
                encontrado = true;
                std::cout << "Medico encontrado: " << linea << std::endl;
                // No se utiliza 'break' para seguir buscando en el archivo
            }
        }

        if (!encontrado) {
            std::cout << "Medico no encontrado." << std::endl;
        }

        archivo.close();
    }
    //FIN METODOS MEDICOS//

    //INICIO METODOS CITAS//
    void crearCitasCSV() {
        // Intentar abrir el archivo en modo lectura
        ifstream archivo("Citas.csv");

        // Si el archivo no existe, lo creamos y escribimos los encabezados
        if (!archivo) {
            ofstream archivoSalida("Citas.csv");
            if (archivoSalida.is_open()) {
                archivoSalida << "ID,Paciente,Medico,fecha de entrada,urgencia\n"; // Escribir encabezados
                cout << "Archivo creado y encabezados escritos.\n"; // Mensaje de depuración
                archivoSalida.close();
            }
            else {
                cout << "No se pudo abrir el archivo para crear.\n";
            }
        }
        else {
            // Verificar si el archivo está vacío
            if (archivo.peek() == std::ifstream::traits_type::eof()) {
                // Si está vacío, escribir el encabezado
                ofstream archivoSalida("Citas.csv", ios::app);
                if (archivoSalida.is_open()) {
                    archivoSalida << "ID,Paciente,Medico,fecha de entrada,urgencia\n"; // Escribir encabezados
                    cout << "Encabezados escritos en el archivo vacío.\n"; // Mensaje de depuración
                    archivoSalida.close();
                }
                else {
                    cout << "No se pudo abrir el archivo para agregar encabezados.\n";
                }
            }
            else {
                cout << "El archivo ya existe y no está vacío, no se escribieron encabezados.\n"; // Mensaje si el archivo ya existe y no está vacío
            }
        }

        archivo.close(); // Cerrar el archivo si se abrió
    }

    int obtenerMaxIdCitas() {
        ifstream archivo("Citas.csv");
        string linea;
        int maxId = 0;

        // Leer el archivo línea por línea
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find("ID") != string::npos) {
                continue; // Ignorar la línea de encabezado y líneas vacías
            }
            int id;
            sscanf(linea.c_str(), "%d", &id); // Extraer el ID de la línea
            if (id > maxId) {
                maxId = id; // Actualizar maxId si se encuentra un ID mayor
            }
        }

        archivo.close();
        return maxId;
    }

    bool guardarCitaEnCSV(const Cita& cita) {
        ofstream archivo("Citas.csv", ios::app);
        if (archivo.is_open()) {
            cout << "Archivo de citas abierto correctamente.\n";
            archivo << cita.urgencia << "," << cita.paciente->nombre << "," << cita.medico->nombre << "," << cita.fecha << "," << cita.urgencia << "\n";
            cout << "Datos de la cita escritos: " << cita.paciente->nombre << " con " << cita.medico->nombre << " en " << cita.fecha << "\n"; // Mensaje de depuración
            archivo.close();
            return true; // Indica que se guardó correctamente
        }
        else {
            cout << "No se pudo abrir el archivo para escribir.\n";
            return false; // Indica que hubo un error al abrir el archivo
        }
    }

    // Método para buscar un paciente por nombre en el CSV
    bool buscarPacienteEnCSV(const string & nombreBuscado) {
     ifstream archivo("Pacientes.csv");
     string linea;
     
         if (!archivo.is_open()) {
             cerr << "Error al abrir el archivo de pacientes." << endl;
             return false;
     
        }
        
                while (getline(archivo, linea)) {
                    istringstream stream(linea);
                    string id, nombre;
            
                    // Leer ID y nombre
                    getline(stream, id, ',');
                getline(stream, nombre, ',');
            
                    if (nombre == nombreBuscado) {
                        archivo.close();
                        return true; // Paciente encontrado
            
            }
            
        }
        
               archivo.close();
            return false; // Paciente no encontrado
        
    }
    
    // Método para buscar un médico por nombre en el CSV
    bool buscarMedicoEnCSV(const string & nombreBuscado) {
        ifstream archivo("Medicos.csv");
        string linea;
    
      if (!archivo.is_open()) {
          cerr << "Error al abrir el archivo de médicos." << endl;
          return false;
      
        }
        
            while (getline(archivo, linea)) {
                istringstream stream(linea);
                string id, nombre;
            
                    // Leer ID y nombre
                    getline(stream, id, ',');
                getline(stream, nombre, ',');
            
                    if (nombre == nombreBuscado) {
                        archivo.close();
                        return true; // Médico encontrado
            
            }
            
        }
        
               archivo.close();
            return false; // Médico no encontrado
        
    }

   
    //FIN METODOS CITAS//
    
    public:
        //INICIO FUNCIONES PACIENTES//
        void agregarPaciente(const string& nombre, const string& fechaIngreso) {
            // Verifica si el archivo CSV existe y crea si no existe
            crearPacientesCSV();

            int nuevoId = obtenerMaxId() + 1; // Obtener el máximo ID y sumarle 1
            auto nuevoPaciente = make_unique<Paciente>(nuevoId, nombre, fechaIngreso);
            if (guardarPacienteEnCSV(*nuevoPaciente)) { // Guardar en CSV
                pacientes.emplace_back(move(nuevoPaciente)); // Agregar paciente a la lista
                cout << "Paciente agregado correctamente con ID: " << nuevoId << "\n";
            }
            else {
                cout << "Error al agregar el paciente. Intente nuevamente.\n";
            }
        }

        void buscarPacientePorNombre() {
            string nombre;
            cout << "Ingrese el nombre del paciente a buscar: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer de entrada
            getline(cin, nombre); // Leer nombre completo
            buscarPaciente(nombre); // Llamar al método de búsqueda
        }

        void eliminarPaciente(const std::string& nombreBuscado) {
            std::ifstream archivo("Pacientes.csv");
            std::vector<std::string> lineas;
            std::string linea;
            bool encontrado = false;

            if (!archivo.is_open()) {
                std::cerr << "Error al abrir el archivo." << std::endl;
                return;
            }

            // Leer todas las líneas del archivo y almacenar en un vector
            while (std::getline(archivo, linea)) {
                std::istringstream stream(linea);
                std::string id, nombre;

                std::getline(stream, id, ','); // Leer ID
                std::getline(stream, nombre, ','); // Leer nombre

                // Si el nombre no coincide con el buscado, lo guardamos
                if (nombre != nombreBuscado) {
                    lineas.push_back(linea);
                }
                else {
                    encontrado = true; // Marcar que encontramos el paciente
                }
            }

            archivo.close(); // Cerrar el archivo después de leer

            // Si encontramos el paciente, escribimos de nuevo el archivo sin él
            if (encontrado) {
                std::ofstream archivoSalida("Pacientes.csv");
                if (archivoSalida.is_open()) {
                    // Escribir el encabezado
                    //archivoSalida << "ID,Nombre,Fecha de Ingreso222\n"; Comentado por crear mas de un encabezado
                    // Escribir todas las líneas restantes
                    for (const auto& l : lineas) {
                        archivoSalida << l << "\n";
                    }
                    archivoSalida.close();
                    std::cout << "Paciente '" << nombreBuscado << "' eliminado correctamente." << std::endl;
                }
                else {
                    std::cerr << "Error al abrir el archivo para escribir." << std::endl;
                }
            }
            else {
                std::cout << "Paciente '" << nombreBuscado << "' no encontrado." << std::endl;
            }
        }

        void modificarNombrePaciente(const std::string& nombreBuscado, const std::string& nuevoNombre) {
            std::ifstream archivo("Pacientes.csv");
            std::vector<std::string> lineas;
            std::string linea;
            bool encontrado = false;

            if (!archivo.is_open()) {
                std::cerr << "Error al abrir el archivo." << std::endl;
                return;
            }

            // Leer todas las líneas del archivo y almacenar en un vector
            while (std::getline(archivo, linea)) {
                std::istringstream stream(linea);
                std::string id, nombre, fechaIngreso;

                std::getline(stream, id, ','); // Leer ID
                std::getline(stream, nombre, ','); // Leer nombre
                std::getline(stream, fechaIngreso); // Leer fecha de ingreso

                // Si el nombre coincide con el buscado, lo modificamos
                if (nombre == nombreBuscado) {
                    nombre = nuevoNombre; // Cambiar el nombre al nuevo
                    encontrado = true; // Marcar que encontramos el paciente
                }

                // Volver a formar la línea con el nombre (modificado o no)
                lineas.push_back(id + "," + nombre + "," + fechaIngreso);
            }

            archivo.close(); // Cerrar el archivo después de leer

            // Si encontramos el paciente, escribimos de nuevo el archivo con el nombre modificado
            if (encontrado) {
                std::ofstream archivoSalida("Pacientes.csv");
                if (archivoSalida.is_open()) {
                    // Escribir el encabezado
                    //archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; Comentado por posible fallo
                    // Escribir todas las líneas restantes
                    for (const auto& l : lineas) {
                        archivoSalida << l << "\n";
                    }
                    archivoSalida.close();
                    std::cout << "Nombre del paciente '" << nombreBuscado << "' modificado a '" << nuevoNombre << "' correctamente." << std::endl;
                }
                else {
                    std::cerr << "Error al abrir el archivo para escribir." << std::endl;
                }
            }
            else {
                std::cout << "Paciente '" << nombreBuscado << "' no encontrado." << std::endl;
            }
        }
        //FIN FUNCIONES PACIENTES//

        //INICIO FUNCIONES MEDICOS//
        void agregarMedico(const string& nombre, const string& especialidad) {
            // Verifica si el archivo CSV existe y crea si no existe
            crearMedicosCSV();

            int nuevoId = obtenerMaxIdMedicos() + 1; // Obtener el máximo ID y sumarle 1
            auto nuevoMedico = make_unique<Medico>(nuevoId, nombre, especialidad);
            if (guardarMedicoEnCSV(*nuevoMedico)) { // Guardar en CSV
                medicos.emplace_back(move(nuevoMedico)); // Agregar paciente a la lista
                cout << "Medico agregado correctamente con ID: " << nuevoId << "\n";
            }
            else {
                cout << "Error al agregar el medico. Intente nuevamente.\n";
            }
        }

        void buscarMedicoPorNombre() {
            string nombre;
            cout << "Ingrese el nombre del medico a buscar: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer de entrada
            getline(cin, nombre); // Leer nombre completo
            buscarMedico(nombre); // Llamar al método de búsqueda
        }

        void eliminarMedico(const std::string& nombreBuscado) {
            std::ifstream archivo("Medicos.csv");
            std::vector<std::string> lineas;
            std::string linea;
            bool encontrado = false;

            if (!archivo.is_open()) {
                std::cerr << "Error al abrir el archivo." << std::endl;
                return;
            }

            // Leer todas las líneas del archivo y almacenar en un vector
            while (std::getline(archivo, linea)) {
                std::istringstream stream(linea);
                std::string id, nombre;

                std::getline(stream, id, ','); // Leer ID
                std::getline(stream, nombre, ','); // Leer nombre

                // Si el nombre no coincide con el buscado, lo guardamos
                if (nombre != nombreBuscado) {
                    lineas.push_back(linea);
                }
                else {
                    encontrado = true; // Marcar que encontramos el paciente
                }
            }

            archivo.close(); // Cerrar el archivo después de leer

            // Si encontramos el paciente, escribimos de nuevo el archivo sin él
            if (encontrado) {
                std::ofstream archivoSalida("Medicos.csv");
                if (archivoSalida.is_open()) {
                    // Escribir el encabezado
                    //archivoSalida << "ID,Nombre,Fecha de Ingreso222\n"; Comentado por crear mas de un encabezado
                    // Escribir todas las líneas restantes
                    for (const auto& l : lineas) {
                        archivoSalida << l << "\n";
                    }
                    archivoSalida.close();
                    std::cout << "Medico '" << nombreBuscado << "' eliminado correctamente." << std::endl;
                }
                else {
                    std::cerr << "Error al abrir el archivo para escribir." << std::endl;
                }
            }
            else {
                std::cout << "Medico '" << nombreBuscado << "' no encontrado." << std::endl;
            }
        }

        void modificarNombreMedico(const std::string& nombreBuscado, const std::string& nuevoNombre) {
            std::ifstream archivo("Medicos.csv");
            std::vector<std::string> lineas;
            std::string linea;
            bool encontrado = false;

            if (!archivo.is_open()) {
                std::cerr << "Error al abrir el archivo." << std::endl;
                return;
            }

            // Leer todas las líneas del archivo y almacenar en un vector
            while (std::getline(archivo, linea)) {
                std::istringstream stream(linea);
                std::string id, nombre, especialidad;

                std::getline(stream, id, ','); // Leer ID
                std::getline(stream, nombre, ','); // Leer nombre
                std::getline(stream, especialidad); // Leer fecha de ingreso

                // Si el nombre coincide con el buscado, lo modificamos
                if (nombre == nombreBuscado) {
                    nombre = nuevoNombre; // Cambiar el nombre al nuevo
                    encontrado = true; // Marcar que encontramos el paciente
                }

                // Volver a formar la línea con el nombre (modificado o no)
                lineas.push_back(id + "," + nombre + "," + especialidad);
            }

            archivo.close(); // Cerrar el archivo después de leer

            // Si encontramos el paciente, escribimos de nuevo el archivo con el nombre modificado
            if (encontrado) {
                std::ofstream archivoSalida("Medicos.csv");
                if (archivoSalida.is_open()) {
                    // Escribir el encabezado
                    //archivoSalida << "ID,Nombre,Fecha de Ingreso\n"; Comentado por posible fallo
                    // Escribir todas las líneas restantes
                    for (const auto& l : lineas) {
                        archivoSalida << l << "\n";
                    }
                    archivoSalida.close();
                    std::cout << "Nombre del medico '" << nombreBuscado << "' modificado a '" << nuevoNombre << "' correctamente." << std::endl;
                }
                else {
                    std::cerr << "Error al abrir el archivo para escribir." << std::endl;
                }
            }
            else {
                std::cout << "Medico '" << nombreBuscado << "' no encontrado." << std::endl;
            }
        }
        //FIN FUNCIONES MEDICOS//

        //INICIO FUNCIONES CITAS//
        // Modificar la función agregarCita
        void agregarCita(const string & nombrePaciente, const string & nombreMedico, const string & fecha, int urgencia) {
                // Verifica si el archivo CSV existe y crea si no existe
                crearCitasCSV();
            
                // Verificar si el paciente y el médico existen en sus respectivos archivos
                if (!buscarPacienteEnCSV(nombrePaciente)) {
                    cout << "El paciente '" << nombrePaciente << "' no existe en el sistema. No se puede agregar la cita." << endl;
                    return;
               
                }
            
                if (!buscarMedicoEnCSV(nombreMedico)) {
                      cout << "El médico '" << nombreMedico << "' no existe en el sistema. No se puede agregar la cita." << endl;
                      return;
                
                }
            
                int nuevoId = obtenerMaxIdCitas() + 1; // Obtener el máximo ID y sumarle 1
                auto paciente = make_unique<Paciente>(nuevoId, nombrePaciente, ""); // Crear paciente temporal
                auto medico = make_unique<Medico>(nuevoId, nombreMedico, ""); // Crear medico temporal
                auto nuevaCita = make_unique<Cita>(move(paciente), move(medico), fecha, urgencia);
           
                if (guardarCitaEnCSV(*nuevaCita)) { // Guardar en CSV
                    citas.emplace_back(move(nuevaCita)); // Agregar cita a la lista
                    cout << "Cita agregada correctamente con ID: " << nuevoId << "\n";
           
                }
                else {
                            cout << "Error al agregar la cita. Intente nuevamente.\n";
                
                }
            
        }

        //FIN FUNCIONES CITAS//

};

int main()
{
    Hospital hospital;
    int opcion;

    while (true) {
        cout << "------Menu------\n";
        cout << "1. Agregar paciente\n";
        cout << "2. Buscar paciente por nombre\n";
        cout << "3. Eliminar paciente por nombre\n";
        cout << "4. Modificar nombre de paciente\n";
        cout << "5. Agregar medico\n";
        cout << "6. Buscar medico por nombre\n";
        cout << "7. Eliminar medico por nombre\n";
        cout << "8. Modificar nombre de medico\n";
        cout << "9. Agregar cita\n";
        cout << "10. Salir\n";
        cout << "\nIntroduce un numero: ";
        cin >> opcion;

        switch (opcion) {

        case 1: {
            string nombre, fechaIngreso;

            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese nombre del paciente: ";
            getline(cin, nombre); // Leer nombre completo
            cout << "Ingrese fecha de ingreso del paciente: ";
            getline(cin, fechaIngreso); // Leer fecha de ingreso

            hospital.agregarPaciente(nombre, fechaIngreso); // Llamar al método sin id
            break;
        }
        case 2:
            hospital.buscarPacientePorNombre(); // Llamar al método de búsqueda
            break;
        case 3: {
            string nombreBuscado;
            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese el nombre del paciente a eliminar: ";
            getline(cin, nombreBuscado); // Leer nombre del paciente a eliminar
            hospital.eliminarPaciente(nombreBuscado); // Llamar al método de eliminación
            break;
        }
        case 4: {
            string nombreBuscado, nuevoNombre;
            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese el nombre del paciente a modificar: ";
            getline(cin, nombreBuscado); // Leer nombre del paciente a modificar
            cout << "Ingrese el nuevo nombre del paciente: ";
            getline(cin, nuevoNombre); // Leer nuevo nombre
            hospital.modificarNombrePaciente(nombreBuscado, nuevoNombre); // Llamar al método de modificación
            break;
        }
        case 5: {
            string nombre, especialidad;

            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese nombre del medico: ";
            getline(cin, nombre); // Leer nombre completo
            cout << "Ingrese la especialidad del medico: ";
            getline(cin, especialidad); // Leer fecha de ingreso

            hospital.agregarMedico(nombre, especialidad); // Llamar al método sin id
            break;
        }
        case 6:
            hospital.buscarMedicoPorNombre(); // Llamar al método de búsqueda
            break;
        case 7: {
            string nombreBuscado;
            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese el nombre del medico a eliminar: ";
            getline(cin, nombreBuscado); // Leer nombre del paciente a eliminar
            hospital.eliminarMedico(nombreBuscado); // Llamar al método de eliminación
            break;
        }
        case 8: {
            string nombreBuscado, nuevoNombre;
            cin.ignore(); // Limpiar el buffer de entrada
            cout << "Ingrese el nombre del medico a modificar: ";
            getline(cin, nombreBuscado); // Leer nombre del paciente a modificar
            cout << "Ingrese el nuevo nombre del medico: ";
            getline(cin, nuevoNombre); // Leer nuevo nombre
            hospital.modificarNombreMedico(nombreBuscado, nuevoNombre); // Llamar al método de modificación
            break;
        }
        case 9: {
               string nombrePaciente, nombreMedico, fecha;
               int urgencia;
            
                  cin.ignore(); // Limpiar el buffer de entrada
               cout << "Ingrese nombre del paciente: ";
               getline(cin, nombrePaciente); // Leer nombre del paciente
               cout << "Ingrese nombre del medico: ";
               getline(cin, nombreMedico); // Leer nombre del medico
                cout << "Ingrese fecha de la cita: ";
                getline(cin, fecha); // Leer fecha de la cita
                cout << "Ingrese urgencia (1: baja, 2: media, 3: alta): ";
                cin >> urgencia; // Leer urgencia
            
                hospital.agregarCita(nombrePaciente, nombreMedico, fecha, urgencia); // Llamar al método para agregar cita
                break;
            
        }

        case 10:
            return 0;
        default:
            cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }

    return 0;
}


