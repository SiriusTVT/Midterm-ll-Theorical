# Makefile para compilar archivos CPP en Algoritmos/Tareas

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TAREAS_DIR = Algoritmos/Tareas
TEST_DIR = Test

# Buscar todos los archivos .cpp en la carpeta Tareas
SOURCES = $(wildcard $(TAREAS_DIR)/*.cpp)
# Generar nombres de ejecutables (sin extensión .cpp, agregando .exe)
TARGETS = $(SOURCES:.cpp=.exe)

# Regla principal: compilar todos los archivos CPP
all: $(TARGETS)

# Regla para compilar cada archivo CPP individualmente
$(TAREAS_DIR)/%.exe: $(TAREAS_DIR)/%.cpp
	@echo "Compilando $<..."
	cd $(TAREAS_DIR) && $(CXX) $(CXXFLAGS) -o $(notdir $@) $(notdir $<)

# Regla para limpiar archivos compilados
clean:
	@echo "Limpiando archivos compilados..."
	del /Q "$(TAREAS_DIR)\*.exe" 2>nul || true

# Regla para ejecutar Tarea 1
run-tarea1: $(TAREAS_DIR)/tarea1.exe
	@echo "Ejecutando Tarea 1..."
	cd $(TAREAS_DIR) && tarea1.exe ..\..\$(TEST_DIR)\ejemplo_tarea1.txt

# Regla para ejecutar Tarea 2 con los tres algoritmos
run-tarea2: $(TAREAS_DIR)/tarea2.exe
	@echo "Ejecutando Tarea 2 con los tres algoritmos..."
	@echo ""
	@echo "=== FIRST FIT ==="
	cd $(TAREAS_DIR) && tarea2.exe 100 1 ..\..\$(TEST_DIR)\ejemplo_tarea2.txt
	@echo ""
	@echo "=== BEST FIT ==="
	cd $(TAREAS_DIR) && tarea2.exe 100 2 ..\..\$(TEST_DIR)\ejemplo_tarea2.txt
	@echo ""
	@echo "=== WORST FIT ==="
	cd $(TAREAS_DIR) && tarea2.exe 100 3 ..\..\$(TEST_DIR)\ejemplo_tarea2.txt

# Regla para mostrar ayuda
help:
	@echo "Makefile para compilar archivos CPP en Algoritmos/Tareas"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make all       - Compilar todos los archivos .cpp"
	@echo "  make clean     - Eliminar archivos .exe"
	@echo "  make run-tarea1 - Ejecutar Tarea 1"
	@echo "  make run-tarea2 - Ejecutar Tarea 2 con los 3 algoritmos"
	@echo "  make help      - Mostrar esta ayuda"

.PHONY: all clean run-tarea1 run-tarea2 help

# Regla para probar diferencias entre algoritmos
test-differences: $(TAREAS_DIR)/memory_manager_tarea2.exe
	@echo "Demostrando diferencias entre algoritmos..."
	@echo ""
	@echo "=== FIRST FIT ==="
	cd $(TAREAS_DIR) && memory_manager_tarea2.exe 120 1 ..\..\$(TEST_DIR)\tarea2_test_diferencias_criticas.txt
	@echo ""
	@echo "=== BEST FIT ==="
	cd $(TAREAS_DIR) && memory_manager_tarea2.exe 120 2 ..\..\$(TEST_DIR)\tarea2_test_diferencias_criticas.txt
	@echo ""
	@echo "=== WORST FIT ==="
	cd $(TAREAS_DIR) && memory_manager_tarea2.exe 120 3 ..\..\$(TEST_DIR)\tarea2_test_diferencias_criticas.txt

# Regla para test de evaluación
test-evaluation: $(TAREAS_DIR)/memory_manager_tarea2.exe
	@echo "Ejecutando test de evaluacion..."
	@echo ""
	@echo "=== FIRST FIT ==="
	cd $(TAREAS_DIR) && memory_manager_tarea2.exe 100 1 ..\..\$(TEST_DIR)\tarea2_test_evaluacion.txt
	@echo ""
	@echo "=== BEST FIT ==="
	cd $(TAREAS_DIR) && memory_manager_tarea2.exe 100 2 ..\..\$(TEST_DIR)\tarea2_test_evaluacion.txt
	@echo ""
	@echo "=== WORST FIT ==="
	cd $(TAREAS_DIR) && memory_manager_tarea2.exe 100 3 ..\..\$(TEST_DIR)\tarea2_test_evaluacion.txt

# Regla para ejecutar con archivo de prueba de Tarea 1
test: $(TAREAS_DIR)/memory_manager.exe
	@echo "Ejecutando con archivo de prueba Tarea 1..."
	cd $(TAREAS_DIR) && (echo 100 && echo 2 && echo ..\..\$(TEST_DIR)\tarea1_test_basico.txt && echo n) | memory_manager.exe

# Regla para probar Tarea 1 con comandos básicos
test-tarea1-basic: $(TAREAS_DIR)/memory_manager.exe
	@echo "Probando Tarea 1 con comandos basicos..."
	cd $(TAREAS_DIR) && memory_manager.exe ..\..\$(TEST_DIR)\tarea1_test_basico.txt

# Regla para probar Tarea 1 con test completo
test-tarea1-complete: $(TAREAS_DIR)/memory_manager.exe
	@echo "Probando Tarea 1 con test completo..."
	cd $(TAREAS_DIR) && memory_manager.exe ..\..\$(TEST_DIR)\tarea1_test_completo.txt

# Regla para mostrar ayuda
help:
	@echo "Makefile para compilar archivos CPP en Algoritmos/Tareas"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make all              - Compilar todos los archivos .cpp en Tareas"
	@echo "  make clean            - Eliminar todos los archivos .exe en Tareas"
	@echo "  make run-tarea1       - Ejecutar memory_manager.exe (Tarea 1)"
	@echo "  make run-tarea2       - Mostrar ayuda para ejecutar Tarea 2"
	@echo "  make test-algorithms  - Probar todos los algoritmos con ejemplo"
	@echo "  make test-differences - Demostrar diferencias entre algoritmos"
	@echo "  make test-evaluation  - Ejecutar test de evaluacion"
	@echo "  make test-tarea1-basic - Probar Tarea 1 con comandos basicos"
	@echo "  make test-tarea1-complete - Probar Tarea 1 con test completo"
	@echo "  make test             - Ejecutar memory_manager con archivo de prueba"
	@echo "  make list             - Mostrar archivos CPP encontrados"
	@echo "  make help             - Mostrar esta ayuda"
	@echo ""
	@echo "Para ejecutar Tarea 2 manualmente:"
	@echo "  cd Algoritmos/Tareas"
	@echo "  memory_manager_tarea2.exe <memoria> <algoritmo> [archivo]"
	@echo "  Ejemplo: memory_manager_tarea2.exe 100 1"
	@echo "  Algoritmos: 1=First Fit, 2=Best Fit, 3=Worst Fit"

# Regla para listar archivos CPP encontrados
list:
	@echo "Archivos CPP encontrados en $(TAREAS_DIR):"
	@echo $(SOURCES)
	@echo ""
	@echo "Ejecutables que se generaran:"
	@echo $(TARGETS)

.PHONY: all clean run-tarea1 run-tarea2 test-algorithms test-differences test-evaluation test-tarea1-basic test-tarea1-complete test help list