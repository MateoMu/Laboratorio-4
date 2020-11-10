#ifndef MAPA_H
#define MAPA_H
#include <string>
#include <map>
#include <vector>

using namespace std;

class enrutador{

private:
    string nombre;
    map<string, int> conexion;
    map<string, vector<string>> camino;

public:
    void setNombre (string);
    void setConexiones (map<string,int>);
    void setCamino (map<string,vector<string>>);
    string getNombre ();
    map<string, int> getConexiones();
    map<string, vector<string>> getCamino();
};

class mapa{

private:
    enrutador enrutadores;
    vector<enrutador> tabla_EN;

public:

    void setEnrutador (enrutador);
    void setTabla_EN  ();
    void setClearTabla (string);
    void setClearConex (enrutador);
    vector<enrutador> getTabla_EN();
};



#endif // MAPA_H
