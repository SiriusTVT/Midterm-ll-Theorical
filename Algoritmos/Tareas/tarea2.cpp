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

// Enumeración para los algoritmos de asignación
enum class AllocationAlgorithm {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

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
    std::map<std::string, int> process_locations;
    AllocationAlgorithm algorithm;
    
    // Algoritmo First Fit
    bool allocate_first_fit(const std::string& process_name, int size) {
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); ++it) {
            if (it->is_free && it->size >= size) {
                allocate_block(it, process_name, size);
                return true;
            }
        }
        return false;
    }
    
    // Algoritmo Best Fit - encuentra el bloque libre más pequeño que quepa
    bool allocate_best_fit(const std::string& process_name, int size) {
        auto best_it = memory_blocks.end();
        int best_size = INT_MAX;
        
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); ++it) {
            if (it->is_free && it->size >= size && it->size < best_size) {
                best_it = it;
                best_size = it->size;
            }
        }
        
        if (best_it != memory_blocks.end()) {
            allocate_block(best_it, process_name, size);
            return true;
        }
        return false;
    }
    
    // Algoritmo Worst Fit - encuentra el bloque libre más grande
    bool allocate_worst_fit(const std::string& process_name, int size) {
        auto worst_it = memory_blocks.end();
        int worst_size = -1;
        
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); ++it) {
            if (it->is_free && it->size >= size && it->size > worst_size) {
                worst_it = it;
                worst_size = it->size;
            }
        }
        
        if (worst_it != memory_blocks.end()) {
            allocate_block(worst_it, process_name, size);
            return true;
        }
        return false;
    }
    
    // Función auxiliar para realizar la asignación en el bloque seleccionado
    void allocate_block(std::vector<MemoryBlock>::iterator it, const std::string& process_name, int size) {
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
    }
    
