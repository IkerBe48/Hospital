// Paciente.h
#ifndef PACIENTE_H
#define PACIENTE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Paciente {
public:
    static std::vector<std::unique_ptr<Paciente>> pacientes;  // Almacena los pacientes

    Paciente(int id, const std::string& nombre, const std::string& fechaIngreso);
    std::string getNombre() const { return nombre; }

   // void agregarHistorial(std::string info);
    //void mostrarInfo() const;

    // Métodos de Pacientes
    static void crearPacientesCSV();
    static bool guardarPacienteEnCSV(const Paciente& paciente);
    static void buscarPaciente();

    static void agregarPaciente(const std::string& nombre, const std::string& fechaIngreso);
    static int obtenerMaxId();
    //static void buscarPacientePorNombre(const std::string& nombreBuscado);
    static void eliminarPaciente(const std::string& nombreBuscado);
    static void modificarNombrePaciente(const std::string& nombreBuscado, const std::string& nuevoNombre);

    static void interfazPacientes(); //Menu de Pacientes
    static void crearBackupPacientesCSV();
    static void exportarPacientes();
    static bool esAnioBisiesto(int anio);
    static bool esFechaValida(const std::string& fecha);
    static void buscarPacientesPorFechaIngreso(const std::string& fechaInicio, const std::string& fechaFin);
    
private:
    int id;
    std::string nombre;
    std::string fechaIngreso;
    std::string historialClinico;
};

#endif // PACIENTE_H