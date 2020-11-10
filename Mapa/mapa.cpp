#include "mapa.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

map<string, vector<string>> enrutador::getCamino()
{
    return camino;
}

void enrutador::setNombre(string name)
{
    nombre = name;
}

void enrutador::setConexiones(map<string, int> conex)
{
    conexion = conex;
}

void enrutador::setCamino(map<string, vector<string>> cam)
{
    camino = cam;
}

string enrutador::getNombre()
{
    return nombre;
}

map<string, int> enrutador::getConexiones()
{
    return conexion;
}


//FUNCIONES DE LA RED**

void mapa::setEnrutador(enrutador enru)
{
    enrutadores = enru;

}

void mapa::setTabla_EN()
{
    tabla_EN.push_back(enrutadores);
}

void mapa::setClearTabla(string nombre)
{
    for (int unsigned i = 0; i < tabla_EN.size(); i++){
        if (tabla_EN[i].getNombre() == nombre)
            tabla_EN.erase(tabla_EN.begin()+i);
    }
}

void mapa::setClearConex(enrutador enru)
{
    enrutadores = enru;
    for(int unsigned i = 0; i < tabla_EN.size(); i++){
        tabla_EN[i];
    }
}

vector<enrutador> mapa::getTabla_EN()
{
    return tabla_EN;
}
