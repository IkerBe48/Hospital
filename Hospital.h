#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Paciente {
public:
    Paciente(const std::string& nombre, const std::string& fechaIngreso);

    std::string getNombre() const { return nombre; }
    std::string getFechaIngreso() const { return fechaIngreso; }

    static void agregarPaciente(const std::string& nombre, const std::string& fechaIngreso);
    static void buscarPacientePorNombre(const std::string& nombre);
    static void eliminarPaciente(const std::string& nombre);
    static void modificarNombrePaciente(const std::string& nombreBuscado, const std::string& nuevoNombre);
    static void interfazPacientes(); //Menu de Pacientes

private:
    std::string nombre;
    std::string fechaIngreso;
    static std::vector<std::unique_ptr<Paciente>> pacientes;
};

class Medico {
public:
    Medico(const std::string& nombre, const std::string& especialidad);

    std::string getNombre() const { return nombre; }
    std::string getEspecialidad() const { return especialidad; }

    static void agregarMedico(const std::string& nombre, const std::string& especialidad);
    static void buscarMedicoPorNombre(const std::string& nombre);
    static void eliminarMedico(const std::string& nombre);
    static void modificarNombreMedico(const std::string& nombreBuscado, const std::string& nuevoNombre);
    static void interfazMedicos(); //Menu de Medicos

private:
    std::string nombre;
    std::string especialidad;
    static std::vector<std::unique_ptr<Medico>> medicos;
};

class Cita {
public:
    Cita(const std::string& nombrePaciente, const std::string& nombreMedico, const std::string& fecha, int urgencia);

    static void agregarCita(const std::string& nombrePaciente, const std::string& nombreMedico, const std::string& fecha, int urgencia);
    static void interfazCitas(); //Menu de Citas

private:
    std::string nombrePaciente;
    std::string nombreMedico;
    std::string fecha;
    int urgencia;
    static std::vector<std::unique_ptr<Cita>> citas;
};

class Hospital {
public:
    void menu();

private:
    std::vector<std::unique_ptr<Paciente>> pacientes;
    std::vector<std::unique_ptr<Medico>> medicos;
    std::vector<std::unique_ptr<Cita>> citas;
};