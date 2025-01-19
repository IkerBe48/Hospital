# Hospital
Descripción

El siguiente proyecto consiste en una aplicación basada en C++ diseñada para gestionar pacientes, médicos, citas y servicios en un entorno hospitalario. La aplicación ofrece un menú interactivo para realizar diversas operaciones como agregar, buscar, modificar y eliminar datos de las entidades mencionadas.

Características Principales

Gestor de Pacientes

Agregar pacientes con nombre y fecha de ingreso.

Buscar pacientes por nombre, ID o fecha de ingreso.

Modificar datos de los pacientes.

Eliminar pacientes del sistema.

Generar respaldos y reportes en formato CSV y TXT.

Gestor de Médicos

Registrar médicos con su especialidad y disponibilidad.

Buscar médicos por nombre, especialidad o disponibilidad.

Modificar datos de los médicos.

Eliminar médicos.

Crear respaldos y reportes detallados.

Gestor de Citas

Programar citas con un paciente y un médico.

Validación de fechas y disponibilidad.

Buscar citas por fecha o urgencia.

Modificar o cancelar citas existentes.

Exportar citas en rangos de fecha o por médico.

Gestor de Servicios

Registrar servicios realizados a un paciente.

Consultar historial clínico de los pacientes.

Modificar descripciones de servicios.

Crear respaldos y reportes por paciente.

Requisitos

Sistema Operativo: Windows/Linux.

Compilador: C++ compatible con C++20.

Herramientas Adicionales: CMake y Ninja (opcional).

Instalación

Clona el repositorio o descarga los archivos.

git clone https://github.com/IkerBe48/Hospital

Crea una carpeta para la construcción del proyecto.

cmake -S . -B build

Configura el proyecto con CMake.

cmake --build build --config Release

Ejecuta la aplicación.

./Hospital

Archivos del Proyecto

Código Fuente:

Hospital.cpp, Hospital.h: Clase principal que administra el menú.

Paciente.cpp, Paciente.h: Gestiona las operaciones relacionadas con pacientes.

Medico.cpp, Medico.h: Gestiona los médicos y su disponibilidad.

Cita.cpp, Cita.h: Administra las citas entre pacientes y médicos.

Servicio.cpp, Servicio.h: Registra servicios y genera historiales clínicos.

Configuración:

CMakeLists.txt: Configuración de CMake para construir el proyecto.

CMakePresets.json: Preajustes de CMake para entornos de desarrollo comunes.

Otros Archivos:

Pacientes.csv, Medicos.csv, Citas.csv, Servicios.csv: Archivos generados para almacenar los datos.

Backups/: Carpeta para guardar copias de las bases.

Uso

Al iniciar, el programa muestra un menú principal con las siguientes opciones:

Gestor de Pacientes

Gestor de Médicos

Gestor de Citas

Gestor de Servicios

Generar Backup

Salir

Selecciona una opción para navegar y realizar las operaciones deseadas.

Contribuciones

Este proyecto ha sido creado y desarrollado por Iker Bedia.