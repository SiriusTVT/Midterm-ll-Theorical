#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace fs = std::filesystem;

// Estructura para representar un bloque de memoria
struct MemoryBlock {
    int start;
    int size;
    std::string process_name;
    bool is_free;
    
    MemoryBlock(int s, int sz, std::string name = "", bool free = true) 
        : start(s), size(sz), process_name(name), is_free(free) {}
};

class MemoryManager {
private:
    int total_memory;
    std::vector<MemoryBlock> memory_blocks;
    std::map<std::string, int> process_locations; // Para rastrear dónde están los procesos
    
public:
    MemoryManager(int size = 100) : total_memory(size) {
        // Inicialmente toda la memoria está libre
        memory_blocks.push_back(MemoryBlock(0, total_memory, "", true));
    }
    
    // Método para asignar memoria (comando A)
    bool allocate(const std::string& process_name, int size) {
        if (size <= 0) {
            std::cout << "Error: El tamano debe ser positivo\n";
            return false;
        }
        
        if (process_locations.find(process_name) != process_locations.end()) {
            std::cout << "Error: El proceso '" << process_name << "' ya tiene memoria asignada\n";
            return false;
        }
        
        // Buscar el primer bloque libre que sea suficientemente grande (First Fit)
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); ++it) {
            if (it->is_free && it->size >= size) {
                // Encontramos un bloque libre suficientemente grande
                int start_pos = it->start;
                int remaining_size = it->size - size;
                
                // Crear el bloque ocupado
                MemoryBlock allocated_block(start_pos, size, process_name, false);
                
                // Reemplazar el bloque libre
                if (remaining_size > 0) {
                    // Si queda espacio, crear un nuevo bloque libre
                    it->start = start_pos + size;
                    it->size = remaining_size;
                    memory_blocks.insert(it, allocated_block);
                } else {
                    // Si no queda espacio, simplemente reemplazar
                    *it = allocated_block;
                }
                
                process_locations[process_name] = start_pos;
                std::cout << "Memoria asignada al proceso '" << process_name 
                         << "' - Tamano: " << size << " unidades\n";
                return true;
            }
        }
        
        std::cout << "Error: No hay suficiente memoria contigua disponible para el proceso '" 
                 << process_name << "'\n";
        return false;
    }
    
    // Método para liberar memoria (comando L)
    bool deallocate(const std::string& process_name) {
        auto it = process_locations.find(process_name);
        if (it == process_locations.end()) {
            std::cout << "Error: El proceso '" << process_name << "' no tiene memoria asignada\n";
            return false;
        }
        
        int process_start = it->second;
        process_locations.erase(it);
        
        // Encontrar y liberar el bloque
        for (auto& block : memory_blocks) {
            if (!block.is_free && block.start == process_start && block.process_name == process_name) {
                block.is_free = true;
                block.process_name = "";
                std::cout << "Memoria liberada del proceso '" << process_name << "'\n";
                
                // Fusionar bloques libres adyacentes
                merge_free_blocks();
                return true;
            }
        }
        
        return false;
    }
    
    // Método para fusionar bloques libres adyacentes
    void merge_free_blocks() {
        // Ordenar bloques por posición de inicio
        std::sort(memory_blocks.begin(), memory_blocks.end(), 
                 [](const MemoryBlock& a, const MemoryBlock& b) {
                     return a.start < b.start;
                 });
        
        // Fusionar bloques libres consecutivos
        for (auto it = memory_blocks.begin(); it != memory_blocks.end() - 1; ) {
            if (it->is_free && (it + 1)->is_free && 
                (it->start + it->size) == (it + 1)->start) {
                // Fusionar los bloques
                it->size += (it + 1)->size;
                memory_blocks.erase(it + 1);
            } else {
                ++it;
            }
        }
    }
    
    // Método para mostrar el estado de la memoria (comando M)
    void show_memory() {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "MAPA DE MEMORIA (Total: " << total_memory << " unidades)\n";
        std::cout << std::string(50, '=') << "\n";
        
        // Ordenar bloques por posición
        std::sort(memory_blocks.begin(), memory_blocks.end(), 
                 [](const MemoryBlock& a, const MemoryBlock& b) {
                     return a.start < b.start;
                 });
        
        std::cout << std::left << std::setw(8) << "Inicio" 
                 << std::setw(8) << "Tamano" 
                 << std::setw(12) << "Estado" 
                 << "Proceso\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (const auto& block : memory_blocks) {
            std::cout << std::left << std::setw(8) << block.start
                     << std::setw(8) << block.size
                     << std::setw(12) << (block.is_free ? "LIBRE" : "OCUPADO")
                     << (block.is_free ? "" : block.process_name) << "\n";
        }
        
        // Mostrar representación visual
        std::cout << "\nRepresentacion visual:\n";
        std::string visual_map(total_memory, '.');
        
        for (const auto& block : memory_blocks) {
            char symbol = block.is_free ? '.' : '#';
            for (int i = block.start; i < block.start + block.size; ++i) {
                visual_map[i] = symbol;
            }
        }
        
        // Mostrar la representación en líneas de 50 caracteres
        for (int i = 0; i < total_memory; i += 50) {
            std::cout << std::setw(3) << i << ": ";
            int end = std::min(i + 50, total_memory);
            for (int j = i; j < end; ++j) {
                std::cout << visual_map[j];
            }
            std::cout << "\n";
        }
        
        std::cout << "\nLeyenda: '.' = Libre, '#' = Ocupado\n";
        std::cout << std::string(50, '=') << "\n\n";
    }
    
    // Mostrar estadísticas de memoria
    void show_statistics() {
        int free_memory = 0;
        int used_memory = 0;
        int free_blocks = 0;
        int used_blocks = 0;
        
        for (const auto& block : memory_blocks) {
            if (block.is_free) {
                free_memory += block.size;
                free_blocks++;
            } else {
                used_memory += block.size;
                used_blocks++;
            }
        }
        
        std::cout << "Estadisticas de memoria:\n";
        std::cout << "- Memoria total: " << total_memory << " unidades\n";
        std::cout << "- Memoria libre: " << free_memory << " unidades (" 
                 << (100.0 * free_memory / total_memory) << "%)\n";
        std::cout << "- Memoria usada: " << used_memory << " unidades (" 
                 << (100.0 * used_memory / total_memory) << "%)\n";
        std::cout << "- Bloques libres: " << free_blocks << "\n";
        std::cout << "- Bloques ocupados: " << used_blocks << "\n";
    }
};

