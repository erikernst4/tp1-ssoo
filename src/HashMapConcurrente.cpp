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
        _semaforos[i] = new std::mutex();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    unsigned int idx = hashIndex(clave);
    _semaforos[idx]->lock();

    unsigned int valor = this->valor(clave);

    if (valor == 0){
        hashMapPair nuevoPar = make_pair(clave, 1);
        tabla[idx]->insertar(nuevoPar);
    } else {
        ListaAtomica<hashMapPair>::iterator it_clave = tabla[idx]->buscar(make_pair(clave,valor));
        *it_clave = make_pair(clave,valor+1);
    }

    _semaforos[idx]->unlock();
}

std::vector<std::string> HashMapConcurrente::claves() {
    std::vector<std::string> res(0);
    for (int i = 0; i < cantLetras; i++){
        for (ListaAtomica<hashMapPair>::iterator it_actual = tabla[i]->begin(); it_actual != tabla[i]->end(); it_actual++){
            res.push_back((*it_actual).first);
        }
    }
    return res;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    ListaAtomica<hashMapPair> * listaDicc = tabla[hashIndex(clave)];
    for (ListaAtomica<hashMapPair>::iterator it = listaDicc->begin(); it != listaDicc->end(); it++){
        if((*it).first == clave){
            return (*it).second;
        }
    }
    return 0;
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
