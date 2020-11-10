
//Bibliotecas

#include "mapa.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

mapa primer_camino (mapa);
map<int, vector<string>> camino_corto (string, string, vector<enrutador>, vector<string>, int*, map<int, vector<string>>);
map<int, vector<string>> calcular_camino (map<int, vector<string>>);
bool no_conexion(map<int, vector<string>>);

int main(){
    ifstream fin;
    fin.open("red.txt");
    int costo = 0;
    string nombre, nombre_router, linea, temp;
    map<string,int> tabla_temp;
    enrutador enruta_temp;
    mapa topologia;
    //Abrir el archivo de texto, lectura de la red
    /***Se define un prototipo de red en texto el cual será explicado en la sustentación.***/
    while(fin.good()){
        getline(fin,linea);
        size_t pos=linea.find(";");    //Toma por determinado que la posición entre el inicio y ";" es el nombre del router
        nombre=linea.substr(0,pos);
        enruta_temp.setNombre(nombre);   //Define el nombre del enrutador

        for (int unsigned i=pos+1;i<linea.size();i++){//Toma el nombre de cada una de sus conexiones tras encontrar una ","

            if (linea[i]!=','){
                temp+=linea[i];
            }
            else{
                size_t pos=temp.find(":");                //Nombre y cada uno de los Costo de las conexiones
                nombre_router=temp.substr(0,pos);             //Almacena el nombre
                costo=atoi(temp.substr(pos+1).c_str());       //Convierte de string a entero.
                tabla_temp.insert(make_pair(nombre_router,costo)); //Ingresa el nombre de la conexion y su costo.
                temp="";
            }
        }
        enruta_temp.setConexiones(tabla_temp);       //Define las conexiones del enrutador
        topologia.setEnrutador(enruta_temp);
        topologia.setTabla_EN();                     //Crea un vector con cada una de las redes
        tabla_temp.clear();                          //Reinicia la variable auxiliar.
    }
    mapa topologia_final;
    topologia_final=primer_camino(topologia);      //Define la red con costos optimos.

    map<string,int>::iterator iter;

    vector<enrutador> tabla_E=topologia_final.getTabla_EN();    //Obtener tabla de enrutamiento.
    enrutador temp2;                                            //Guardar enrutadores guardados en tabla_E.
    map<string,int> conex2;                                    //Conexiones de cada enrutador.

    //Menú.
    int opcion=0;
    char aux;
    cout<<"BIENVENIDO A SU RED! "<<endl<<endl<<"Escoja una opcion: "<<endl;
    cout<<endl;
    while (true) {

    cout<<"1. Mostrar red original."<<endl<<"2. Mostrar red con conexiones más bajas."<<endl<<"3. Agregar enrutador de la red."<<endl<<"4. Remover enrutador de la red."<<endl<<"5. Costo de envio de un router a otro."<<endl;
    cout<<"6. Salir"<<endl;
    cout<<"Opcion: "; cin>>opcion;
    cout<<opcion;
    cout<<endl;

    //***Red inicial***   -> Muestra la red sin sin los costos más bajos. Definida originalmente
    if (opcion==1){
        tabla_E=topologia.getTabla_EN();
        for (int unsigned i=0;i<tabla_E.size();i++){         //Enrutadores de la red.
            temp2=tabla_E[i];                                     //Cada enrutador de la red.
            conex2=temp2.getConexiones();                         //Conexiones de ese enrutador.

            //Imprime la red original con sus conexiones y costos definidos.
            for (iter=conex2.begin();iter!=conex2.end();iter++){
                cout<<"Router: "<<temp2.getNombre()<<" conexion: "<<iter->first<<" costo: "<<iter->second<<endl;
            }
            cout<<endl;
        }
        cout<<"Presione cualquier tecla, seguida de enter para continuar: "; cin>>aux;
    }

    //***Red final***    -> Muestra la red con los costos más bajos de cada nodo a cada uno de los routers.
    if (opcion==2){
        tabla_E=topologia_final.getTabla_EN();
        for (int unsigned i=0;i<tabla_E.size();i++){              //Enrutadores de la red.
            temp2=tabla_E[i];                                     //Cada enrutador de la red.
            conex2=temp2.getConexiones();                         //Conexiones de ese enrutador.

            //Mostrar en pantalla.
            for (iter=conex2.begin();iter!=conex2.end();iter++){
                cout<<"Router: "<<temp2.getNombre()<<" conexion: "<<iter->first<<" costo: "<<iter->second<<endl;
            }
            cout<<endl;
        }
        cout<<"Presione cualquier tecla, seguida de enter para continuar: ";cin>>aux;
    }

    //***Agregar router***  -> Se define un nuevo enrutador y se conecta a la red
    /*OBSERVACION: Sólo se puede conectar con un nodo.*/
    if (opcion==3){
        map<string, int> temp;
        fflush(stdin);
        cout<<"Ingrese el nombre del router que quieres agregar: "; getline(cin,nombre);
        enruta_temp.setNombre(nombre);                              //Crea el nuevo router.

        cout<<"El router "<<nombre<<" solo tendra un enlace: "<<endl;
        cout<<"Conexiones posibles: "<<endl;                //Imprime las opciones de conexión.
        for (int unsigned i=0;i<tabla_E.size();i++){
            temp2=tabla_E[i];
            cout<<"-"<<temp2.getNombre()<<endl;
        }

        fflush(stdin);
        cout<<"Ingrese el nombre del router de conexion: "; getline(cin,nombre_router);
        cout<<"Ingrese el costo: "; cin>>costo;
        cout<<endl;

        temp.insert(make_pair(nombre_router,costo));        //Router nuevo le asgina su conexión.
        enruta_temp.setConexiones(temp);                    //Le asigna la conexión
        topologia.setEnrutador(enruta_temp);                //Ingresa a la red.
        topologia.setTabla_EN();                            //Se ingresa a la tabla de la red.
        tabla_E = topologia.getTabla_EN();

        //Conectar con el router destino
        for (int unsigned i=0;i<tabla_E.size();i++){
            enrutador enruta_temp_2;
            enruta_temp_2=tabla_E[i];
            if (enruta_temp_2.getNombre()==nombre_router){
                topologia.setClearTabla(nombre_router);     //Borra el router de llegada de la red.
                temp = enruta_temp_2.getConexiones();       //Se obtiene las conexiones del router de llegada.
                temp.insert(make_pair(nombre,costo));       //Se le agrega la nueva conexión.
                enruta_temp_2.setConexiones(temp);          //Se le agrega al router.
                topologia.setEnrutador(enruta_temp_2);      //Se actualiza en la red oficial.
                topologia.setTabla_EN();
            }
        }
        cout<<"Su red se ha actualizado con exito!"<<endl;
        topologia_final = primer_camino(topologia);         //Se actualiza con los caminos mas cortos junto la nueva conexión.
        cout<<"Presione cualquier tecla, seguida de enter para continuar: "; cin>>aux;
    }


    //***Remover router***  ->  Se elimina un router
    /*Observaciones: A la hora de eliminar el router, también es necesario eliminarlo
                     de cada una de las conexiones que tenía, por lo cual también se debe eliminar
                     de toda la tabla.*/
    if (opcion==4){
        string eliminar;
        tabla_E=topologia.getTabla_EN();
        cout<<"Estos son los routers de la red, al final ingrese el nombre del que desea eliminar: "<<endl;
        for (int unsigned i=0;i<tabla_E.size();i++){
            temp2=tabla_E[i];
            cout<<"-"<<temp2.getNombre()<<endl;
        }
        fflush(stdin);
        cout<<"Ingrese el nombre del router: "; getline(cin,eliminar);
        for (int unsigned i=0;i<tabla_E.size();i++){
            enrutador enruta_temp_2;
            enruta_temp_2=tabla_E[i];
            if (enruta_temp_2.getNombre()==eliminar){
                topologia.setClearTabla(eliminar);
            }
            else{
                conex2=enruta_temp_2.getConexiones();
                conex2.erase(eliminar);
                topologia.setClearTabla(enruta_temp_2.getNombre()); //Borra el router de llegada de la red.
                enruta_temp_2.setConexiones(conex2);                //Se le agrega al router.
                topologia.setEnrutador(enruta_temp_2);              //Se actualiza en la red oficial.
                topologia.setTabla_EN();
            }
            }
        cout<<"Su red se ha actualizado con exito!"<<endl;
        topologia_final = primer_camino(topologia);
        cout<<"Presione cualquier tecla, seguida de enter para continuar: "; cin>>aux;
    }

    //Costo y camino

    if (opcion==5){
        string inicio,final;                       //punto de inicio y destino
        tabla_E=topologia_final.getTabla_EN();     /*Se carga la topología con los routers
                                                       y sus costos*/
        cout<<"Routers de la red: "<<endl;                    //Muestra los routers
        for (int unsigned i = 0; i < tabla_E.size(); i++){
            temp2 = tabla_E[i];
            cout<<"-"<<temp2.getNombre()<<endl;
        }
        fflush(stdin);
        cout<<"Ingrese el nombre del router de inicio para el envio del paquete: "; getline(cin, inicio);
        cout<<"Ingrese el nombre del router del final: "; getline(cin, final);
        for (int unsigned i=0;i<tabla_E.size();i++){
            enruta_temp=tabla_E[i];
            if (enruta_temp.getNombre()==inicio){
                conex2=enruta_temp.getConexiones();
                map<string,int>::iterator iter=conex2.find(final);
                if (iter!=conex2.end()){
                    cout<<"El costo del envio del paquete por el camino mas corto"<<endl<<"desde "<<inicio<<" hasta "<<final<<" es "<<iter->second<<endl;
                }
                map<string,vector<string>> camino;
                camino=enruta_temp.getCamino();
                vector<string> camino_2;
                map<string,vector<string>>::iterator iter_2=camino.find(final);
                if (iter_2!=camino.end()) camino_2=iter_2->second;
                cout<<"La ruta fue: "<<endl;
                for(int unsigned i=0;i<camino_2.size();i++){
                    cout<<"->"<<camino_2[i];
                }
                cout<<"->"<<final;
            }
        }
        cout<<endl;
        cout<<"Ingresa cualquier tecla y enter para continuar: "; cin>>aux;
        cout<<endl;
    }

    //Salir.
    if (opcion==6){
        break;
    }
    cout<<endl;
    }
}

