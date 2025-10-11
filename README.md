# Simuladores Avanzados de Gestión de Memoria
## Midterm II - Sistemas Operativos 2025-2

## Descripción General

Implementación completa de simuladores de gestión de memoria con algoritmos First Fit, Best Fit y Worst Fit. El proyecto incluye funcionalidades avanzadas como selección dinámica de archivos, análisis detallado de fragmentación (Tarea 3), y herramientas de visualización interactiva desarrolladas en C++17.

## Objetivos Cumplidos

✅ **Identificar la memoria física** - Visualización gráfica y mapeo detallado  
✅ **Aplicar principios de memoria virtual** - Simulación completa de asignación/liberación  
✅ **Gestionar solicitudes dinámicas** - Modo interactivo y desde archivo  
✅ **Implementar algoritmos de asignación** - First Fit, Best Fit, Worst Fit completamente funcionales  
✅ **Presentar estado de memoria** - Representación visual y estadísticas detalladas  
✅ **Analizar fragmentación** - **TAREA 3**: Métricas avanzadas de fragmentación interna/externa

## Arquitectura del Proyecto

### Estructura de Archivos
```
├── Algoritmos/Tareas/
│   ├── tarea1.cpp          # Simulador First Fit avanzado
│   ├── tarea2.cpp          # Comparador multi-algoritmo
│   ├── tarea1.exe          # Ejecutable compilado
│   └── tarea2.exe          # Ejecutable compilado
├── Test/
│   ├── ejemplo_tarea1.txt  # Archivo de prueba básico
│   ├── ejemplo_tarea2.txt  # Archivo de prueba básico
│   ├── test_*.txt          # 5 archivos de prueba complejos
├── ANALISIS_COMPARATIVO.md # Reporte detallado de resultados
├── COMANDOS.md             # Guía completa de comandos
├── GUION.md               # Scripts para videos demostrativos
└── Makefile               # Compilación automatizada C++17
```

## Funcionalidades Implementadas

### Tarea 1: Simulador First Fit Avanzado
**Archivo:** `tarea1.cpp` | **Tecnología:** C++17 + std::filesystem

#### Características Principales
- **🔧 Configuración flexible**: Tamaño de memoria configurable (mínimo 100 unidades)
- **📁 Selección dinámica de archivos**: Escaneo automático del directorio Test/
- **🎮 Modo dual**: Interactivo (shell) o ejecución desde archivo
- **🧠 Algoritmo First Fit optimizado**: Con fusión automática de bloques libres
- **📊 Visualización avanzada**: Mapa gráfico y representación textual
- **⚡ Análisis en tiempo real**: Estadísticas dinámicas de memoria

#### Comandos Disponibles
- `A <proceso> <tamaño>` - Asignar memoria con validaciones
- `L <proceso>` - Liberar memoria con fusión automática
- `M` - Mostrar mapa completo de memoria
- `S` - Estadísticas detalladas de utilización
- `FR` - **TAREA 3**: Análisis de fragmentación interna/externa
- `F [archivo]` - Ejecutar archivo (selección dinámica si no se especifica)
- `Q` - Salir

### Tarea 2: Comparador Multi-Algoritmo  
**Archivo:** `tarea2.cpp` | **Tecnología:** C++17 + Algoritmos avanzados

#### Algoritmos Implementados
- **🥇 First Fit**: Primer bloque libre suficiente (velocidad optimizada)
- **🎯 Best Fit**: Bloque libre más pequeño que quepa (eficiencia de espacio)
- **🏆 Worst Fit**: Bloque libre más grande (fragmentos grandes disponibles)

#### Características Avanzadas
- **🔄 Cambio dinámico**: Comando `ALG <1|2|3>` para cambiar algoritmo en tiempo real
- **📋 Parámetros flexibles**: Línea de comandos o configuración interactiva
- **📈 Análisis comparativo**: Métricas lado a lado de rendimiento
- **🎛️ Modos de visualización**: Simple (`M`) y detallado (`D`)

