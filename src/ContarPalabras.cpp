#include <iostream>

#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"
#include <chrono>


int main(int argc, char **argv) {
    if (argc < 4) {
        std::cout << "Error: faltan argumentos." << std::endl;
        std::cout << std::endl;
        std::cout << "Modo de uso: " << argv[0] << " <threads_lectura> <hacerMaximo> <threads_maximo>" << std::endl;
        std::cout << "    " << "<archivo1> [<archivo2>...]" << std::endl;
        std::cout << std::endl;
        std::cout << "    threads_lectura: "
            << "Cantidad de threads a usar para leer archivos." << std::endl;
        std::cout << "    hacerMaximo: "
            << "'true' si requiere hacer el maximo" << std::endl;
        std::cout << "    threads_maximo: "
            << "Cantidad de threads a usar para computar máximo. No indicar de no ser necesario" << std::endl;
        std::cout << "    archivo1, archivo2...: "
            << "Archivos a procesar." << std::endl;
        return 1;
    }
    bool hacermaximo = argv[2] == "true";

    int cantThreadsLectura = std::stoi(argv[1]);
    int cantThreadsMaximo;
    int comienzo;
    if(hacermaximo){
        cantThreadsMaximo =  std::stoi(argv[3]);
        comienzo = 4;
    }else{
        comienzo = 3;
    }
    std::vector<std::string> filePaths = {};
    for (int i = comienzo; i < argc; i++) {
        filePaths.push_back(argv[i]);
    }

    HashMapConcurrente hashMap{}; // = HashMapConcurrente();
    auto start_multiples_archivos = std::chrono::steady_clock::now();
    cargarMultiplesArchivos(hashMap, cantThreadsLectura, filePaths);
    auto end_multiples_archivos = std::chrono::steady_clock::now();
    auto tiempo_multiples_archivos = start_multiples_archivos - end_multiples_archivos;
    std::cout << tiempo_multiples_archivos.count() << std::endl;
    if(hacermaximo){
        auto start_maximo_paralelo = std::chrono::steady_clock::now();
        auto maximo = hashMap.maximoParalelo(cantThreadsMaximo);
        auto end_maximo_paralelo = std::chrono::steady_clock::now();
        auto tiempo_maximo_paralelo = start_maximo_paralelo - end_maximo_paralelo;
        std::cout << tiempo_maximo_paralelo.count() << std::endl;
    }

    return 0;
}
