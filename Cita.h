#ifndef CITA_H
#define CITA_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

class Paciente; // Declaración anticipada
class Medico;   // Declaración anticipada

class Cita {
public:

    static std::vector<std::unique_ptr<Cita>> citas; // Almacena las citas

    Cita(int id, std::unique_ptr<Paciente> paciente, std::unique_ptr<Medico> medico, const std::string& fecha, int urgencia);

    static void crearCitasCSV();
    static int obtenerMaxIdCitas();
    static bool guardarCitaEnCSV(const Cita& cita);
    static bool buscarPacienteEnCSV(const std::string& nombreBuscado);
    static bool buscarMedicoEnCSV(const std::string& nombreBuscado);
    static void agregarCita(const std::string& nombrePaciente, const std::string& nombreMedico, const std::string& fecha, int urgencia);
    static void crearBackupCitasCSV();
    static bool esAnioBisiesto(int anio);
    static bool esFechaValida(const std::string& fecha);
    static void exportarCitas();
    static void eliminarCita(const std::string& nombreBuscado);
    static void modificarFechaCita(const std::string& nombreBuscado, const std::string& nuevaFecha);
    static void buscarCitasPorFecha(const std::string& fechaInicio, const std::string& fechaFin);

    static void exportarCitasPorFecha(const std::string& fechaInicio, const std::string& fechaFin);
    static void exportarCitasPorMedico(const std::string& fechaInicio, const std::string& fechaFin);
    static void exportarCitasCronicas(const std::string& fechaInicio, const std::string& fechaFin);

    static void interfazCitas(); //Menu de Citas

private:
    int id;
    std::unique_ptr<Paciente> paciente;
    std::unique_ptr<Medico> medico;
    std::string fecha;
    int urgencia;
};

#endif // CITA_H