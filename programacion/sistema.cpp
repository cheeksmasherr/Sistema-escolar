#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include <sstream>

using namespace std;


const double CALIFICACION_MIN = 0.0;
const double CALIFICACION_MAX = 100.0;
const int CAPACIDAD_INICIAL = 10;


struct Alumno {
    string nombre;
    string matricula;
    double calificacion1;
    double calificacion2;
    double calificacion3;
    double calificacionFinal;
    
    
    Alumno() : calificacion1(0), calificacion2(0), calificacion3(0), calificacionFinal(0) {}
    
    
    Alumno(string nom, string mat, double c1, double c2, double c3) 
        : nombre(nom), matricula(mat), calificacion1(c1), calificacion2(c2), calificacion3(c3) {
        calcularCalificacionFinal();
    }
    
    
    void calcularCalificacionFinal() {
        calificacionFinal = (calificacion1 + calificacion2 + calificacion3) / 3.0;
        
        calificacionFinal = round(calificacionFinal * 100.0) / 100.0;
    }
    
    
    bool estaAprobado() const {
        return calificacionFinal >= 70.0;
    }
    
    
    bool validarCalificaciones() const {
        return (calificacion1 >= CALIFICACION_MIN && calificacion1 <= CALIFICACION_MAX &&
                calificacion2 >= CALIFICACION_MIN && calificacion2 <= CALIFICACION_MAX &&
                calificacion3 >= CALIFICACION_MIN && calificacion3 <= CALIFICACION_MAX);
    }
    
    
    bool validarDatos() const {
        return !nombre.empty() && !matricula.empty() && validarCalificaciones();
    }
};


class SistemaAdministrativo {
private:
    Alumno* alumnos;
    int cantidadAlumnos;
    int capacidad;
    
    
    void redimensionar(int nuevaCapacidad) {
        if (nuevaCapacidad <= capacidad) return;
        
        Alumno* nuevo = new (nothrow) Alumno[nuevaCapacidad];
        if (!nuevo) {
            cerr << "Error: No se pudo asignar memoria." << endl;
            return;
        }
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            nuevo[i] = alumnos[i];
        }
        
        delete[] alumnos;
        alumnos = nuevo;
        capacidad = nuevaCapacidad;
    }
    
    
    int buscarIndice(const string& matricula) const {
        for (int i = 0; i < cantidadAlumnos; i++) {
            if (alumnos[i].matricula == matricula) {
                return i;
            }
        }
        return -1;
    }
    
    
    bool leerCalificacion(double& calificacion, const string& mensaje) {
        cout << mensaje;
        string entrada;
        cin >> entrada;
        
        try {
            calificacion = stod(entrada);
            if (calificacion < CALIFICACION_MIN || calificacion > CALIFICACION_MAX) {
                cout << "Error: La calificación debe estar entre " 
                     << CALIFICACION_MIN << " y " << CALIFICACION_MAX << "." << endl;
                return false;
            }
            return true;
        } catch (const invalid_argument&) {
            cout << "Error: Entrada inválida. Debe ser un número." << endl;
            return false;
        } catch (const out_of_range&) {
            cout << "Error: Número fuera de rango." << endl;
            return false;
        }
    }
    
    
    void limpiarBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    
    bool validarCadenaNoVacia(const string& cadena, const string& campo) {
        if (cadena.empty() || cadena.find_first_not_of(" \t\n") == string::npos) {
            cout << "Error: El campo '" << campo << "' no puede estar vacío." << endl;
            return false;
        }
        return true;
    }
    
