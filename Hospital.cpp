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

// Implementación de métodos de la clase Servicio
Servicio::Servicio(const std::string& nombrePaciente, const std::string& fecha, const std::string& descripcion)
    : nombrePaciente(nombrePaciente), fecha(fecha), descripcion(descripcion) {
}

// Implementación del menú del Hospital
void Hospital::menu() {
    int opcion;
    while (true) {
        std::cout << "------Menu------\n";
        std::cout << "1. Apartado Pacientes\n";
        std::cout << "2. Apartado Medicos\n";
        std::cout << "3. Apartado Citas\n";
        std::cout << "4. Apartado Servicios\n";
        std::cout << "5. Generar backup\n";
        std::cout << "6. Salir\n";
        std::cout << "\nIntroduce un numero: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            Paciente::interfazPacientes();
            break;
        }
        case 2: {
            Medico::interfazMedicos();
            break;
        }
        case 3: {
            Cita::interfazCitas();
            break;
        }

        case 4: {
            Servicio::interfazServicios();
            break;
        }
        case 5: {
            Paciente::crearBackupPacientesCSV();
            Medico::crearBackupMedicosCSV();
            Cita::crearBackupCitasCSV();
            break;
        }
        case 6:
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