### Tarea 3: Análisis de Fragmentación (Integrada)
**Comando:** `FR` | **Disponible en:** Ambos simuladores

#### Métricas Calculadas
- **📉 Fragmentación Externa**: Bloques libres inutilizables (< 10 unidades)
- **📊 Fragmentación Interna**: Desperdicio dentro de bloques asignados
- **🏷️ Clasificación de fragmentos**:
  - 🔴 **Críticos** (1-5 unidades): Prácticamente inutilizables
  - 🟡 **Pequeños** (6-15 unidades): Utilidad limitada  
  - 🟢 **Utilizables** (>15 unidades): Aprovechables
- **🎯 Recomendaciones específicas**: Por algoritmo y situación

## Compilación y Ejecución

### Requisitos del Sistema
- **Compilador**: GCC con soporte C++17 o superior
- **Sistema Operativo**: Windows/Linux/macOS  
- **Dependencias**: std::filesystem (incluido en C++17)

### Compilación Automatizada
```bash
# Compilar todos los programas (Recomendado)
make all

# Verificar compilación exitosa
dir "Algoritmos\Tareas\*.exe"  # Windows
ls -la Algoritmos/Tareas/*.exe  # Linux/macOS

# Limpiar archivos compilados
make clean
```

### Compilación Manual
```bash
# Navegar al directorio de trabajo
cd "Algoritmos/Tareas"

# Compilar Tarea 1 (First Fit)
g++ -std=c++17 -Wall -Wextra -g tarea1.cpp -o tarea1.exe

# Compilar Tarea 2 (Multi-algoritmo)  
g++ -std=c++17 -Wall -Wextra -g tarea2.cpp -o tarea2.exe
```

### Ejecución Rápida
```bash
# Ejecutar con archivos de ejemplo
make run-tarea1    # Tarea 1 con ejemplo básico
make run-tarea2    # Tarea 2 con los 3 algoritmos

# Ejecución interactiva (Recomendado)
cd Algoritmos/Tareas
./tarea1.exe       # Configuración guiada paso a paso
./tarea2.exe       # Selección de algoritmo y archivo
```

### Tarea 2: Algoritmos de Asignación
```bash
## Archivos de Prueba Incluidos

### Archivos Básicos
- **📄 ejemplo_tarea1.txt**: Secuencia básica para demostrar First Fit
- **📄 ejemplo_tarea2.txt**: Casos de prueba para comparar algoritmos

### Archivos de Prueba Complejos (Tarea 3)
- **🧪 test_fragmentacion_progresiva.txt**: Evalúa fragmentación progresiva con patrones específicos
- **🎯 test_best_fit_ventaja.txt**: Demuestra escenarios donde Best Fit es superior
- **📊 test_densidad_alta.txt**: Múltiples asignaciones pequeñas para probar fragmentación
- **⚡ test_worst_fit_extremo.txt**: Casos extremos para evaluar Worst Fit
- **🔄 test_comparacion_mixta.txt**: Comparación compleja de todos los algoritmos

## Guías y Documentación

### 📚 Documentos Incluidos
- **`COMANDOS.md`**: Guía completa de comandos y ejemplos de uso
- **`GUION.md`**: Scripts detallados para crear videos demostrativos  
- **`ANALISIS_COMPARATIVO.md`**: Reporte técnico con resultados y métricas
- **`Makefile`**: Automatización de compilación y pruebas

### 🎥 Videos Demostrativos
1. **Video 1**: Tarea 1 - Simulador First Fit con funcionalidades avanzadas
2. **Video 2**: Tarea 2 - Comparación de algoritmos First/Best/Worst Fit
3. **Video 3**: Tarea 3 - Análisis detallado de fragmentación interna/externa

## Ejemplos de Uso Práctico

### Ejemplo 1: Análisis de Fragmentación
```bash
# Ejecutar simulador con archivo complejo
cd Algoritmos/Tareas
./tarea1.exe

# Configuración:
# - Memoria: 200 unidades
# - Modo: Archivo (opción 2)
# - Archivo: test_fragmentacion_progresiva.txt