mapa primer_camino(mapa topologia){
    //map<string,int>::iterator iter;
    enrutador enruta_temp;
    int costo=0;
    vector<enrutador> tabla_E=topologia.getTabla_EN();        //Obtener tabla de enrutamiento.
    enrutador temp2;                                            //Guardar enrutadores guardados en tabla_E.
    //map<string, int> conex2;                                  //Conexiones de cada enrutador.
    mapa topologia_final;
    vector<string> ruta;
    vector<string> nom_enrutadores;                             //Para conectar todos los routers.
    map<string, int> conexiones;
    map<int, vector<string>>calcular_cam;
    map<int, vector<string>>calcular_costo;
    map<string, vector<string>>caminos;                    //Caminos a todos los enrutadores.

    for (int unsigned i=0; i<tabla_E.size();i++){      //Capturar todos los routers en la red original.
        temp2=tabla_E[i];
        nom_enrutadores.push_back(temp2.getNombre());
    }

    bool conex;

    for (int unsigned i=0; i<nom_enrutadores.size();i++){      //Recorre los nodos.
        enruta_temp.setNombre(nom_enrutadores[i]);                  //Asigana la conexión con el mismo.
        conexiones.insert(make_pair(nom_enrutadores[i],0));         //Agrega su propia conexión.
        for(int unsigned y = 0; y < nom_enrutadores.size(); y++){   //Conecta el router con los demás.
            if (nom_enrutadores[i]==nom_enrutadores[y]){          //Si el mismo ya no se escoge.
            }
            else
            {
                ruta.push_back(nom_enrutadores[i]);                 //Se asigna el punto de partida.
                //Calcula todos los caminos posibles
                calcular_costo = camino_corto(nom_enrutadores[i], nom_enrutadores[y], tabla_E, ruta, &costo, calcular_costo);
                //Reinicia los valores
                ruta.clear();
                calcular_cam=calcular_camino(calcular_costo); //Calcular camino mas corto.
                conex=no_conexion(calcular_cam);              //Saber si tiene conexión con el nodo buscado.
                costo=0;
                calcular_costo.clear();

                map<int, vector<string>>::iterator iter=calcular_cam.begin();
                while (iter!=calcular_cam.end()) {
                    if (conex){
                        iter++;
                    }
                    else{
                        conexiones.insert(make_pair(nom_enrutadores[y],iter->first));
                        caminos.insert(make_pair(nom_enrutadores[y],iter->second));
                        iter ++;
                    }
                }
            }
        }
        enruta_temp.setCamino(caminos);
        enruta_temp.setConexiones(conexiones);
        conexiones.clear();
        topologia_final.setEnrutador(enruta_temp);
        topologia_final.setTabla_EN();
        caminos.clear();
    }
    return topologia_final;
}

