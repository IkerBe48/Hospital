#ifndef SERVICIO_H
#define SERVICIO_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

class Paciente; // Declaración anticipada

class Servicio {
public:

    static std::vector<std::unique_ptr<Servicio>> servicios; // Almacena las citas

    Cita(int id, std::unique_ptr<Paciente> paciente, const std::string& fecha, const std::string& descripcion);

    static void crearServiciosCSV();
    static int obtenerMaxIdServicios();
    static bool guardarServicioEnCSV(const Cita& cita);
    static bool buscarPacienteEnCSV(const std::string& nombreBuscado);
    static void agregarServicio(const std::string& nombrePaciente, const std::string& nombreMedico, const std::string& fecha, int urgencia);
    static void crearBackupServiciosCSV();
    static bool esAnioBisiesto(int anio);
    static bool esFechaValida(const std::string& fecha);
    static void exportarServicios();
    static void modificarFechaCita(const std::string& nombreBuscado, const std::string& nuevaFecha);
    static void buscarHistorialClinico(const std::string& fechaInicio, const std::string& fechaFin);

    static void exportarServicioPorFecha(const std::string& fechaInicio, const std::string& fechaFin);
    static void exportarServiciosPorPaciente(const std::string& nombreMedico);

    static void interfazServicios(); //Menu de Citas

private:
    int id;
    std::unique_ptr<Paciente> paciente;
    std::string fecha;
    std::string descripcion;
};

#endif // SERVICIO_H