# Al final ejecutar comando FR para análisis detallado
```

### Ejemplo 2: Comparación de Algoritmos
```bash
# Probar mismo archivo con diferentes algoritmos
./tarea2.exe 150 1 ../../Test/test_best_fit_ventaja.txt  # First Fit
./tarea2.exe 150 2 ../../Test/test_best_fit_ventaja.txt  # Best Fit  
./tarea2.exe 150 3 ../../Test/test_best_fit_ventaja.txt  # Worst Fit

# Comparar resultados de fragmentación
```

### Ejemplo 3: Modo Interactivo Avanzado
```bash
./tarea2.exe
# Configuración interactiva:
# - Memoria: 300
# - Algoritmo: 2 (Best Fit)
# - Archivo: Selección dinámica
# 
# Durante ejecución usar:
# - ALG 1  (cambiar a First Fit)
# - FR     (análisis de fragmentación)
# - S      (estadísticas detalladas)
```

## Características Técnicas Avanzadas

### 🔧 Tecnologías Utilizadas
- **Lenguaje**: C++17 con características modernas
- **Bibliotecas**: std::filesystem, std::vector, std::map, std::algorithm
- **Compilador**: GCC con flags -std=c++17 -Wall -Wextra -g
- **Arquitectura**: Orientada a objetos con clases especializadas

### ⚡ Optimizaciones Implementadas
- **Fusión automática**: Bloques libres adyacentes se combinan automáticamente
- **Búsqueda optimizada**: Algoritmos implementados con complejidad O(n) eficiente
- **Gestión de memoria**: Sin memory leaks, gestión automática de contenedores
- **Validaciones robustas**: Prevención de errores y estados inconsistentes

### 📊 Métricas y Análisis
- **Fragmentación externa**: Cálculo dinámico de bloques inutilizables
- **Fragmentación interna**: Análisis de desperdicio dentro de asignaciones
- **Eficiencia de utilización**: Porcentaje de memoria efectivamente aprovechada
- **Clasificación inteligente**: Fragmentos categorizados por utilidad

## Resultados y Conclusiones

### 🏆 Rendimiento por Algoritmo

| Algoritmo | Velocidad | Eficiencia Espacio | Fragmentación | Uso Recomendado |
|-----------|-----------|-------------------|---------------|-----------------|
| **First Fit** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | Media | Aplicaciones de tiempo real |
| **Best Fit** | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | Baja interna | Sistemas con memoria limitada |
| **Worst Fit** | ⭐⭐⭐ | ⭐⭐ | Baja externa | Apps con asignaciones grandes |

### 📈 Hallazgos Principales
- **First Fit**: Excelente velocidad, fragmentación concentrada al inicio
- **Best Fit**: Mejor utilización de espacio, pero puede crear fragmentos pequeños
- **Worst Fit**: Mantiene fragmentos grandes disponibles, mayor fragmentación interna

## Contribuciones y Mejoras

### ✨ Funcionalidades Añadidas
- 🔍 **Selección dinámica de archivos**: Interfaz intuitiva para elegir tests
- 📊 **Análisis de fragmentación avanzado**: Métricas detalladas en tiempo real  
- 🎮 **Modo interactivo mejorado**: Configuración guiada paso a paso
- 📝 **Documentación completa**: Guías, ejemplos y scripts de video
- 🧪 **Suite de pruebas extensa**: 5 archivos de test complejos diseñados específicamente

### 🔮 Posibles Mejoras Futuras
- Implementación de algoritmos híbridos
- Interfaz gráfica para visualización
- Análisis de rendimiento automatizado
- Soporte para diferentes tamaños de bloque
- Simulación de compactación de memoria

---

## 📞 Información del Proyecto

**Materia**: Sistemas Operativos  
**Periodo**: 2025-2 Midterm II  
**Tecnología**: C++17 + std::filesystem  
**Documentación**: Completa con guías y análisis detallado

Para más detalles técnicos, consultar `ANALISIS_COMPARATIVO.md` y `COMANDOS.md`.

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