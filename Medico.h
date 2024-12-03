#ifndef MEDICO_H
#define MEDICO_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Medico {
public:
    static std::vector<std::unique_ptr<Medico>> medicos;  // Almacena los m�dicos

    Medico(int id, const std::string& nombre, const std::string& especialidad);
    std::string getNombre() const { return nombre; }

    // M�todos para manejar m�dicos
    static void crearMedicosCSV();
    static int obtenerMaxIdMedicos();
    static bool guardarMedicoEnCSV(const Medico& medico);
    static void buscarMedico(const std::string& nombreBuscado);
    static void agregarMedico(const std::string& nombre, const std::string& especialidad);
    static void buscarMedicoPorNombre(const std::string& nombreBuscado);
    static void eliminarMedico(const std::string& nombreBuscado);
    static void modificarNombreMedico(const std::string& nombreBuscado, const std::string& nuevoNombre);

    static void interfazMedicos(); //Menu de Medicos

private:
    int id;
    std::string nombre;
    std::string especialidad;    
};

#endif // MEDICO_H