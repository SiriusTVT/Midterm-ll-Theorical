# Makefile para compilar todos los archivos CPP en Algoritmos/Tareas

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
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

# Regla para ejecutar memory_manager (principal)
run: $(TAREAS_DIR)/memory_manager.exe
	@echo "Ejecutando memory_manager..."
	cd $(TAREAS_DIR) && memory_manager.exe

# Regla para ejecutar con archivo de prueba
test: $(TAREAS_DIR)/memory_manager.exe
	@echo "Ejecutando con archivo de prueba..."
	cd $(TAREAS_DIR) && (echo 100 && echo 2 && echo ..\..\$(TEST_DIR)\test_commands.txt && echo n) | memory_manager.exe

# Regla para mostrar ayuda
help:
	@echo "Makefile para compilar archivos CPP en Algoritmos/Tareas"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make all     - Compilar todos los archivos .cpp en Tareas"
	@echo "  make clean   - Eliminar todos los archivos .exe en Tareas"
	@echo "  make run     - Ejecutar memory_manager.exe"
	@echo "  make test    - Ejecutar memory_manager con archivo de prueba"
	@echo "  make list    - Mostrar archivos CPP encontrados"
	@echo "  make help    - Mostrar esta ayuda"

# Regla para listar archivos CPP encontrados
list:
	@echo "Archivos CPP encontrados en $(TAREAS_DIR):"
	@echo $(SOURCES)
	@echo ""
	@echo "Ejecutables que se generaran:"
	@echo $(TARGETS)

.PHONY: all clean run test help list