//Recorrer los caminos
map<int, vector<string>> camino_corto(string inicial, string final, vector<enrutador> tabla, vector<string> ruta, int* costo, map<int, vector<string>> calcular_costo){
    enrutador temp;
    map<string, int> conexiones;
    int costo_ida=0,vuelta=1;
    string ruta_ida="";

    //Buscar las conexiones del inicial
    for (int unsigned i=0;i<tabla.size();i++){
        temp=tabla[i];
        if (temp.getNombre()==inicial){
            break;
        }
    }
    conexiones=temp.getConexiones();

    map<string,int>::iterator iter;
    bool flag=true;

    //Iteración sobre las conexiones
    for (iter=conexiones.begin();iter!=conexiones.end();iter++){
        costo_ida=iter->second;                       //Resta para devolverse en el costo.
        ruta_ida=iter->first;                         //Resta para devolverse en el camino.

        //Revisa si ya pasó por ese nodo, para no hacerlo nuevamente
        for (int unsigned m=0;m<ruta.size();m++){
            if (iter->first==ruta[m]){
                flag=false;
                vuelta=0;
                break;
            }
        }
        if(flag){
            *costo+=iter->second;                 //Suma el costo de la ruta que va hacer.
            if (iter->first==final){              //Indica si ya encontró el nodo de llegada.
                calcular_costo.insert(make_pair(*costo,ruta));     //Cuanto costó llegar y la ruta a llegar.
                vuelta=1;                         //Indica que ya se puede devolver para la resta mas abajo.
            }
            else{
                ruta.push_back(iter->first);        //Indica que el nodo elegido no es el final para ir a buscar en los nodos conectados a el.
                calcular_costo=camino_corto(iter->first,final,tabla,ruta,costo,calcular_costo);
            }
        }
        if(vuelta==1){                            //Indica que ya se va devolver.
            *costo-=costo_ida;                    //Resta lo que le costó llegar al nodo que ya revisó
            for (int unsigned i=0;i<ruta.size();i++){  //Resta la ruta que eligió para llegar.
                if(ruta[i]==ruta_ida)ruta.erase(ruta.begin()+i);
            }
        }
        vuelta=1;                                 //Indica que ya se puede devolver.
        flag=true;                                //Indica que el nodo a elegir puede ser un posible nodo.
    }
    return calcular_costo;
}

//Escoge el menor costo
map<int, vector<string>> calcular_camino(map<int,vector<string>>costo){
    map<int,vector<string>>::iterator iter;
    map<int,vector<string>> camino;
    vector<string> ruta;
    int temp=0,cont=0;
    for(iter=costo.begin();iter!=costo.end();iter++){     //Recorre las rutas que se le asignaron.
        if (cont==0){                                         //Solo entra la primer vez para comparar luego
            temp=iter->first;                                 //Guarda el valor.
            ruta=iter->second;                                //Guarda la ruta.
            cont++;                                             //Para no entrar otra vez.
        }
        else{
            if (iter->first > temp);        //Indica si el costo elegido es mayor que el anterior.
            else temp=iter->first,ruta=iter->second;   //Indica que el costo elegido es el menor.
        }
    }
    camino.insert(make_pair(temp,ruta));    //Guarda el menor costo con la ruta asignada.
    return camino;
}

//Determina si el router tiene conexiones
bool no_conexion (map<int, vector<string>> tabla){
    map<int, vector<string>>::iterator iter = tabla.begin();
    for(iter; iter != tabla.end(); iter++){
        if (iter->first == 0) return true;      //Si alguno es cero es porque no hay conexión con ese router.
    }
    return false;
}
