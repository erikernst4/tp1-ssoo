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
        this->_semaforos[index]->lock();
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }
    for(auto s:_semaforos){
        s->unlock();
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    if(cant_threads > cantLetras)throw("invalid input");

    std::thread* hilos[cant_threads];

    std::atomic<int> proximaFila(0);    
    hashMapPair maxFila[cantLetras];
    std::mutex sem();

    for(unsigned int i = 0; i < cant_threads; i++){
        std::thread t = std::thread(maximoAux, &proximaFila, maxFila, sem);
        hilos[i] = &t;
    }

    for(unsigned int i = 0; i < cant_threads; i++){
        hilos[i]->join();        
    }

    hashMapPair res = maxFila[0];
    for(int i = 0; i < cantLetras; i ++){
        if (maxFila[i].second > res.second) {
                res = maxFila[i];
        }
    }

    return res;

}
hashMapPair HashMapConcurrente::maximoAux(std::atomic<int>* proximaFila, hashMapPair maxFila[], std::mutex &semaforo){
    int filaActual(0);

    while(1){
        semaforo.lock();
        if(proximaFila->load() >= cantLetras) break;
        filaActual = proximaFila->fetch_add(1);
        semaforo.unlock();
        hashMapPair *max = new hashMapPair();
        max->second = 0;
        
        for (auto &p : *tabla[filaActual]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
        maxFila[filaActual] = *max;
    }

}


#endif
