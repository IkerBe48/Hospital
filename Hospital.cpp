#include "Hospital.h"

// Inicialización de vectores estáticos
//std::vector<std::unique_ptr<Paciente>> Paciente::pacientes;
//std::vector<std::unique_ptr<Medico>> Medico::medicos;
//std::vector<std::unique_ptr<Cita>> Cita::citas;

// Implementación de métodos de la clase Paciente
Paciente::Paciente(const std::string& nombre, const std::string& fechaIngreso)
    : nombre(nombre), fechaIngreso(fechaIngreso) {
}

// Implementación de métodos de la clase Medico
Medico::Medico(const std::string& nombre, const std::string& especialidad)
    : nombre(nombre), especialidad(especialidad) {
}

// Implementación de métodos de la clase Cita
Cita::Cita(const std::string& nombrePaciente, const std::string& nombreMedico, const std::string& fecha, int urgencia)
    : nombrePaciente(nombrePaciente), nombreMedico(nombreMedico), fecha(fecha), urgencia(urgencia) {
}

// Implementación del menú del Hospital
void Hospital::menu() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Apartado Pacientes\n";
        std::cout << "2. Apartado Medicos\n";
        std::cout << "3. Apartado Citas\n";
        std::cout << "4. Apartado Servicios (PENDIENTE)\n";
        std::cout << "5. Generar backup (PENDIENTE)\n";
        
        std::cout << "10. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            Paciente::interfazPacientes();
            break;
        }
        case 2: {
            /*std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a buscar: ";
            std::getline(std::cin, nombreBuscado);
            Paciente::buscarPacientePorNombre(nombreBuscado);*/
            Medico::interfazMedicos();
            break;
        }
        case 3: {
            /*std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del paciente a eliminar: ";
            std::getline(std::cin, nombreBuscado);
            Paciente::eliminarPaciente(nombreBuscado);*/
            Cita::interfazCitas();
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
        case 5: {
            std::string nombre, especialidad;
            std::cin.ignore();
            std::cout << "Ingrese nombre del medico: ";
            std::getline(std::cin, nombre);
            std::cout << "Ingrese la especialidad del medico: ";
            std::getline(std::cin, especialidad);
            Medico::agregarMedico(nombre, especialidad);
            break;
        }
        case 6: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del medico a buscar: ";
            std::getline(std::cin, nombreBuscado);
            Medico::buscarMedicoPorNombre(nombreBuscado);
            break;
        }
        case 7: {
            std::string nombreBuscado;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del medico a eliminar: ";
            std::getline(std::cin, nombreBuscado);
            Medico::eliminarMedico(nombreBuscado);
            break;
        }
        case 8: {
            std::string nombreBuscado, nuevoNombre;
            std::cin.ignore();
            std::cout << "Ingrese el nombre del medico a modificar: ";
            std::getline(std::cin, nombreBuscado);
            std::cout << "Ingrese el nuevo nombre del medico: ";
            std::getline(std::cin, nuevoNombre);
            Medico::modificarNombreMedico(nombreBuscado, nuevoNombre);
            break;
        }
        case 9: {
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
        case 10:
            return;
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}

int main() {
    Hospital hospital; // Crear una instancia de la clase Hospital
    hospital.menu();
}