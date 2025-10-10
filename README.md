# Simulador de Gestión de Memoria

## Descripción General

A continuación se presenta una guía teórico-práctica para los estudiantes del curso de sistemas operativos. Esta guía permite explorar y experimentar con conceptos relacionados con memoria, asignación de memoria en sistemas paginados y aplicar los principios de la memoria virtual.

## Objetivos

Durante el desarrollo de esta guía el estudiante:

- Identificar la memoria física.
- Aplicar los principios de la memoria virtual.
- Recibir solicitudes de asignación y liberación de memoria.
- Aplicar los algoritmos de asignación: First Fit, Best Fit, Worst Fit
- Presentar el estado de la memoria tras cada operación.

## Funcionalidades Implementadas

## Funcionalidades Implementadas

### Tarea 1: Simulador Básico
- **Entrada dinámica**: El programa permite elegir entre modo interactivo (consola) o desde archivo de texto
- **Asignación de memoria (A)**: Comando `A <proceso> <tamaño>` para asignar memoria a procesos
- **Liberación de memoria (L)**: Comando `L <proceso>` para liberar memoria de procesos
- **Visualización de memoria (M)**: Comando `M` para mostrar el estado actual de la memoria
- **Memoria configurable**: Tamaño mínimo de 100 unidades, configurable al inicio
- **Algoritmo First Fit**: Busca el primer bloque libre suficientemente grande (implementado)
- **Fusión automática**: Los bloques libres adyacentes se fusionan automáticamente

### Tarea 2: Algoritmos de Asignación
- **First Fit**: Primer bloque libre que quepa el proceso
- **Best Fit**: Bloque libre más pequeño que pueda contener el proceso
- **Worst Fit**: Bloque libre más grande disponible
- **Parámetros por línea de comandos**: Tamaño de memoria, algoritmo y archivo de entrada
- **Cambio dinámico de algoritmo**: Comando `ALG` para cambiar algoritmo durante ejecución
- **Formato de salida específico**: `[Proceso: tamaño][Libre: tamaño]` según especificaciones

> **Nota**: La Tarea 1 implementa solo First Fit. La Tarea 2 implementa los tres algoritmos completos con entrada por línea de comandos.

## Compilación y Ejecución

### Tarea 1: Simulador Básico
```bash
# Compilar
make all

# Ejecutar Tarea 1
make run-tarea1
```

### Tarea 2: Algoritmos de Asignación
```bash
# Compilar
make all

# Ejecutar con parámetros específicos
cd Algoritmos/Tareas
memory_manager_tarea2.exe <memoria> <algoritmo> [archivo]

# Ejemplos:
memory_manager_tarea2.exe 100 1              # 100 unidades, First Fit, interactivo
memory_manager_tarea2.exe 150 2 ../../Test/ejemplo_tarea2.txt  # Best Fit con archivo

# Algoritmos disponibles:
# 1 = First Fit
# 2 = Best Fit  
# 3 = Worst Fit
```

### Pruebas Automatizadas
```bash
# Probar ejemplo específico de Tarea 2 con todos los algoritmos
make test-algorithms

# Demostrar diferencias entre algoritmos
make test-differences

# Ver todos los comandos disponibles
make help
```

## Comandos del Makefile

| Comando | Descripción |
|---------|-------------|
| `make all` | Compila todos los archivos .cpp en Algoritmos/Tareas |
| `make clean` | Elimina todos los archivos .exe en Algoritmos/Tareas |
| `make run-tarea1` | Ejecuta memory_manager.exe (Tarea 1) |
| `make run-tarea2` | Muestra ayuda para ejecutar Tarea 2 |
| `make test-algorithms` | Prueba todos los algoritmos con ejemplo de Tarea 2 |
| `make test-differences` | Demuestra diferencias entre algoritmos |
| `make list` | Muestra archivos CPP encontrados y ejecutables a generar |
| `make help` | Muestra la ayuda completa del Makefile |

## Ejemplo de Uso - Tarea 2

### Uso por Línea de Comandos
```bash
# Ejecutar con First Fit
memory_manager_tarea2.exe 100 1

# Ejecutar con Best Fit y archivo
memory_manager_tarea2.exe 100 2 ../../Test/ejemplo_tarea2.txt

# Resultado esperado del ejemplo:
# Entrada: A P1 10, A P2 25, L P1, A P3 8, M
# Salida: [P2: 25][Libre: 10][P3: 8][Libre: 57]  (con Worst Fit)
#         [P3: 8][Libre: 2][P2: 25][Libre: 65]   (con First/Best Fit)
```

### Demostración de Diferencias
```bash
# El comando make test-algorithms muestra cómo cada algoritmo 
# produce resultados diferentes con la misma secuencia de comandos:

# First Fit: Usa el primer bloque disponible
# Best Fit:  Usa el bloque más pequeño que sirva  
# Worst Fit: Usa el bloque más grande disponible
```

## Estructura del Proyecto

```
├── Algoritmos/
│   └── Tareas/
│       ├── memory_manager.cpp          # Tarea 1: Simulador básico
│       ├── memory_manager.exe          # Ejecutable Tarea 1
│       ├── memory_manager_tarea2.cpp   # Tarea 2: Tres algoritmos
│       └── memory_manager_tarea2.exe   # Ejecutable Tarea 2
├── Test/
│   ├── test_commands.txt               # Comandos de prueba Tarea 1
│   ├── ejemplo_tarea2.txt              # Ejemplo específico Tarea 2
│   └── test_algorithms.txt             # Prueba diferencias algoritmos
├── README.md                           # Documentación del proyecto
├── Makefile                            # Configuración de compilación
├── .gitignore                          # Archivos ignorados por Git
└── Parcial_OS2025_2.pdf               # Documento del parcial
```

## Archivos Incluidos

- `Algoritmos/Tareas/memory_manager.cpp`: Código fuente principal
- `Test/test_commands.txt`: Archivo de comandos de prueba
- `README.md`: Documentación del proyecto

## Comandos Disponibles

- `A <proceso> <tamaño>`: Asignar memoria
- `L <proceso>`: Liberar memoria
- `M`: Mostrar estado de memoria
- `S`: Mostrar estadísticas
- `F <archivo>`: Ejecutar comandos desde archivo
- `Q`: Salir

Para más detalles sobre el funcionamiento del programa, ejecute el comando `M` para ver la visualización de memoria o consulte los comentarios en el código fuente.

## Flujo de Trabajo Recomendado

1. **Desarrollo:**
   ```bash
   # Ver qué archivos detecta el Makefile
   make list
   
   # Compilar todos los programas
   make all
   ```

2. **Pruebas:**
   ```bash
   # Ejecutar programa principal
   make run
   
   # O ejecutar con archivo de prueba automático
   make test
   ```

3. **Limpieza:**
   ```bash
   # Limpiar archivos compilados
   make clean
   ```