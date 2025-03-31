#ifndef MEM_BASE_H
#define MEM_BASE_H

#include <string>
#include <iostream>
#include <cmath>
#include "cache.hpp"

using namespace std;

class Memoria
{
protected:

    string BinAdrss,Etiqueta,Indice,HM,Word,type;
    int cantPalabras, tamanoBloque, viasMax,offsetPalabra,offsetBit,totalAcierto,totalDirecciones;
    bool curAcierto;

public:
    Memoria();
    void init(int tam, int palabras = 1, int vias = 1);
    ~Memoria();

    void imprimir();
    void restartText();
    void start();
    bool acierto(int index);
    string convertBinary(unsigned int num);
    void updateTable(int entrada, int etiqueta, int indice, int palabra);
    void spliceData(int entrada,int &etiqueta, int &indice, int &palabra);
    void readOne(int entrada);
    void prefetch();

    int getTotalAciertos();
    int getTotalDirecciones();
    bool getCurAcierto();
};

Memoria::Memoria()
{
}

void Memoria::init(int tam, int palabras, int vias)
{
    tamanoBloque = tam;
    cantPalabras = palabras;
    viasMax = vias;
    offsetPalabra = log2(cantPalabras);
    offsetBit = log2(tamanoBloque);
    totalAcierto = 0;
    totalDirecciones = 0;
    curAcierto = false;
    this->restartText();
}

Memoria::~Memoria()
{
}

void Memoria::restartText(){
    this->BinAdrss = "Binary Adress: ";
    this->Etiqueta = "Etiquetas: ";
    this->Indice = "Indice: ";
    this->HM = "Hit/Miss: ";
    this->Word = "Palabra: ";
}

void Memoria::start(){
    return;
}

void Memoria::readOne(int entrada){
    return;
}

bool Memoria::acierto(int index){
    return false;
}

void Memoria::imprimir(){
    cout << type << "\n" << BinAdrss << endl << Etiqueta << '\n' << Indice << '\n' << Word << '\n' << HM << endl;
    this->restartText();
}

string Memoria::convertBinary(unsigned int num){
    string binary;
    while (num > 0) {
        int bit = num & 1;
        binary = (bit == 0 ? '0' : '1') + binary;
        num >>= 1;
    }
    binary = binary.size() <= 0 ? "0" : binary;
    return binary;
}

void Memoria::updateTable(int entrada, int etiqueta, int indice, int palabra){

    this->BinAdrss += convertBinary(entrada) + ", ";
    this->Etiqueta += convertBinary(etiqueta) + ", ";
    this->Indice += convertBinary(indice) + ", ";
    this->Word += convertBinary(palabra) + ", ";
}

void Memoria::spliceData(int entrada,int &etiqueta, int &indice, int &palabra){
    etiqueta = entrada >> this->offsetBit + this->offsetPalabra;
    indice =  (entrada >> this->offsetPalabra) % this->tamanoBloque;
    palabra = entrada % this->cantPalabras;
}

void Memoria::prefetch(){
    return;
}

int Memoria::getTotalAciertos(){
    return totalAcierto;
}
int Memoria::getTotalDirecciones(){
    return totalDirecciones;
}

bool Memoria::getCurAcierto(){
    return curAcierto;
}
#endif