public:
    SistemaAdministrativo() : alumnos(nullptr), cantidadAlumnos(0), capacidad(CAPACIDAD_INICIAL) {
        alumnos = new (nothrow) Alumno[capacidad];
        if (!alumnos) {
            cerr << "Error crítico: No se pudo inicializar el sistema." << endl;
            capacidad = 0;
        }
    }
    
    
    SistemaAdministrativo(const SistemaAdministrativo& otro) 
        : cantidadAlumnos(otro.cantidadAlumnos), capacidad(otro.capacidad) {
        alumnos = new (nothrow) Alumno[capacidad];
        if (alumnos) {
            for (int i = 0; i < cantidadAlumnos; i++) {
                alumnos[i] = otro.alumnos[i];
            }
        }
    }
    
    
    SistemaAdministrativo& operator=(const SistemaAdministrativo& otro) {
        if (this != &otro) {
            delete[] alumnos;
            cantidadAlumnos = otro.cantidadAlumnos;
            capacidad = otro.capacidad;
            alumnos = new (nothrow) Alumno[capacidad];
            if (alumnos) {
                for (int i = 0; i < cantidadAlumnos; i++) {
                    alumnos[i] = otro.alumnos[i];
                }
            }
        }
        return *this;
    }
    
    ~SistemaAdministrativo() {
        delete[] alumnos;
    }
    
    
    bool cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            return false;
        }
        
        cantidadAlumnos = 0;
        string linea;
        int numeroLinea = 0;
        int alumnosCargados = 0;
        int errores = 0;
        
        while (getline(archivo, linea)) {
            numeroLinea++;
            
            
            linea.erase(0, linea.find_first_not_of(" \t\n\r"));
            linea.erase(linea.find_last_not_of(" \t\n\r") + 1);
            
            if (linea.empty()) continue;
            
            
            size_t pos1 = linea.find('|');
            if (pos1 == string::npos) {
                cerr << "Advertencia: Línea " << numeroLinea 
                     << " ignorada (formato inválido)." << endl;
                errores++;
                continue;
            }
            
            string nombre = linea.substr(0, pos1);
            size_t pos2 = linea.find('|', pos1 + 1);
            if (pos2 == string::npos) {
                cerr << "Advertencia: Línea " << numeroLinea 
                     << " ignorada (formato inválido)." << endl;
                errores++;
                continue;
            }
            
            string matricula = linea.substr(pos1 + 1, pos2 - pos1 - 1);
            size_t pos3 = linea.find('|', pos2 + 1);
            if (pos3 == string::npos) {
                cerr << "Advertencia: Línea " << numeroLinea 
                     << " ignorada (formato inválido)." << endl;
                errores++;
                continue;
            }
            
            double cal1, cal2, cal3;
            try {
                cal1 = stod(linea.substr(pos2 + 1, pos3 - pos2 - 1));
                size_t pos4 = linea.find('|', pos3 + 1);
                if (pos4 == string::npos) {
                    cerr << "Advertencia: Línea " << numeroLinea 
                         << " ignorada (formato inválido)." << endl;
                    errores++;
                    continue;
                }
                
                cal2 = stod(linea.substr(pos3 + 1, pos4 - pos3 - 1));
                cal3 = stod(linea.substr(pos4 + 1));
                
                
                if (cal1 < CALIFICACION_MIN || cal1 > CALIFICACION_MAX ||
                    cal2 < CALIFICACION_MIN || cal2 > CALIFICACION_MAX ||
                    cal3 < CALIFICACION_MIN || cal3 > CALIFICACION_MAX) {
                    cerr << "Advertencia: Línea " << numeroLinea 
                         << " ignorada (calificaciones fuera de rango)." << endl;
                    errores++;
                    continue;
                }
                
            } catch (const invalid_argument&) {
                cerr << "Advertencia: Línea " << numeroLinea 
                     << " ignorada (calificaciones inválidas)." << endl;
                errores++;
                continue;
            } catch (const out_of_range&) {
                cerr << "Advertencia: Línea " << numeroLinea 
                     << " ignorada (números fuera de rango)." << endl;
                errores++;
                continue;
            }
            
            
            if (nombre.empty() || matricula.empty()) {
                cerr << "Advertencia: Línea " << numeroLinea 
                     << " ignorada (nombre o matrícula vacíos)." << endl;
                errores++;
                continue;
            }
            
            
            if (cantidadAlumnos > 0) {
                bool duplicado = false;
                for (int i = 0; i < cantidadAlumnos; i++) {
                    if (alumnos[i].matricula == matricula) {
                        cerr << "Advertencia: Línea " << numeroLinea 
                             << " ignorada (matrícula duplicada: " << matricula << ")." << endl;
                        errores++;
                        duplicado = true;
                        break;
                    }
                }
                if (duplicado) continue;
            }
            
            
            if (cantidadAlumnos >= capacidad) {
                redimensionar(capacidad * 2);
            }
            
            alumnos[cantidadAlumnos] = Alumno(nombre, matricula, cal1, cal2, cal3);
            cantidadAlumnos++;
            alumnosCargados++;
        }
        
        archivo.close();
        
        if (errores > 0) {
            cout << "Se encontraron " << errores << " error(es) al cargar el archivo." << endl;
        }
        
        return alumnosCargados > 0 || cantidadAlumnos > 0;
    }
    
    
    bool guardarEnArchivo(const string& nombreArchivo) {
        if (cantidadAlumnos == 0) {
            
            ofstream archivo(nombreArchivo);
            archivo.close();
            return true;
        }
        
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo para escritura." << endl;
            return false;
        }
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            if (alumnos[i].validarDatos()) {
                archivo << alumnos[i].nombre << "|"
                        << alumnos[i].matricula << "|"
                        << fixed << setprecision(2)
                        << alumnos[i].calificacion1 << "|"
                        << alumnos[i].calificacion2 << "|"
                        << alumnos[i].calificacion3 << endl;
            }
        }
        
        archivo.close();
        return archivo.good() || archivo.eof();
    }
    
    
    bool agregarAlumno() {
        if (!alumnos) {
            cout << "Error: Sistema no inicializado correctamente." << endl;
            return false;
        }
        
        string nombre, matricula;
        double cal1, cal2, cal3;
        
        cout << "\n=== AGREGAR ALUMNO ===" << endl;
        
        
        cout << "Nombre completo: ";
        limpiarBuffer();
        getline(cin, nombre);
        if (!validarCadenaNoVacia(nombre, "Nombre")) {
            return false;
        }
        
        
        cout << "Matrícula: ";
        getline(cin, matricula);
        if (!validarCadenaNoVacia(matricula, "Matrícula")) {
            return false;
        }
        
        
        if (buscarIndice(matricula) != -1) {
            cout << "Error: Ya existe un alumno con esa matrícula." << endl;
            return false;
        }
        
        
        if (!leerCalificacion(cal1, "Calificación parcial 1: ")) {
            return false;
        }
        if (!leerCalificacion(cal2, "Calificación parcial 2: ")) {
            return false;
        }
        if (!leerCalificacion(cal3, "Calificación parcial 3: ")) {
            return false;
        }
        
        
        if (cantidadAlumnos >= capacidad) {
            redimensionar(capacidad * 2);
            if (!alumnos) {
                cout << "Error: No se pudo asignar memoria." << endl;
                return false;
            }
        }
        
        alumnos[cantidadAlumnos] = Alumno(nombre, matricula, cal1, cal2, cal3);
        cantidadAlumnos++;
        
        cout << "\nAlumno agregado exitosamente." << endl;
        return true;
    }
    
    
    void leerAlumno() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        string matricula;
        cout << "\n=== LEER ALUMNO ===" << endl;
        cout << "Ingrese la matrícula: ";
        cin >> matricula;
        
        int indice = buscarIndice(matricula);
        if (indice == -1) {
            cout << "Alumno no encontrado." << endl;
            return;
        }
        
        mostrarAlumno(indice);
    }
    
    
    void modificarAlumno() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        string matricula;
        cout << "\n=== MODIFICAR ALUMNO ===" << endl;
        cout << "Ingrese la matrícula del alumno a modificar: ";
        cin >> matricula;
        
        int indice = buscarIndice(matricula);
        if (indice == -1) {
            cout << "Alumno no encontrado." << endl;
            return;
        }
        
        cout << "\nDatos actuales:" << endl;
        mostrarAlumno(indice);
        
        int opcion;
        cout << "\n¿Qué desea modificar?" << endl;
        cout << "1. Nombre" << endl;
        cout << "2. Matrícula" << endl;
        cout << "3. Calificaciones" << endl;
        cout << "4. Todo" << endl;
        cout << "5. Cancelar" << endl;
        cout << "Opción: ";
        
        if (!(cin >> opcion)) {
            limpiarBuffer();
            cout << "Error: Entrada inválida." << endl;
            return;
        }
        
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                cout << "Nuevo nombre: ";
                string nuevoNombre;
                getline(cin, nuevoNombre);
                if (validarCadenaNoVacia(nuevoNombre, "Nombre")) {
                    alumnos[indice].nombre = nuevoNombre;
                    cout << "\nAlumno modificado exitosamente." << endl;
                }
                break;
            }
            case 2: {
                string nuevaMatricula;
                cout << "Nueva matrícula: ";
                getline(cin, nuevaMatricula);
                if (!validarCadenaNoVacia(nuevaMatricula, "Matrícula")) {
                    return;
                }
                if (buscarIndice(nuevaMatricula) != -1 && nuevaMatricula != matricula) {
                    cout << "Error: Ya existe un alumno con esa matrícula." << endl;
                    return;
                }
                alumnos[indice].matricula = nuevaMatricula;
                cout << "\nAlumno modificado exitosamente." << endl;
                break;
            }
            case 3: {
                double cal1, cal2, cal3;
                if (leerCalificacion(cal1, "Nueva calificación parcial 1: ")) {
                    if (leerCalificacion(cal2, "Nueva calificación parcial 2: ")) {
                        if (leerCalificacion(cal3, "Nueva calificación parcial 3: ")) {
                            alumnos[indice].calificacion1 = cal1;
                            alumnos[indice].calificacion2 = cal2;
                            alumnos[indice].calificacion3 = cal3;
                            alumnos[indice].calcularCalificacionFinal();
                            cout << "\n Alumno modificado exitosamente." << endl;
                        }
                    }
                }
                break;
            }
            case 4: {
                cout << "Nuevo nombre: ";
                string nuevoNombre;
                getline(cin, nuevoNombre);
                if (!validarCadenaNoVacia(nuevoNombre, "Nombre")) {
                    return;
                }
                
                string nuevaMatricula;
                cout << "Nueva matrícula: ";
                getline(cin, nuevaMatricula);
                if (!validarCadenaNoVacia(nuevaMatricula, "Matrícula")) {
                    return;
                }
                
                if (buscarIndice(nuevaMatricula) != -1 && nuevaMatricula != matricula) {
                    cout << "Error: Ya existe un alumno con esa matrícula." << endl;
                    return;
                }
                
                double cal1, cal2, cal3;
                if (leerCalificacion(cal1, "Nueva calificación parcial 1: ")) {
                    if (leerCalificacion(cal2, "Nueva calificación parcial 2: ")) {
                        if (leerCalificacion(cal3, "Nueva calificación parcial 3: ")) {
                            alumnos[indice].nombre = nuevoNombre;
                            alumnos[indice].matricula = nuevaMatricula;
                            alumnos[indice].calificacion1 = cal1;
                            alumnos[indice].calificacion2 = cal2;
                            alumnos[indice].calificacion3 = cal3;
                            alumnos[indice].calcularCalificacionFinal();
                            cout << "\n Alumno modificado exitosamente." << endl;
                        }
                    }
                }
                break;
            }
            case 5:
                cout << "Operación cancelada." << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
                return;
        }
    }
    
    
    void eliminarAlumno() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        string matricula;
        cout << "\n=== ELIMINAR ALUMNO ===" << endl;
        cout << "Ingrese la matrícula del alumno a eliminar: ";
        cin >> matricula;
        
        int indice = buscarIndice(matricula);
        if (indice == -1) {
            cout << " Alumno no encontrado." << endl;
            return;
        }
        
        cout << "\nEstá seguro de eliminar a " << alumnos[indice].nombre 
             << " (Matrícula: " << alumnos[indice].matricula << ")? (s/n): ";
        char confirmacion;
        cin >> confirmacion;
        limpiarBuffer();
        
        if (confirmacion == 's' || confirmacion == 'S') {
            
            for (int i = indice; i < cantidadAlumnos - 1; i++) {
                alumnos[i] = alumnos[i + 1];
            }
            cantidadAlumnos--;
            cout << "\nAlumno eliminado exitosamente." << endl;
        } else {
            cout << "Operación cancelada." << endl;
        }
    }
    
    
    void mostrarAlumno(int indice) const {
        if (indice < 0 || indice >= cantidadAlumnos) return;
        
        cout << "\n" << string(40, '-') << endl;
        cout << "   INFORMACIÓN DEL ALUMNO" << endl;
        cout << string(40, '-') << endl;
        cout << left << setw(15) << "Nombre:" << alumnos[indice].nombre << endl;
        cout << setw(15) << "Matrícula:" << alumnos[indice].matricula << endl;
        cout << setw(15) << "Parcial 1:" << fixed << setprecision(2) 
             << alumnos[indice].calificacion1 << endl;
        cout << setw(15) << "Parcial 2:" << alumnos[indice].calificacion2 << endl;
        cout << setw(15) << "Parcial 3:" << alumnos[indice].calificacion3 << endl;
        cout << setw(15) << "Calificación Final:" << alumnos[indice].calificacionFinal << endl;
        cout << setw(15) << "Estado:" 
             << (alumnos[indice].estaAprobado() ? "APROBADO" : "REPROBADO") << endl;
        cout << string(40, '-') << endl;
    }
    
    
    double calcularPromedioGeneral() const {
        if (cantidadAlumnos == 0) return 0.0;
        
        double suma = 0.0;
        for (int i = 0; i < cantidadAlumnos; i++) {
            suma += alumnos[i].calificacionFinal;
        }
        return round((suma / cantidadAlumnos) * 100.0) / 100.0;
    }
    
    
    double calcularDesviacionEstandar() const {
        if (cantidadAlumnos == 0) return 0.0;
        
        double promedio = calcularPromedioGeneral();
        double sumaCuadrados = 0.0;
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            double diferencia = alumnos[i].calificacionFinal - promedio;
            sumaCuadrados += diferencia * diferencia;
        }
        
        double varianza = sumaCuadrados / cantidadAlumnos;
        return round(sqrt(varianza) * 100.0) / 100.0;
    }
    
    
    void buscarYAnalizar() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        string matricula;
        cout << "\n=== BÚSQUEDA Y ANÁLISIS ===" << endl;
        cout << "Ingrese la matrícula: ";
        cin >> matricula;
        
        int indice = buscarIndice(matricula);
        if (indice == -1) {
            cout << " Alumno no encontrado." << endl;
            return;
        }
        
        mostrarAlumno(indice);
        
        double promedioGeneral = calcularPromedioGeneral();
        cout << "\n--- ANÁLISIS COMPARATIVO ---" << endl;
        cout << "Promedio General del Grupo: " << fixed << setprecision(2) 
             << promedioGeneral << endl;
        
        double diferencia = alumnos[indice].calificacionFinal - promedioGeneral;
        if (diferencia > 0.01) {
            cout << "El alumno está ARRIBA del promedio general (+" 
                 << fixed << setprecision(2) << diferencia << " puntos)." << endl;
        } else if (diferencia < -0.01) {
            cout << "El alumno está ABAJO del promedio general (" 
                 << fixed << setprecision(2) << diferencia << " puntos)." << endl;
        } else {
            cout << "El alumno está EN el promedio general." << endl;
        }
    }
    
    
    void listarAprobados() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        cout << "\n=== ALUMNOS APROBADOS ===" << endl;
        bool hayAprobados = false;
        int contador = 1;
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            if (alumnos[i].estaAprobado()) {
                hayAprobados = true;
                cout << "\n" << contador++ << ". " << alumnos[i].nombre 
                     << " (Matrícula: " << alumnos[i].matricula << ")" << endl;
                cout << "   Calificación Final: " << fixed << setprecision(2) 
                     << alumnos[i].calificacionFinal << endl;
            }
        }
        
        if (!hayAprobados) {
            cout << "No hay alumnos aprobados." << endl;
        } else {
            cout << "\nTotal: " << (contador - 1) << " alumno(s) aprobado(s)." << endl;
        }
    }
    
    
    void listarReprobados() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        cout << "\n=== ALUMNOS REPROBADOS ===" << endl;
        bool hayReprobados = false;
        int contador = 1;
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            if (!alumnos[i].estaAprobado()) {
                hayReprobados = true;
                cout << "\n" << contador++ << ". " << alumnos[i].nombre 
                     << " (Matrícula: " << alumnos[i].matricula << ")" << endl;
                cout << "   Calificación Final: " << fixed << setprecision(2) 
                     << alumnos[i].calificacionFinal << endl;
            }
        }
        
        if (!hayReprobados) {
            cout << "No hay alumnos reprobados." << endl;
        } else {
            cout << "\nTotal: " << (contador - 1) << " alumno(s) reprobado(s)." << endl;
        }
    }
    
    
    void generarReporteGeneral() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados para generar el reporte." << endl;
            return;
        }
        
        ofstream archivo("reporte.txt");
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo crear el archivo reporte.txt" << endl;
            return;
        }
        
        archivo << string(50, '=') << endl;
        archivo << "     REPORTE GENERAL DE ALUMNOS" << endl;
        archivo << string(50, '=') << endl << endl;
        
        archivo << "Fecha de generación: " << __DATE__ << " " << __TIME__ << endl;
        archivo << "Total de Alumnos: " << cantidadAlumnos << endl << endl;
        
        archivo << string(50, '-') << endl;
        archivo << "INFORMACIÓN DE ALUMNOS" << endl;
        archivo << string(50, '-') << endl;
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            archivo << "\nAlumno " << (i + 1) << ":" << endl;
            archivo << "  Nombre: " << alumnos[i].nombre << endl;
            archivo << "  Matrícula: " << alumnos[i].matricula << endl;
            archivo << "  Calificación Parcial 1: " << fixed << setprecision(2) 
                    << alumnos[i].calificacion1 << endl;
            archivo << "  Calificación Parcial 2: " << alumnos[i].calificacion2 << endl;
            archivo << "  Calificación Parcial 3: " << alumnos[i].calificacion3 << endl;
            archivo << "  Calificación Final: " << alumnos[i].calificacionFinal << endl;
            archivo << "  Estado: " << (alumnos[i].estaAprobado() ? "APROBADO" : "REPROBADO") << endl;
        }
        
        archivo << "\n" << string(50, '-') << endl;
        archivo << "ESTADÍSTICAS GENERALES" << endl;
        archivo << string(50, '-') << endl;
        
        double promedio = calcularPromedioGeneral();
        double desviacion = calcularDesviacionEstandar();
        
        archivo << "Promedio General: " << fixed << setprecision(2) << promedio << endl;
        archivo << "Desviación Estándar: " << desviacion << endl;
        
        
        int aprobados = 0, reprobados = 0;
        double calMax = 0, calMin = 100;
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            if (alumnos[i].estaAprobado()) aprobados++;
            else reprobados++;
            
            if (alumnos[i].calificacionFinal > calMax) calMax = alumnos[i].calificacionFinal;
            if (alumnos[i].calificacionFinal < calMin) calMin = alumnos[i].calificacionFinal;
        }
        
        archivo << "Alumnos Aprobados: " << aprobados << " (" 
                << fixed << setprecision(1) 
                << (aprobados * 100.0 / cantidadAlumnos) << "%)" << endl;
        archivo << "Alumnos Reprobados: " << reprobados << " (" 
                << (reprobados * 100.0 / cantidadAlumnos) << "%)" << endl;
        archivo << "Calificación Máxima: " << fixed << setprecision(2) << calMax << endl;
        archivo << "Calificación Mínima: " << calMin << endl;
        
        archivo << "\n" << string(50, '=') << endl;
        
        archivo.close();
        
        if (archivo.good() || archivo.eof()) {
            cout << "\nReporte generado exitosamente en reporte.txt" << endl;
        } else {
            cerr << "Advertencia: Puede haber ocurrido un error al escribir el archivo." << endl;
        }
    }
    
    
    void top3MejoresCalificaciones() {
        if (cantidadAlumnos == 0) {
            cout << "\nNo hay alumnos registrados." << endl;
            return;
        }
        
        
        Alumno* copia = new (nothrow) Alumno[cantidadAlumnos];
        if (!copia) {
            cerr << "Error: No se pudo asignar memoria." << endl;
            return;
        }
        
        for (int i = 0; i < cantidadAlumnos; i++) {
            copia[i] = alumnos[i];
        }
        
        
        bool intercambio;
        for (int i = 0; i < cantidadAlumnos - 1; i++) {
            intercambio = false;
            for (int j = 0; j < cantidadAlumnos - i - 1; j++) {
                if (copia[j].calificacionFinal < copia[j + 1].calificacionFinal) {
                    Alumno temp = copia[j];
                    copia[j] = copia[j + 1];
                    copia[j + 1] = temp;
                    intercambio = true;
                }
            }
            if (!intercambio) break; 
        }
        
        cout << "\n=== TOP 3 MEJORES CALIFICACIONES ===" << endl;
        int mostrar = min(3, cantidadAlumnos);
        
        for (int i = 0; i < mostrar; i++) {
            cout << "\n" << (i + 1) << ". " << copia[i].nombre 
                 << " (Matrícula: " << copia[i].matricula << ")" << endl;
            cout << "   Calificación Final: " << fixed << setprecision(2) 
                 << copia[i].calificacionFinal << endl;
        }
        
        delete[] copia;
    }
    
    
    int getCantidadAlumnos() const {
        return cantidadAlumnos;
    }
};