// Función para listar archivos en el directorio Test
std::vector<std::string> list_test_files() {
    std::vector<std::string> files;
    std::string test_dir = "../../Test";
    
    try {
        if (fs::exists(test_dir) && fs::is_directory(test_dir)) {
            for (const auto& entry : fs::directory_iterator(test_dir)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().filename().string());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al acceder al directorio Test: " << e.what() << std::endl;
    }
    
    return files;
}

// Función para mostrar y seleccionar archivo dinámicamente
std::string select_test_file() {
    std::vector<std::string> files = list_test_files();
    
    if (files.empty()) {
        std::cout << "No se encontraron archivos en el directorio Test.\n";
        std::cout << "Ingrese el nombre del archivo manualmente: ";
        std::string filename;
        std::getline(std::cin, filename);
        return filename;
    }
    
    std::cout << "\n=== ARCHIVOS DISPONIBLES EN TEST ===\n";
    for (size_t i = 0; i < files.size(); i++) {
        std::cout << (i + 1) << ". " << files[i] << "\n";
    }
    std::cout << (files.size() + 1) << ". Introducir nombre manualmente\n";
    
    std::cout << "\nSeleccione una opcion (1-" << (files.size() + 1) << "): ";
    std::string input;
    std::getline(std::cin, input);
    
    try {
        int choice = std::stoi(input);
        if (choice >= 1 && choice <= static_cast<int>(files.size())) {
            std::string selected_file = "../../Test/" + files[choice - 1];
            std::cout << "Archivo seleccionado: " << files[choice - 1] << "\n";
            return selected_file;
        } else if (choice == static_cast<int>(files.size()) + 1) {
            std::cout << "Ingrese el nombre del archivo: ";
            std::string filename;
            std::getline(std::cin, filename);
            return filename;
        } else {
            std::cout << "Opción no válida. Usando el primer archivo disponible.\n";
            std::string selected_file = "../../Test/" + files[0];
            std::cout << "Archivo seleccionado: " << files[0] << "\n";
            return selected_file;
        }
    } catch (const std::exception& e) {
        std::cout << "Entrada no válida. Usando el primer archivo disponible.\n";
        std::string selected_file = "../../Test/" + files[0];
        std::cout << "Archivo seleccionado: " << files[0] << "\n";
        return selected_file;
    }
}

class MemorySimulator {
private:
    MemoryManager memory_manager;
    
public:
    MemorySimulator(int memory_size = 100) : memory_manager(memory_size) {}
    
    void run() {
        std::cout << "=== SIMULADOR DE GESTION DE MEMORIA ===\n\n";
        
        // Preguntar al usuario cómo quiere introducir los comandos
        std::cout << "Como desea introducir los comandos?\n";
        std::cout << "1. Desde consola (interactivo)\n";
        std::cout << "2. Desde archivo de texto\n";
        std::cout << "Seleccione una opcion (1 o 2): ";
        
        std::string option;
        std::getline(std::cin, option);
        
        if (option == "1") {
            run_interactive();
        } else if (option == "2") {
            run_from_file();
        } else {
            std::cout << "Opcion no valida. Ejecutando modo interactivo por defecto.\n\n";
            run_interactive();
        }
    }
    
    void run_interactive() {
        std::cout << "\n=== MODO INTERACTIVO ===\n";
        std::cout << "Comandos disponibles:\n";
        std::cout << "  A <proceso> <tamano>  - Asignar memoria\n";
        std::cout << "  L <proceso>           - Liberar memoria\n";
        std::cout << "  M                     - Mostrar estado de la memoria\n";
        std::cout << "  S                     - Mostrar estadisticas\n";
        std::cout << "  F [archivo]           - Ejecutar comandos desde archivo (selección dinámica si no se especifica)\n";
        std::cout << "  Q                     - Salir\n\n";
        
        std::string line;
        while (true) {
            std::cout << "shell> ";
            if (!std::getline(std::cin, line)) {
                break;
            }
            
            if (!process_command(line)) {
                break;
            }
        }
    }
    
    void run_from_file() {
        std::cout << "\n=== MODO ARCHIVO ===\n";
        
        std::string filename = select_test_file();
        
        if (filename.empty()) {
            std::cout << "No se especifico archivo. Cambiando a modo interactivo.\n";
            run_interactive();
            return;
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: No se pudo abrir el archivo '" << filename << "'.\n";
            std::cout << "Cambiando a modo interactivo.\n";
            run_interactive();
            return;
        }
        
        std::cout << "Ejecutando comandos desde '" << filename << "'...\n\n";
        
        std::string line;
        int line_number = 0;
        
        while (std::getline(file, line)) {
            line_number++;
            
            // Ignorar líneas vacías y comentarios
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            std::cout << "[Linea " << line_number << "] " << line << "\n";
            process_command(line);
            std::cout << "\n"; // Espacio entre comandos para mejor legibilidad
        }
        
        file.close();
        std::cout << "=== Ejecucion del archivo completada ===\n";
        
        // Preguntar si quiere continuar en modo interactivo
        std::cout << "\nDesea continuar en modo interactivo? (s/n): ";
        std::string continue_option;
        std::getline(std::cin, continue_option);
        
        if (continue_option == "s" || continue_option == "S" || continue_option == "si" || continue_option == "SI") {
            run_interactive();
        }
    }
    
    bool process_command(const std::string& line) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        
        if (command.empty()) {
            return true;
        }
        
        // Convertir a mayúsculas
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        
        if (command == "A") {
            std::string process_name;
            int size;
            if (iss >> process_name >> size) {
                memory_manager.allocate(process_name, size);
            } else {
                std::cout << "Uso: A <proceso> <tamano>\n";
            }
        }
        else if (command == "L") {
            std::string process_name;
            if (iss >> process_name) {
                memory_manager.deallocate(process_name);
            } else {
                std::cout << "Uso: L <proceso>\n";
            }
        }
        else if (command == "M") {
            memory_manager.show_memory();
        }
        else if (command == "S") {
            memory_manager.show_statistics();
        }
        else if (command == "F") {
            std::string filename;
            if (iss >> filename) {
                execute_from_file(filename);
            } else {
                // Si no se proporciona archivo, usar selección dinámica
                std::cout << "Seleccionando archivo dinámicamente...\n";
                std::string selected_file = select_test_file();
                if (!selected_file.empty()) {
                    execute_from_file(selected_file);
                } else {
                    std::cout << "No se seleccionó ningún archivo.\n";
                }
            }
        }
        else if (command == "Q") {
            std::cout << "Saliendo del simulador...\n";
            return false;
        }
        else {
            std::cout << "Comando no reconocido: " << command << "\n";
        }
        
        return true;
    }
    
    void execute_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: No se pudo abrir el archivo '" << filename << "'\n";
            return;
        }
        
        std::string line;
        int line_number = 0;
        std::cout << "Ejecutando comandos desde '" << filename << "'...\n\n";
        
        while (std::getline(file, line)) {
            line_number++;
            
            // Ignorar líneas vacías y comentarios
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            std::cout << "[Linea " << line_number << "] " << line << "\n";
            process_command(line);
            std::cout << "\n"; // Espacio para mejor legibilidad
        }
        
        std::cout << "=== Ejecucion del archivo completada ===\n";
        file.close();
    }
};

int main() {
    std::cout << "=== SIMULADOR DE GESTION DE MEMORIA ===\n";
    std::cout << "Configuracion inicial\n\n";
    
    int memory_size;
    std::cout << "Ingrese el tamano total de memoria (minimo 100): ";
    std::cin >> memory_size;
    std::cin.ignore(); // Limpiar el buffer
    
    if (memory_size < 100) {
        std::cout << "Tamano minimo es 100. Usando 100 unidades.\n";
        memory_size = 100;
    }
    
    std::cout << "Memoria configurada: " << memory_size << " unidades\n\n";
    
    MemorySimulator simulator(memory_size);
    simulator.run();
    
    return 0;
}