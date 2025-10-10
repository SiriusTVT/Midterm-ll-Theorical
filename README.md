# Simulador de Gestión de Memoria

Este es un programa en C++ que simula un sistema básico de gestión de memoria con las siguientes características:

## Funcionalidades Implementadas

- **Entrada dinámica**: El programa permite elegir entre modo interactivo (consola) o desde archivo de texto
- **Asignación de memoria (A)**: Comando `A <proceso> <tamaño>` para asignar memoria a procesos
- **Liberación de memoria (L)**: Comando `L <proceso>` para liberar memoria de procesos
- **Visualización de memoria (M)**: Comando `M` para mostrar el estado actual de la memoria
- **Memoria configurable**: Tamaño mínimo de 100 unidades, configurable al inicio
- **Algoritmo First Fit**: Busca el primer bloque libre suficientemente grande
- **Fusión automática**: Los bloques libres adyacentes se fusionan automáticamente

## Compilación y Ejecución

### Opción 1: Usando Makefile (Recomendado)
```bash
# Compilar todos los archivos CPP en Algoritmos/Tareas
make all

# Ejecutar el programa principal
make run

# Ejecutar con archivo de prueba automáticamente
make test

# Limpiar archivos compilados
make clean

# Ver archivos CPP detectados y ejecutables que se generarán
make list

# Ver ayuda de comandos disponibles
make help
```

### Opción 2: Compilación Manual
```bash
# Navegar a la carpeta de tareas
cd Algoritmos/Tareas

# Compilar
g++ memory_manager.cpp -o memory_manager.exe

# Ejecutar
memory_manager.exe
```

## Comandos del Makefile

| Comando | Descripción |
|---------|-------------|
| `make all` | Compila todos los archivos .cpp en Algoritmos/Tareas |
| `make clean` | Elimina todos los archivos .exe en Algoritmos/Tareas |
| `make run` | Ejecuta memory_manager.exe |
| `make test` | Ejecuta memory_manager con archivo de prueba automáticamente |
| `make list` | Muestra archivos CPP encontrados y ejecutables a generar |
| `make help` | Muestra la ayuda completa del Makefile |

## Ejemplo de Uso

```
=== SIMULADOR DE GESTIÓN DE MEMORIA ===
Configuración inicial

Ingrese el tamaño total de memoria (mínimo 100): 100
Memoria configurada: 100 unidades

¿Cómo desea introducir los comandos?
1. Desde consola (interactivo)
2. Desde archivo de texto
Seleccione una opción (1 o 2): 1

=== MODO INTERACTIVO ===
Comandos disponibles:
  A <proceso> <tamaño>  - Asignar memoria
  L <proceso>           - Liberar memoria
  M                     - Mostrar estado de la memoria
  S                     - Mostrar estadísticas
  F <archivo>           - Ejecutar comandos desde archivo
  Q                     - Salir

shell> A proceso1 20
Memoria asignada al proceso 'proceso1' - Tamaño: 20 unidades

shell> M
==================================================
MAPA DE MEMORIA (Total: 100 unidades)
==================================================
Inicio  Tamaño Estado      Proceso
--------------------------------------------------
0       20      OCUPADO     proceso1
20      80      LIBRE

shell> Q
```

### Modo Archivo
Si selecciona la opción 2, el programa le pedirá el nombre del archivo con los comandos y los ejecutará automáticamente. Después de terminar, puede optar por continuar en modo interactivo.

**Ejemplo usando el archivo de prueba:**
```
shell> 2
Ingrese el nombre del archivo con los comandos: ..\Test\test_commands.txt
```

## Estructura del Proyecto

```
├── Algoritmos/
│   └── Tareas/
│       ├── memory_manager.cpp    # Código fuente principal
│       └── memory_manager.exe    # Ejecutable compilado
├── Test/
│   └── test_commands.txt         # Archivo de comandos de prueba
├── README.md                     # Documentación del proyecto
├── Makefile                      # Configuración de compilación
└── Parcial_OS2025_2.pdf         # Documento del parcial
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