bool leerOpcion(int& opcion, int min, int max) {
    if (!(cin >> opcion)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return opcion >= min && opcion <= max;
}


void mostrarMenuPrincipal() {
    cout << "\n" << string(50, '=') << endl;
    cout << "   SISTEMA ADMINISTRATIVO ESCOLAR" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Módulo de Alumnos" << endl;
    cout << "2. Módulo de Reportes" << endl;
    cout << "3. Salir" << endl;
    cout << string(50, '=') << endl;
    cout << "Seleccione una opción: ";
}


void mostrarMenuAlumnos() {
    cout << "\n" << string(40, '-') << endl;
    cout << "   MÓDULO DE ALUMNOS" << endl;
    cout << string(40, '-') << endl;
    cout << "1. Agregar Alumno" << endl;
    cout << "2. Leer Alumno" << endl;
    cout << "3. Modificar Alumno" << endl;
    cout << "4. Eliminar Alumno" << endl;
    cout << "5. Volver al Menú Principal" << endl;
    cout << string(40, '-') << endl;
    cout << "Seleccione una opción: ";
}


void mostrarMenuReportes() {
    cout << "\n" << string(40, '-') << endl;
    cout << "   MÓDULO DE REPORTES" << endl;
    cout << string(40, '-') << endl;
    cout << "1. Buscar y Analizar Alumno" << endl;
    cout << "2. Listar Alumnos Aprobados" << endl;
    cout << "3. Listar Alumnos Reprobados" << endl;
    cout << "4. Generar Reporte General" << endl;
    cout << "5. Top 3 Mejores Calificaciones" << endl;
    cout << "6. Volver al Menú Principal" << endl;
    cout << string(40, '-') << endl;
    cout << "Seleccione una opción: ";
}

int main() {
    SistemaAdministrativo sistema;
    
    
    cout << "\n" << string(50, '=') << endl;
    cout << "   INICIALIZANDO SISTEMA" << endl;
    cout << string(50, '=') << endl;
    cout << "Cargando datos desde alumnos.txt..." << endl;
    
    if (sistema.cargarDesdeArchivo("alumnos.txt")) {
        cout << "Datos cargados exitosamente. (" 
             << sistema.getCantidadAlumnos() << " alumno(s) registrado(s))" << endl;
    } else {
        cout << "No se encontró el archivo alumnos.txt. Se creará uno nuevo al guardar." << endl;
    }
    
    int opcionPrincipal, opcionModulo;
    bool salir = false;
    
    while (!salir) {
        mostrarMenuPrincipal();
        
        if (!leerOpcion(opcionPrincipal, 1, 3)) {
            cout << "\nOpción inválida. Por favor, ingrese un número entre 1 y 3." << endl;
            continue;
        }
        
        switch (opcionPrincipal) {
            case 1: 
                do {
                    mostrarMenuAlumnos();
                    
                    if (!leerOpcion(opcionModulo, 1, 5)) {
                        cout << "\nOpción inválida. Por favor, ingrese un número entre 1 y 5." << endl;
                        continue;
                    }
                    
                    switch (opcionModulo) {
                        case 1:
                            if (sistema.agregarAlumno()) {
                                if (sistema.guardarEnArchivo("alumnos.txt")) {
                                    cout << "Cambios guardados en archivo." << endl;
                                } else {
                                    cerr << "Error al guardar cambios." << endl;
                                }
                            }
                            break;
                        case 2:
                            sistema.leerAlumno();
                            break;
                        case 3:
                            sistema.modificarAlumno();
                            if (sistema.guardarEnArchivo("alumnos.txt")) {
                                cout << "Cambios guardados en archivo." << endl;
                            } else {
                                cerr << "Error al guardar cambios." << endl;
                            }
                            break;
                        case 4:
                            sistema.eliminarAlumno();
                            if (sistema.guardarEnArchivo("alumnos.txt")) {
                                cout << "Cambios guardados en archivo." << endl;
                            } else {
                                cerr << "Error al guardar cambios." << endl;
                            }
                            break;
                        case 5:
                            break;
                    }
                } while (opcionModulo != 5);
                break;
                
            case 2: 
                do {
                    mostrarMenuReportes();
                    
                    if (!leerOpcion(opcionModulo, 1, 6)) {
                        cout << "\nOpción inválida. Por favor, ingrese un número entre 1 y 6." << endl;
                        continue;
                    }
                    
                    switch (opcionModulo) {
                        case 1:
                            sistema.buscarYAnalizar();
                            break;
                        case 2:
                            sistema.listarAprobados();
                            break;
                        case 3:
                            sistema.listarReprobados();
                            break;
                        case 4:
                            sistema.generarReporteGeneral();
                            break;
                        case 5:
                            sistema.top3MejoresCalificaciones();
                            break;
                        case 6:
                            break;
                    }
                } while (opcionModulo != 6);
                break;
                
            case 3: 
                if (sistema.guardarEnArchivo("alumnos.txt")) {
                    cout << "\nDatos guardados exitosamente." << endl;
                } else {
                    cerr << "\nAdvertencia: Hubo un problema al guardar los datos." << endl;
                }
                cout << "\nGracias por usar el Sistema Administrativo Escolar" << endl;
                cout << "Hasta luego" << endl;
                salir = true;
                break;
        }
    }
    
    return 0;
}
