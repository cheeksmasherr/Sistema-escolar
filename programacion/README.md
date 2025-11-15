# Sistema Administrativo Escolar en C++



Sistema administrativo completo para la gestión de información de alumnos en instituciones educativas. El programa permite gestionar datos de estudiantes, generar reportes estadísticos y realizar análisis académicos mediante una interfaz de consola intuitiva.

##  Características Principales

### Módulo de Alumnos
- Agregar nuevos alumnos con validación completa
- Consultar información por matrícula
- Modificar datos existentes (nombre, matrícula, calificaciones)
- Eliminar alumnos con confirmación

### Módulo de Reportes
- Búsqueda y análisis detallado de alumnos
- Listado de alumnos aprobados/reprobados
-  Generación de reportes estadísticos completos
- Top 3 mejores calificaciones con ordenamiento

##  Mejoras y Optimizaciones Implementadas

### 1. **Manejo Robusto de Errores**

#### Validación de Entrada
- **Validación de calificaciones**: Las calificaciones deben estar en el rango 0-100
- **Validación de cadenas vacías**: Previene nombres y matrículas vacíos
- **Validación de formato numérico**: Manejo de excepciones para conversiones inválidas
- **Validación de opciones de menú**: Verificación de rangos válidos antes de procesar

```cpp
// Ejemplo de validación robusta
bool leerCalificacion(double& calificacion, const string& mensaje) {
    try {
        calificacion = stod(entrada);
        if (calificacion < CALIFICACION_MIN || calificacion > CALIFICACION_MAX) {
            // Manejo de error
        }
    } catch (const invalid_argument&) {
        // Manejo de excepción
    }
}
```

#### Manejo de Excepciones
- **try-catch para conversiones**: Protección contra `stod()` con datos inválidos
- **Manejo de archivos**: Verificación de apertura y escritura exitosa
- **Validación de memoria**: Uso de `nothrow` para asignación de memoria dinámica
- **Mensajes de error descriptivos**: Información clara sobre qué salió mal

### 2. **Validación de Datos al Cargar Archivos**

#### Características Implementadas
- **Validación línea por línea**: Cada línea del archivo se valida individualmente
- **Detección de formato inválido**: Identifica líneas mal formateadas
- **Validación de rangos**: Verifica que las calificaciones estén en rango válido
- **Detección de duplicados**: Previene cargar matrículas duplicadas
- **Manejo de datos corruptos**: Continúa cargando aunque algunas líneas fallen
- **Reporte de errores**: Informa cuántas líneas tuvieron problemas

```cpp
// El sistema continúa funcionando aunque algunas líneas del archivo tengan errores
// Muestra advertencias específicas para cada problema encontrado
```

### 3. **Optimizaciones de Código**

#### Memoria Dinámica Mejorada
- **Asignación segura**: Uso de `new (nothrow)` para evitar excepciones no controladas
- **Redimensionamiento eficiente**: Duplicación de capacidad solo cuando es necesario
- **Verificación de asignación**: Validación de éxito en asignaciones de memoria

#### Algoritmos Optimizados
- **Bubble Sort mejorado**: Implementación con bandera de intercambio para detenerse temprano
- **Búsqueda eficiente**: Búsqueda lineal optimizada para el tamaño típico de datos
- **Cálculos precisos**: Redondeo a 2 decimales para evitar errores de precisión flotante

```cpp
// Bubble Sort optimizado con detección temprana
bool intercambio;
for (int i = 0; i < cantidadAlumnos - 1; i++) {
    intercambio = false;
    for (int j = 0; j < cantidadAlumnos - i - 1; j++) {
        if (copia[j].calificacionFinal < copia[j + 1].calificacionFinal) {
            // Intercambio
            intercambio = true;
        }
    }
    if (!intercambio) break; // Ya está ordenado
}
```

### 4. **Mejoras en la Interfaz de Usuario**

#### Presentación Visual
- **Separadores visuales**: Uso de líneas de caracteres para mejor legibilidad
- **Símbolos informativos**: ✓ para éxito, ✗ para errores, ℹ para información
- **Formato tabular**: Alineación consistente de datos
- **Mensajes claros**: Indicadores visuales de estado de operaciones

#### Experiencia de Usuario
- **Limpieza de buffer**: Manejo correcto de entrada para evitar problemas con `cin`
- **Validación en tiempo real**: Feedback inmediato sobre errores de entrada
- **Confirmaciones**: Solicitud de confirmación para operaciones destructivas
- **Contadores**: Muestra totales en listados (ej: "Total: 5 alumno(s) aprobado(s)")

### 5. **Regla de Tres (Rule of Three)**

Implementación completa de la regla de tres para manejo seguro de memoria:
- **Constructor de copia**: Permite copiar objetos de forma segura
- **Operador de asignación**: Evita problemas de memoria al asignar
- **Destructor**: Libera memoria correctamente

```cpp
// Constructor de copia
SistemaAdministrativo(const SistemaAdministrativo& otro);

// Operador de asignación
SistemaAdministrativo& operator=(const SistemaAdministrativo& otro);

// Destructor
~SistemaAdministrativo();
```

### 6. **Validaciones de Integridad de Datos**

#### Validaciones Implementadas
- **Método `validarDatos()`**: Verifica que todos los datos del alumno sean válidos
- **Método `validarCalificaciones()`**: Asegura que las calificaciones estén en rango
- **Validación de matrículas duplicadas**: Previene inconsistencias en la base de datos
- **Validación antes de guardar**: Solo guarda datos válidos en el archivo

### 7. **Mejoras en Reportes**