public:
    MemoryManager(int size = 100, AllocationAlgorithm alg = AllocationAlgorithm::FIRST_FIT) 
        : total_memory(size), algorithm(alg) {
        // Inicialmente toda la memoria está libre
        memory_blocks.push_back(MemoryBlock(0, total_memory, "", true));
    }
    
    // Método para cambiar el algoritmo
    void set_algorithm(AllocationAlgorithm alg) {
        algorithm = alg;
    }
    
    // Obtener nombre del algoritmo actual
    std::string get_algorithm_name() const {
        switch (algorithm) {
            case AllocationAlgorithm::FIRST_FIT: return "First Fit";
            case AllocationAlgorithm::BEST_FIT: return "Best Fit";
            case AllocationAlgorithm::WORST_FIT: return "Worst Fit";
            default: return "Desconocido";
        }
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
        
        bool success = false;
        
        // Aplicar el algoritmo seleccionado
        switch (algorithm) {
            case AllocationAlgorithm::FIRST_FIT:
                success = allocate_first_fit(process_name, size);
                break;
            case AllocationAlgorithm::BEST_FIT:
                success = allocate_best_fit(process_name, size);
                break;
            case AllocationAlgorithm::WORST_FIT:
                success = allocate_worst_fit(process_name, size);
                break;
        }
        
        if (success) {
            std::cout << "Memoria asignada al proceso '" << process_name 
                     << "' - Tamano: " << size << " unidades (Algoritmo: " 
                     << get_algorithm_name() << ")\n";
        } else {
            std::cout << "Error: No hay suficiente memoria contigua disponible para el proceso '" 
                     << process_name << "'\n";
        }
        
        return success;
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
    
    // Método para mostrar el estado de la memoria (comando M) - Formato del ejemplo
    void show_memory() {
        // Ordenar bloques por posición
        std::sort(memory_blocks.begin(), memory_blocks.end(), 
                 [](const MemoryBlock& a, const MemoryBlock& b) {
                     return a.start < b.start;
                 });
        
        std::cout << "[";
        bool first = true;
        
        for (const auto& block : memory_blocks) {
            if (!first) {
                std::cout << "][";
            }
            first = false;
            
            if (block.is_free) {
                std::cout << "Libre: " << block.size;
            } else {
                std::cout << block.process_name << ": " << block.size;
            }
        }
        std::cout << "]\n";
    }
    
    // Método para mostrar estado detallado de la memoria
    void show_detailed_memory() {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "MAPA DETALLADO DE MEMORIA (Total: " << total_memory << " unidades)\n";
        std::cout << "Algoritmo: " << get_algorithm_name() << "\n";
        std::cout << std::string(60, '=') << "\n";
        
        // Ordenar bloques por posición
        std::sort(memory_blocks.begin(), memory_blocks.end(), 
                 [](const MemoryBlock& a, const MemoryBlock& b) {
                     return a.start < b.start;
                 });
        
        std::cout << std::left << std::setw(8) << "Inicio" 
                 << std::setw(8) << "Tamano" 
                 << std::setw(12) << "Estado" 
                 << "Proceso\n";
        std::cout << std::string(60, '-') << "\n";
        
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
        std::cout << std::string(60, '=') << "\n\n";
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
        
        std::cout << "Estadisticas de memoria (Algoritmo: " << get_algorithm_name() << "):\n";
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
    MemorySimulator(int memory_size, AllocationAlgorithm algorithm) 
        : memory_manager(memory_size, algorithm) {}
    
    void run() {
        std::cout << "=== SIMULADOR DE GESTION DE MEMORIA - TAREA 2 ===\n";
        std::cout << "Algoritmo seleccionado: " << memory_manager.get_algorithm_name() << "\n";
        std::cout << "Comandos disponibles:\n";
        std::cout << "  A <proceso> <tamano>  - Asignar memoria\n";
        std::cout << "  L <proceso>           - Liberar memoria\n";
        std::cout << "  M                     - Mostrar estado (formato simple)\n";
        std::cout << "  D                     - Mostrar estado detallado\n";
        std::cout << "  S                     - Mostrar estadisticas\n";
        std::cout << "  F [archivo]           - Ejecutar comandos desde archivo (selección dinámica si no se especifica)\n";
        std::cout << "  ALG <1|2|3>           - Cambiar algoritmo (1=First, 2=Best, 3=Worst)\n";
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
        else if (command == "D") {
            memory_manager.show_detailed_memory();
        }
        else if (command == "S") {
            memory_manager.show_statistics();
        }
        else if (command == "ALG") {
            int alg_num;
            if (iss >> alg_num) {
                AllocationAlgorithm new_alg;
                switch (alg_num) {
                    case 1: new_alg = AllocationAlgorithm::FIRST_FIT; break;
                    case 2: new_alg = AllocationAlgorithm::BEST_FIT; break;
                    case 3: new_alg = AllocationAlgorithm::WORST_FIT; break;
                    default:
                        std::cout << "Algoritmo invalido. Use 1=First Fit, 2=Best Fit, 3=Worst Fit\n";
                        return true;
                }
                memory_manager.set_algorithm(new_alg);
                std::cout << "Algoritmo cambiado a: " << memory_manager.get_algorithm_name() << "\n";
            } else {
                std::cout << "Uso: ALG <1|2|3> (1=First Fit, 2=Best Fit, 3=Worst Fit)\n";
            }
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
            std::cout << "\n";
        }
        
        std::cout << "=== Ejecucion del archivo completada ===\n";
        file.close();
    }
};

// Función para mostrar ayuda de uso
void show_usage(const char* program_name) {
    std::cout << "Uso: " << program_name << " [tamano_memoria] [algoritmo] [archivo_entrada]\n";
    std::cout << "\nParametros (todos opcionales para modo interactivo):\n";
    std::cout << "  tamano_memoria  : Tamaño total de memoria (minimo 100)\n";
    std::cout << "  algoritmo       : 1=First Fit, 2=Best Fit, 3=Worst Fit\n";
    std::cout << "  archivo_entrada : (Opcional) Archivo con comandos a ejecutar\n";
    std::cout << "\nModos de uso:\n";
    std::cout << "  " << program_name << "                       # Modo interactivo con selección dinámica\n";
    std::cout << "  " << program_name << " 200 1              # 200 unidades, First Fit, modo interactivo\n";
    std::cout << "  " << program_name << " 150 2 comandos.txt # 150 unidades, Best Fit, desde archivo\n";
}

int main(int argc, char* argv[]) {
    std::cout << "=== SIMULADOR DE GESTION DE MEMORIA - TAREA 2 ===\n\n";
    
    // Modo interactivo para selección de parámetros si no se proporcionan argumentos suficientes
    if (argc < 3) {
        std::cout << "Modo interactivo de configuración:\n\n";
        
        int memory_size;
        std::cout << "Ingrese el tamaño de memoria (mínimo 100): ";
        std::cin >> memory_size;
        std::cin.ignore(); // Limpiar buffer
        
        if (memory_size < 100) {
            std::cout << "Tamaño mínimo es 100. Usando 100 unidades.\n";
            memory_size = 100;
        }
        
        int algorithm_num;
        std::cout << "Seleccione algoritmo de asignación:\n";
        std::cout << "1. First Fit\n";
        std::cout << "2. Best Fit\n";
        std::cout << "3. Worst Fit\n";
        std::cout << "Opción (1-3): ";
        std::cin >> algorithm_num;
        std::cin.ignore(); // Limpiar buffer
        
        AllocationAlgorithm algorithm;
        switch (algorithm_num) {
            case 1: algorithm = AllocationAlgorithm::FIRST_FIT; break;
            case 2: algorithm = AllocationAlgorithm::BEST_FIT; break;
            case 3: algorithm = AllocationAlgorithm::WORST_FIT; break;
            default:
                std::cout << "Algoritmo no válido. Usando First Fit por defecto.\n";
                algorithm = AllocationAlgorithm::FIRST_FIT;
                break;
        }
        
        std::cout << "\nDesea cargar comandos desde un archivo? (s/n): ";
        std::string load_file;
        std::getline(std::cin, load_file);
        
        std::string input_file;
        if (load_file == "s" || load_file == "S" || load_file == "si" || load_file == "SI") {
            input_file = select_test_file();
        }
        
        std::cout << "\nConfiguracion:\n";
        std::cout << "- Memoria: " << memory_size << " unidades\n";
        
        switch (algorithm) {
            case AllocationAlgorithm::FIRST_FIT: std::cout << "- Algoritmo: First Fit\n"; break;
            case AllocationAlgorithm::BEST_FIT: std::cout << "- Algoritmo: Best Fit\n"; break;
            case AllocationAlgorithm::WORST_FIT: std::cout << "- Algoritmo: Worst Fit\n"; break;
        }
        
        if (!input_file.empty()) {
            std::cout << "- Archivo de entrada: " << input_file << "\n";
        }
        std::cout << "\n";
        
        // Crear simulador
        MemorySimulator simulator(memory_size, algorithm);
        
        // Si hay archivo de entrada, ejecutarlo primero
        if (!input_file.empty()) {
            std::ifstream file(input_file);
            if (!file.is_open()) {
                std::cout << "Error: No se pudo abrir el archivo '" << input_file << "'\n";
                std::cout << "Continuando en modo interactivo...\n\n";
            } else {
                std::string line;
                int line_number = 0;
                std::cout << "Ejecutando comandos desde '" << input_file << "'...\n\n";
                
                while (std::getline(file, line)) {
                    line_number++;
                    
                    // Ignorar líneas vacías y comentarios
                    if (line.empty() || line[0] == '#') {
                        continue;
                    }
                    
                    std::cout << "[Linea " << line_number << "] " << line << "\n";
                    simulator.process_command(line);
                    std::cout << "\n";
                }
                
                file.close();
                std::cout << "=== Ejecucion del archivo completada ===\n\n";
                std::cout << "Continuando en modo interactivo...\n\n";
            }
        }
        
        // Ejecutar modo interactivo
        simulator.run();
        
        return 0;
    }
    
    // Modo tradicional con argumentos de línea de comandos
    if (argc > 4) {
        show_usage(argv[0]);
        return 1;
    }
    
    // Parsear argumentos
    int memory_size = std::atoi(argv[1]);
    int algorithm_num = std::atoi(argv[2]);
    std::string input_file = (argc == 4) ? argv[3] : "";
    
    // Validar tamaño de memoria
    if (memory_size < 100) {
        std::cout << "Error: Tamaño de memoria debe ser al menos 100 unidades\n";
        return 1;
    }
    
    // Validar y configurar algoritmo
    AllocationAlgorithm algorithm;
    switch (algorithm_num) {
        case 1: algorithm = AllocationAlgorithm::FIRST_FIT; break;
        case 2: algorithm = AllocationAlgorithm::BEST_FIT; break;
        case 3: algorithm = AllocationAlgorithm::WORST_FIT; break;
        default:
            std::cout << "Error: Algoritmo debe ser 1, 2 o 3\n";
            show_usage(argv[0]);
            return 1;
    }
    
    std::cout << "Configuracion:\n";
    std::cout << "- Memoria: " << memory_size << " unidades\n";
    
    switch (algorithm) {
        case AllocationAlgorithm::FIRST_FIT: std::cout << "- Algoritmo: First Fit\n"; break;
        case AllocationAlgorithm::BEST_FIT: std::cout << "- Algoritmo: Best Fit\n"; break;
        case AllocationAlgorithm::WORST_FIT: std::cout << "- Algoritmo: Worst Fit\n"; break;
    }
    
    if (!input_file.empty()) {
        std::cout << "- Archivo de entrada: " << input_file << "\n";
    }
    std::cout << "\n";
    
    // Crear simulador
    MemorySimulator simulator(memory_size, algorithm);
    
    // Si hay archivo de entrada, ejecutarlo primero
    if (!input_file.empty()) {
        std::ifstream file(input_file);
        if (!file.is_open()) {
            std::cout << "Error: No se pudo abrir el archivo '" << input_file << "'\n";
            return 1;
        }
        
        std::string line;
        int line_number = 0;
        std::cout << "Ejecutando comandos desde '" << input_file << "'...\n\n";
        
        while (std::getline(file, line)) {
            line_number++;
            
            // Ignorar líneas vacías y comentarios
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            std::cout << "[Linea " << line_number << "] " << line << "\n";
            simulator.process_command(line);
            std::cout << "\n";
        }
        
        file.close();
        std::cout << "=== Ejecucion del archivo completada ===\n\n";
        std::cout << "Continuando en modo interactivo...\n\n";
    }
    
    // Ejecutar modo interactivo
    simulator.run();
    
    return 0;
}