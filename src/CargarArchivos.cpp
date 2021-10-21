#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
//#include <pthread.h>
#include <thread>

#include "CargarArchivos.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        // Completar (Ejercicio 4)
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}


void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    // Completar (Ejercicio 4)
    if(cantThreads > filePaths.size())throw("invalid input");

    std::thread* hilos[cantThreads];
    int proximoArchivo = 0;
    std::mutex semaforo();

    for(unsigned int i = 0; i < cantThreads; i++){
        std::thread t = std::thread(cargarMultiplesArchivosAux,hashMap,filePaths,&proximoArchivo,&semaforo);
        hilos[i] = &t;
    }

    for(unsigned int i = 0; i < cantThreads; i++){
        hilos[i]->join();        
    }
}

/*void cargarMultiplesArchivosAux(HashMapConcurrente &hashMap, std::vector<std::string>& filePaths, std::atomic<int>& proximoArchivo){
    int archivoActual = 0;
    while(proximoArchivo.load() < filePaths.size()){
        archivoActual = proximoArchivo.fetch_add(1);
        cargarArchivo(hashMap,filePaths[proximoArchivo]);
    }
}*/

void cargarMultiplesArchivosAux(HashMapConcurrente &hashMap, std::vector<std::string>& filePaths, int * proximoArchivo, std::mutex &semaforo){
    while(1){
        semaforo.lock();
        if(*proximoArchivo >= filePaths.size()){break;}
        (*proximoArchivo) ++;
        semaforo.unlock();
        cargarArchivo(hashMap,filePaths[*proximoArchivo]);
    }
}



#endif
