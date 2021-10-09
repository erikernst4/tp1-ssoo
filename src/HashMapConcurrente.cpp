#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
        // Arrancamos los semaforos desbloqueados
        _semaforos[i] = new mutex();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    unsigned int idx = hashIndex(clave);
    _semaforos[idx].lock();

    ListaAtomica<hashMapPair>::iterator it_clave = tabla[i].buscar(clave);
    if (it_clave == tabla[i].end()){
        hashMapPair nuevoPar = make_pair(clave, 1);
        tabla[i].insertar(nuevoPar);
    } else {
        it_clave*.second()++;
    }

    _semaforos[idx].unlock();
}

std::vector<std::string> HashMapConcurrente::claves() {
    std::vector<std::string> res;
    for (int i = 0; i < tabla.size(); i++){
        for (ListaAtomica<hashMapPair>::iterator it_actual = tabla[i].begin(); i != tabla[i].end(); it_actual++){
            res.push_back(it_actual*.first());
        }
    }
    return res;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
}

#endif