#### Reporte General Mejorado
- **Estadísticas adicionales**: 
  - Porcentaje de aprobados/reprobados
  - Calificación máxima y mínima
  - Fecha de generación del reporte
- **Formato mejorado**: Separadores y estructura clara
- **Validación de escritura**: Verificación de éxito al escribir el archivo

#### Análisis Comparativo Mejorado
- **Diferencia numérica**: Muestra cuántos puntos arriba/abajo del promedio
- **Comparación precisa**: Manejo de errores de precisión flotante

### 8. **Constantes y Configuración**

- **Constantes definidas**: `CALIFICACION_MIN`, `CALIFICACION_MAX`, `CAPACIDAD_INICIAL`
- **Fácil mantenimiento**: Cambios centralizados en constantes
- **Código más legible**: Uso de nombres descriptivos

### 9. **Manejo de Casos Edge**

#### Casos Especiales Manejados
- **Archivo vacío**: El sistema funciona correctamente sin datos
- **Archivo inexistente**: Crea uno nuevo al guardar
- **Datos corruptos**: Continúa funcionando ignorando líneas inválidas
- **Sin alumnos registrados**: Mensajes apropiados en todas las operaciones
- **Memoria insuficiente**: Manejo graceful de errores de asignación

### 10. **Precisión Numérica**

- **Redondeo consistente**: Todas las calificaciones se redondean a 2 decimales
- **Cálculos precisos**: Uso de `round()` para evitar errores de precisión flotante
- **Formato consistente**: Uso de `fixed` y `setprecision(2)` en toda la salida

##  Estructura de Archivos

```
.
├── sistema.cpp          # Código fuente principal
├── alumnos.txt          # Base de datos de alumnos (formato: nombre|matricula|cal1|cal2|cal3)
├── reporte.txt          # Reporte generado (se crea automáticamente)
└── README.md           # Este archivo
```

## Compilación

```bash
g++ -o sistema sistema.cpp -std=c++11 -Wall -Wextra
```

### Opciones de Compilación Recomendadas
- `-std=c++11`: Estándar C++11
- `-Wall`: Habilita todas las advertencias
- `-Wextra`: Advertencias adicionales
- `-O2`: Optimización (opcional, para producción)

##  Uso

### Ejecución
```bash
./sistema
```

### Formato del Archivo alumnos.txt
```
Nombre Completo|Matrícula|Calificación1|Calificación2|Calificación3
Juan Pérez García|A001|85.5|90.0|88.5
María González López|A002|75.0|80.0|77.5
```

##  Seguridad y Robustez

### Validaciones Implementadas
1.  Entrada numérica validada
2.  Rangos de calificaciones verificados
3. Cadenas vacías rechazadas
4. Matrículas duplicadas detectadas
5. Archivos validados antes de leer/escribir
6. Memoria verificada después de asignación
7. Excepciones capturadas y manejadas

### Prevención de Errores
- **Buffer overflow**: Uso de `getline()` con límites seguros
- **División por cero**: Verificación antes de calcular promedios
- **Acceso fuera de rango**: Validación de índices antes de acceso
- **Datos corruptos**: Validación exhaustiva al cargar archivos

## 📊 Estadísticas y Reportes

### Cálculos Implementados
- **Promedio general**: Media aritmética de todas las calificaciones finales
- **Desviación estándar**: Medida de dispersión de las calificaciones
- **Porcentajes**: Aprobados vs reprobados
- **Extremos**: Calificación máxima y mínima

### Fórmula de Desviación Estándar
```
σ = √(Σ(xi - μ)² / N)
```
Donde:
- σ = Desviación estándar
- xi = Calificación final del alumno i
- μ = Promedio general
- N = Número de alumnos

##  Características Técnicas

### Estructuras de Datos
- **Arreglo dinámico**: Redimensionable según necesidad
- **Estructura Alumno**: Organización clara de datos
- **Gestión de memoria**: Manual con new/delete

### Algoritmos
- **Bubble Sort**: Para ordenamiento del Top 3 (optimizado)
- **Búsqueda lineal**: Para encontrar alumnos por matrícula
- **Cálculos estadísticos**: Promedio y desviación estándar

##  Flujo de Datos

1. **Inicio**: Carga automática desde `alumnos.txt`
2. **Operaciones**: Modificaciones en memoria
3. **Persistencia**: Guardado automático después de cambios
4. **Cierre**: Guardado final antes de salir

##  Notas de Implementación

### Decisiones de Diseño
- **Memoria dinámica**: Permite escalabilidad sin límites fijos
- **Validación exhaustiva**: Prioriza robustez sobre velocidad
- **Mensajes claros**: Facilita debugging y uso
- **Código modular**: Fácil mantenimiento y extensión

### Mejoras Futuras Potenciales
- Implementación con contenedores STL (vector, map)
- Persistencia en base de datos
- Interfaz gráfica
- Exportación a diferentes formatos (CSV, JSON, PDF)
- Búsqueda avanzada y filtros
- Historial de cambios

## Solución de Problemas

### Problemas Comunes

**Error al cargar archivo**
- Verifica que el formato sea correcto (nombre|matrícula|cal1|cal2|cal3)
- Asegúrate de que las calificaciones estén entre 0 y 100

**Error de memoria**
- El sistema maneja automáticamente errores de asignación
- Verifica que haya suficiente memoria disponible

**Datos no se guardan**
- Verifica permisos de escritura en el directorio
- Revisa mensajes de error en consola


##  Autores

Pedro Ramses Sanchez Zarate, Monica Guadalupe Torres Loazano, Emiliano Montalvo Rocha

