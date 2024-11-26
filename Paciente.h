#ifndef PACIENTE_H
#define PACIENTE_H

#include <iostream>
#include <string>

class Paciente {
public:
    int id;
    std::string nombre;
    std::string fechaIngreso;
    std::string historialClinico;
    std::string getNombre() const { return nombre; }

    Paciente(int id, std::string nombre, std::string fechaIngreso);
    void agregarHistorial(std::string info);
    void mostrarInfo() const;

    // Métodos de Pacientes
    static void crearPacientesCSV();
    static int obtenerMaxId();
    static bool guardarPacienteEnCSV(const Paciente& paciente);
    static void buscarPaciente(const std::string& nombreBuscado);

    static void agregarPaciente(const std::string& nombre, const std::string& fechaIngreso);
    static void buscarPacientePorNombre();
    static void eliminarPaciente(const std::string& nombreBuscado);
    static void modificarNombrePaciente(const std::string& nombreBuscado, const std::string& nuevoNombre);

};

#endif // PACIENTE_H