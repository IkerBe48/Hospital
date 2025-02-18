﻿#include "Hospital.h"


// Implementacion del menu del Hospital
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

        // Control de error para verificar que la entrada es un número
        while (std::cin.fail() || opcion < 1 || opcion > 6) {
            std::cin.clear(); // Limpiar el estado de error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
            std::cout << "Entrada no valida. Introduce un numero entre 1 y 6: ";
            std::cin >> opcion;
        }

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
            Servicio::crearBackupServiciosCSV();
            break;
        }
        case 6:
            std::cout << "Saliendo del programa...\n";
            exit(0); // Cierra la aplicación
        default:
            std::cout << "\nOpcion invalida. Intente de nuevo.\n";
        }
    }
}

int main() {
    Hospital hospital; // Crear una instancia de la clase Hospital
    hospital.menu();
}