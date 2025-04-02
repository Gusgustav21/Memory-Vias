#include <iostream>
#include <sstream>

#include "memLibrary/memVias.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>
#include <ctime>
  
using namespace std;

void porVias(size_t size, char *addr);

bool fetchBoolean = false;
char manualAddress;

int main(int argc, const char* argv[])
{
    char *addr;
    int fd = open("ancona_data.csv", O_RDONLY);
    int direccion;
    double aciertos, total;

    if(fd == -1){
        cout << "Archivo no encontrado";
        return -1;
    }
    struct stat fileInfo = {0};
    if (fstat(fd, &fileInfo) == -1)
    {
        close(fd);
        cout << "Error obteniendo el tamaño";
        return -1;
    }
    addr = (char*)mmap(NULL,fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        close(fd);
        cout << "Error mmapping the file";
        return -1;
    }
    size_t size = fileInfo.st_size;
    string type = "-1";
    system("clear");
    
    porVias(size, addr);
    close(fd);
    return 0;
}

void porVias(size_t size,char *addr)
{
    system("clear");
    char curAddr;
    int vias1, vias2, vias3, bloques, palabras;
    double aciertos1, aciertos2, aciertos3, total1, total2, total3;

    vias1 = 2;
    vias2 = 4;
    vias3 = 8;
    bloques = 4;
    palabras = 1;

    clock_t t1, t2, t3;

    string config = "-1";
    while(!isdigit(config[0]) || stoi(config) < 0){
        cout << "Inserta el numero de bloques (escriba 0 para ir a la configuracion predeterminada)\n\n";
        cin >> config;
        if(!isdigit(config[0]) || stoi(config) < 0){
            system("clear");
            cout << "\nValor invalido, intente de nuevo\n\n";
        }
    }
    system("clear");
    if(stoi(config) != 0){
        bloques = stoi(config);
        config = "-1";
        while(!isdigit(config[0]) || stoi(config) < 0){
            cout << "Inserta el numero de palabras\n";
            cin >> config;
            if(!isdigit(config[0]) || stoi(config) < 0){
                system("clear");
                cout << "\nValor invalido, intente de nuevo\n\n";
            }
        }
        system("clear");
        palabras = stoi(config);
    }

    memVias memoria1(bloques,palabras,vias1),  memoria2(bloques,palabras,vias2), memoria3(bloques,palabras,vias3);

    ifstream entrada1("ancona_data.csv"), entrada2("ancona_data.csv"), entrada3("ancona_data.csv");
    if(!entrada1.is_open())
    {
        cout << "No se encuentra el archivo ancona_data.csv";
        return;
    };
    if(entrada1.good() && entrada1.is_open()){
        t1 = clock();
        while(!entrada1.eof()){
            if(!fetchBoolean){
                entrada1.get(manualAddress);
            }
            fetchBoolean = false;
            if(manualAddress != ','){
                memoria1.readOne((int)manualAddress);
                if(!memoria1.getCurAcierto())
                {
                    entrada1.get(manualAddress);
                    while(manualAddress == ','){
                        entrada1.get(manualAddress);
                    }
                    memoria1.prefetch((int)(manualAddress));
                    fetchBoolean = true;
                }
            }
        }
        t1 = clock() - t1;
    }

    entrada1.close();
    
    if(!entrada2.is_open())
    {
        cout << "No se encuentra el archivo ancona_data.csv";
        return;
    };
    if(entrada2.good() && entrada2.is_open()){
        t2 = clock();
        while(!entrada2.eof()){
            if(!fetchBoolean){
                entrada2.get(manualAddress);
            }
            fetchBoolean = false;
            if(manualAddress != ','){
                memoria2.readOne((int)manualAddress);
                if(!memoria2.getCurAcierto())
                {
                    entrada2.get(manualAddress);
                    while(manualAddress == ','){
                        entrada2.get(manualAddress);
                    }
                    memoria2.prefetch((int)(manualAddress));
                    fetchBoolean = true;
                }
            }
        }
        t2 = clock() - t2;
    }

    entrada2.close();
    
    if(!entrada3.is_open())
    {
        cout << "No se encuentra el archivo ancona_data.csv";
        return;
    };
    if(entrada3.good() && entrada3.is_open()){
        t3 = clock();
        while(!entrada3.eof()){
            if(!fetchBoolean){
                entrada3.get(manualAddress);
            }
            fetchBoolean = false;
            if(manualAddress != ','){
                memoria3.readOne((int)manualAddress);
                if(!memoria3.getCurAcierto())
                {
                    entrada3.get(manualAddress);
                    while(manualAddress == ','){
                        entrada3.get(manualAddress);
                    }
                    memoria3.prefetch((int)(manualAddress));
                    fetchBoolean = true;
                }
            }
        }
        t3 = clock() - t3;
    }
    entrada3.close();

    aciertos1 = memoria1.getTotalAciertos();
    total1 = memoria1.getTotalDirecciones();

    aciertos2 = memoria2.getTotalAciertos();
    total2 = memoria2.getTotalDirecciones();

    aciertos3 = memoria3.getTotalAciertos();
    total3 = memoria3.getTotalDirecciones();

    system("clear");
    cout << "Entrada estandar:\n\n";
    cout << "2 vías\n";
    cout << "╔═══════════════════════════════════════════════╗\n\n";
    cout << "  - Cantidad de bloques: "  << bloques << "\n";
    cout << "  - Total de palabras: " << palabras << "\n";
    cout << "  - Total de vias: " << vias1 << "\n";
    cout << "  - Total de aciertos: " << (unsigned long int)aciertos1 << "\n";
    cout << "  - Total de fallos : " << (unsigned long int)(total1 - aciertos1) << "\n";
    cout << "  - Total de direcciones: " << (unsigned long int)total1 << "\n";
    cout << "  - Tiempo de ejecución (segundos): " << ((float)t1)/CLOCKS_PER_SEC << "\n";
    cout << "  - El porcentaje de aciertos es de: " << float(aciertos1/total1)*100 << "%\n\n";
    cout << "╚═══════════════════════════════════════════════╝\n\n";

    cout << "4 vías\n";
    cout << "╔═══════════════════════════════════════════════╗\n\n";
    cout << "  - Cantidad de bloques: "  << bloques << "\n";
    cout << "  - Total de palabras: " << palabras << "\n";
    cout << "  - Total de vias: " << vias2 << "\n";
    cout << "  - Total de aciertos: " << (unsigned long int)aciertos2 << "\n";
    cout << "  - Total de fallos : " << (unsigned long int)(total2 - aciertos2) << "\n";
    cout << "  - Total de direcciones: " << (unsigned long int)total2 << "\n";
    cout << "  - Tiempo de ejecución (segundos): " << ((float)t2)/CLOCKS_PER_SEC << "\n";
    cout << "  - El porcentaje de aciertos es de: " << float(aciertos2/total2)*100 << "%\n\n";
    cout << "╚═══════════════════════════════════════════════╝\n\n";

    cout << "8 vías\n";
    cout << "╔═══════════════════════════════════════════════╗\n\n";
    cout << "  - Cantidad de bloques: "  << bloques << "\n";
    cout << "  - Total de palabras: " << palabras << "\n";
    cout << "  - Total de vias: " << vias3 << "\n";
    cout << "  - Total de aciertos: " << (unsigned long int)aciertos3 << "\n";
    cout << "  - Total de fallos : " << (unsigned long int)(total3 - aciertos3) << "\n";
    cout << "  - Total de direcciones: " << (unsigned long int)total3 << "\n";
    cout << "  - Tiempo de ejecución (segundos): " << ((float)t3)/CLOCKS_PER_SEC << "\n";
    cout << "  - El porcentaje de aciertos es de: " << float(aciertos3/total3)*100 << "%\n\n";
    cout << "╚═══════════════════════════════════════════════╝\n\n";
}

