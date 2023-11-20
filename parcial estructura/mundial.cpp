#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue> 
#include <stack>
#include <algorithm>
#include <cstdlib>
#include <ctime>


using namespace std;

struct Equipo {
    string nombre;
    int habilidad;
    int partidosJugados;
    int partidosGanados;
    int partidosEmpatados;
    int partidosPerdidos;
    int golesAFavor;
    int golesEnContra;
    int diferenciaGol;
    int puntos;
};

struct Llave
{
    Equipo first;
    Equipo second;
};




map<string, Equipo> equipos;


void simularPartido(Equipo &equipo1, Equipo &equipo2)
{
    if (equipo1.partidosJugados >= 3 || equipo2.partidosJugados >= 3)
    {
        return;
    }

    int probabilidad_equipo1 = equipo1.habilidad;
    int probabilidad_equipo2 = equipo2.habilidad;
    probabilidad_equipo1 = std::min(probabilidad_equipo1, 100 - probabilidad_equipo2);
    probabilidad_equipo2 = std::min(probabilidad_equipo2, 100 - probabilidad_equipo1);
    probabilidad_equipo1 = std::max(probabilidad_equipo1, 1);
    probabilidad_equipo2 = std::max(probabilidad_equipo2, 1);

    int resultado = rand() % (probabilidad_equipo1 + probabilidad_equipo2);

    if (resultado < probabilidad_equipo1 && equipo1.golesAFavor < 6)
    {
        equipo1.golesAFavor++;
        equipo2.golesEnContra++;
        equipo1.partidosGanados++;
        equipo2.partidosPerdidos++;
        equipo1.puntos += 3;
    }
    else if (resultado < probabilidad_equipo1 + probabilidad_equipo2 && equipo2.golesAFavor < 6)
    {
        equipo2.golesAFavor++;
        equipo1.golesEnContra++;
        equipo2.partidosGanados++;
        equipo1.partidosPerdidos++;
        equipo2.puntos += 3;
    }
    else
    {
        
        if (equipo1.golesAFavor < 6 || equipo2.golesAFavor < 6)
        {
            int penalesEquipo1 = rand() % 5 + 1;
            int penalesEquipo2 = rand() % 5 + 1;

            int golesEquipo1 = std::min(penalesEquipo1, 6 - equipo1.golesAFavor);
            int golesEquipo2 = std::min(penalesEquipo2, 6 - equipo2.golesAFavor);

            if (penalesEquipo1 > penalesEquipo2)
            {
                equipo1.golesAFavor += golesEquipo1;
                equipo2.golesEnContra += golesEquipo1;
                equipo1.partidosGanados++;
                equipo2.partidosPerdidos++;
                equipo1.puntos += 3;
            }
            else if (penalesEquipo1 < penalesEquipo2)
            {
                equipo2.golesAFavor += golesEquipo2;
                equipo1.golesEnContra += golesEquipo2;
                equipo2.partidosGanados++;
                equipo1.partidosPerdidos++;
                equipo2.puntos += 3;
            }
            else
            {
                equipo1.partidosEmpatados++;
                equipo2.partidosEmpatados++;
                equipo1.puntos++;
                equipo2.puntos++;
            }
        }
    }
    equipo1.partidosJugados++;
    equipo2.partidosJugados++;

    

    equipo1.golesAFavor = std::min(equipo1.golesAFavor, 6);
    equipo2.golesAFavor = std::min(equipo2.golesAFavor, 6);
    equipo1.golesEnContra = std::min(equipo1.golesEnContra, 6);
    equipo2.golesEnContra = std::min(equipo2.golesEnContra, 6);
    equipo1.golesAFavor = std::max(equipo1.golesAFavor, 0);
    equipo2.golesAFavor = std::max(equipo2.golesAFavor, 0);
    equipo1.golesEnContra = std::max(equipo1.golesEnContra, 0);
    equipo2.golesEnContra = std::max(equipo2.golesEnContra, 0);
}

    void simularGrupo(vector<Equipo> & equipos)
    {
        for (int i = 0; i < equipos.size(); ++i)
        {
            for (int j = i + 1; j < equipos.size(); ++j)
            {
                if (equipos[i].partidosJugados < 3 && equipos[j].partidosJugados < 3)
                {
                    simularPartido(equipos[i], equipos[j]);
                }
            }
        }
}
void leerEquipos(const string& nombreGrupo) {
    ifstream archivo("Grupo" + nombreGrupo + ".txt");
    string linea;

    while (getline(archivo, linea)) {
        istringstream iss(linea);
        string nombreEquipo;
        int habilidad;
        iss >> nombreEquipo >> habilidad;

        Equipo equipo;
        equipo.nombre = nombreEquipo;
        equipo.habilidad = habilidad;
        equipo.partidosJugados = 0;
        
        equipo.partidosGanados = 0;
        equipo.partidosEmpatados = 0;
        equipo.partidosPerdidos = 0;
        equipo.golesAFavor = 0;
        equipo.golesEnContra = 0;
        equipo.diferenciaGol = 0;
        equipo.puntos = 0;

        equipos[nombreEquipo] = equipo;
    }

    archivo.close();
}
vector<Equipo> equiposQueAvanzan() {
    vector<Equipo> equiposVec;
    for (const auto& par : equipos) {
        equiposVec.push_back(par.second);
    }

    sort(equiposVec.begin(), equiposVec.end(), [](const Equipo& a, const Equipo& b) {
        if (a.puntos != b.puntos) {
            return a.puntos > b.puntos;
        } else if (a.diferenciaGol != b.diferenciaGol) {
            return a.diferenciaGol > b.diferenciaGol;
        } else {
            return a.golesAFavor > b.golesAFavor;
        }
    });
    return {equiposVec[0], equiposVec[1]};
}
int simularTandaPenales()
{
    int penalesEquipo1 = 0;
    int penalesEquipo2 = 0;

    for (int i = 0; i < 5; ++i) {
        if (std::rand() % 2 == 0) {
            ++penalesEquipo1;
        } else {
            ++penalesEquipo2;
        }
    }
    return penalesEquipo1 > penalesEquipo2 ? 1 : 2;
}

int main() {
    system("color 0B");
    srand(time(nullptr));
    map<char, vector<Equipo>> equiposAvanzadosPorGrupo;
    cout << "****************************************************" << endl;
    cout << "************ BIENVENIDO A LA COPA DEL MUNDO CATAR 2022 ************" << endl;
    cout << "****************************************************" << endl;

    for (char grupo = 'A'; grupo <= 'H'; ++grupo) {
        string nombreGrupo(1, grupo);
        leerEquipos(nombreGrupo);

        for (const auto& parEquipo1 : equipos) {
            for (const auto& parEquipo2 : equipos) {
                if (parEquipo1.first != parEquipo2.first) {
                    simularPartido(equipos[parEquipo1.first], equipos[parEquipo2.first]);
                }
            }
        }
        cout << "****************************" << endl;
        cout << "Resultados Grupo " << nombreGrupo << ":" << endl;
        for (const auto& par : equipos) {
            const Equipo& equipo = par.second;
            cout << "Equipo: " << equipo.nombre << endl;
            cout << "Partidos jugados: " << equipo.partidosJugados << endl;
            cout << "Partidos ganados: " << equipo.partidosGanados << endl;
            cout << "Partidos empatados: " << equipo.partidosEmpatados << endl;
            cout << "Partidos perdidos: " << equipo.partidosPerdidos << endl;
            cout << "Goles a favor: " << equipo.golesAFavor << endl;
            cout << "Goles en contra: " << equipo.golesEnContra << endl;
            cout << "Diferencia de gol: " << equipo.diferenciaGol << endl;
            cout << "Puntos: " << equipo.puntos << endl;
        cout << "****************************" << endl;
            cout << endl;
        }

        vector<Equipo> equiposAvanzados = equiposQueAvanzan();
        equiposAvanzadosPorGrupo[grupo] = equiposAvanzados;

        equipos.clear();
    }
    cout << "\nEquipos Clasificados por Grupo:" << endl;
    
    for (const auto& par : equiposAvanzadosPorGrupo) {
        cout << "*************" << endl;
        cout << "Grupo " << par.first << ":" << endl;
        for (const auto& equipo : par.second) {
            cout << equipo.nombre << endl;
        }
        cout << "*************" << endl;
        cout << endl;
    }
    cout << "\nLlaves de Octavos de Final:" << endl;
    vector<Llave> llavesOctavos;
    vector<vector<Equipo>> equiposPorGrupo;

    for (const auto &par : equiposAvanzadosPorGrupo) {
        equiposPorGrupo.push_back(par.second);
    }

    for (size_t i = 0; i < equiposPorGrupo.size(); ++i) {
        size_t j = (i + 1) % equiposPorGrupo.size();
        llavesOctavos.push_back({equiposPorGrupo[i][0], equiposPorGrupo[j][1]});
    }

    for (size_t i = 0; i < llavesOctavos.size(); ++i) {
        cout << "Llave " << (i + 1) << ": " << llavesOctavos[i].first.nombre << " vs " << llavesOctavos[i].second.nombre << endl;
    }

    cout << "\nDesea simular los octavos de final? (S/N): ";
    char respuestaOctavos;
    cin >> respuestaOctavos;

    if (toupper(respuestaOctavos) == 'S') {
        queue<Llave> partidosColaOctavos;
        stack<Equipo> equiposGanadoresPilaOctavos;

        for (size_t i = 0; i < llavesOctavos.size(); ++i) {
            Llave llave;
            llave.first = llavesOctavos[i].first;
            llave.second = llavesOctavos[i].second;

            partidosColaOctavos.push(llave);
        }

        while (!partidosColaOctavos.empty()) {
            Llave llave = partidosColaOctavos.front();
            partidosColaOctavos.pop();

            cout << "\nSimulando partido de octavos de final \nLlave: "
                << llave.first.nombre << " vs " << llave.second.nombre << endl;


            simularPartido(llave.first, llave.second);
            cout << "Resultado: " << llave.first.nombre << " " << llave.first.golesAFavor << " - "
                << llave.second.golesAFavor << " " << llave.second.nombre << endl;
            std::srand(std::time(nullptr));
            while (llave.first.golesAFavor == llave.second.golesAFavor) {
    int ganadorTandaPenales = simularTandaPenales();

    cout << "Se procede a la tanda de penales:" << endl;
    cout << llave.first.nombre << ": " << (ganadorTandaPenales == 1 ? "Ganador" : "Perdedor") << endl;
    cout << llave.second.nombre << ": " << (ganadorTandaPenales == 2 ? "Ganador" : "Perdedor") << endl;

    if (ganadorTandaPenales == 1) {
        llave.first.golesAFavor += 1;
        llave.second.golesEnContra += 1;
        llave.first.partidosGanados++;
        llave.second.partidosPerdidos++;
        llave.first.puntos += 3;
    } else {
        llave.second.golesAFavor += 1;
        llave.first.golesEnContra += 1;
        llave.second.partidosGanados++;
        llave.first.partidosPerdidos++;
        llave.second.puntos += 3;
    }
}
            if (llave.first.golesAFavor > llave.second.golesAFavor) {
                cout << "Ganador: " << llave.first.nombre << endl;
                equiposGanadoresPilaOctavos.push(llave.first);
            }
            else {
                cout << "Ganador: " << llave.second.nombre << endl;
                equiposGanadoresPilaOctavos.push(llave.second);
            }
        }

        cout << "\nGanadores de octavos de final:" << endl;
stack<Equipo> equiposGanadoresPilaCuartosMostrar = equiposGanadoresPilaOctavos;
while (!equiposGanadoresPilaCuartosMostrar.empty()) {
    cout << "Llave: " << equiposGanadoresPilaCuartosMostrar.top().nombre << endl;
    equiposGanadoresPilaCuartosMostrar.pop();
}
    cout << "\nLlaves de Cuartos de Final:" << endl;
    stack<Equipo> equiposGanadoresPilaCuartos = equiposGanadoresPilaOctavos;
    vector<Llave> llavesCuartos;
    for (size_t i = 0; i < 4; ++i) {
    Llave llave;
    llave.first = equiposGanadoresPilaCuartos.top();
    equiposGanadoresPilaCuartos.pop();
    llave.second = equiposGanadoresPilaCuartos.top();
    equiposGanadoresPilaCuartos.pop();

    llavesCuartos.push_back(llave);
    cout << "Llave " << (i + 1) << ": " << llave.first.nombre << " vs " << llave.second.nombre << endl;
    }
        cout << "\nDesea simular los cuartos de final? (S/N): ";
char respuestaCuartos;
cin >> respuestaCuartos;

if (toupper(respuestaCuartos) == 'S') {
    queue<Llave> partidosColaCuartos;
    stack<Equipo> equiposGanadoresPilaCuartos;
    for (size_t i = 0; i < llavesCuartos.size(); ++i) {
        Llave llave = llavesCuartos[i];
        partidosColaCuartos.push(llave);
    }
    while (!partidosColaCuartos.empty()) {
        Llave llave = partidosColaCuartos.front();
        partidosColaCuartos.pop();

        cout << "\nSimulando partido de cuartos de final \nLlave: "
            << llave.first.nombre << " vs " << llave.second.nombre << endl;

        simularPartido(llave.first, llave.second);
        cout << "Resultado: " << llave.first.nombre << " " << llave.first.golesAFavor << " - "
            << llave.second.golesAFavor << " " << llave.second.nombre << endl;

        std::srand(std::time(nullptr));
        while (llave.first.golesAFavor == llave.second.golesAFavor) {
    int ganadorTandaPenales = simularTandaPenales();

    cout << "Se procede a la tanda de penales:" << endl;
    cout << llave.first.nombre << ": " << (ganadorTandaPenales == 1 ? "Ganador" : "Perdedor") << endl;
    cout << llave.second.nombre << ": " << (ganadorTandaPenales == 2 ? "Ganador" : "Perdedor") << endl;

    if (ganadorTandaPenales == 1) {
        llave.first.golesAFavor += 1;
        llave.second.golesEnContra += 1;
        llave.first.partidosGanados++;
        llave.second.partidosPerdidos++;
        llave.first.puntos += 3;
    } else {
        llave.second.golesAFavor += 1;
        llave.first.golesEnContra += 1;
        llave.second.partidosGanados++;
        llave.first.partidosPerdidos++;
        llave.second.puntos += 3;
    }
}

        if (llave.first.golesAFavor > llave.second.golesAFavor) {
            cout << "Ganador: " << llave.first.nombre << endl;
            equiposGanadoresPilaCuartos.push(llave.first);
        } else {
            cout << "Ganador: " << llave.second.nombre << endl;
            equiposGanadoresPilaCuartos.push(llave.second);
        }
        
    }
    cout << "\nGanadores de cuartos de final:" << endl;
stack<Equipo> equiposGanadoresPilaCuartosMostrar;

for (size_t i = 0; i < llavesCuartos.size(); ++i) {
    Llave llave = llavesCuartos[i];
    if (llave.first.golesAFavor > llave.second.golesAFavor) {
        equiposGanadoresPilaCuartosMostrar.push(llave.first);
        cout << "Ganador Llave " << (i + 1) << ": " << llave.first.nombre << endl;
    } else {
        equiposGanadoresPilaCuartosMostrar.push(llave.second);
        cout << "Ganador Llave " << (i + 1) << ": " << llave.second.nombre << endl;
    }
}
cout << "\nLlaves de Semifinales:" << endl;
stack<Equipo> equiposGanadoresPilaSemis = equiposGanadoresPilaCuartosMostrar;

for (size_t i = 0; i < 2; ++i) {
    Llave llave;
    llave.first = equiposGanadoresPilaSemis.top();
    equiposGanadoresPilaSemis.pop();
    llave.second = equiposGanadoresPilaSemis.top();
    equiposGanadoresPilaSemis.pop();
    cout << "Llave " << (i + 1) << ": " << llave.first.nombre << " vs " << llave.second.nombre << endl;
} 
cout << "\nDesea simular las semifinales? (S/N): ";
char respuestaSemis;
cin >> respuestaSemis;

if (toupper(respuestaSemis) == 'S') {
    queue<Llave> partidosColaSemis;
    stack<Equipo> equiposGanadoresPilaSemis = equiposGanadoresPilaCuartosMostrar;
    stack<Equipo> equiposPerdedoresPilaSemis;  

    for (size_t i = 0; i < 2; ++i) {
        Llave llave;
        llave.first = equiposGanadoresPilaSemis.top();
        equiposGanadoresPilaSemis.pop();
        llave.second = equiposGanadoresPilaSemis.top();
        equiposGanadoresPilaSemis.pop();
        partidosColaSemis.push(llave);
    }

    while (!partidosColaSemis.empty()) {
        Llave llave = partidosColaSemis.front();
        partidosColaSemis.pop();

        cout << "\nSimulando partido de semifinal \nLlave: "
            << llave.first.nombre << " vs " << llave.second.nombre << endl;

        simularPartido(llave.first, llave.second);
        cout << "Resultado: " << llave.first.nombre << " " << llave.first.golesAFavor << " - "
            << llave.second.golesAFavor << " " << llave.second.nombre << endl;

        std::srand(std::time(nullptr));
            // ...

while (llave.first.golesAFavor == llave.second.golesAFavor) {
    int ganadorTandaPenales = simularTandaPenales();

    cout << "Se procede a la tanda de penales:" << endl;
    cout << llave.first.nombre << ": " << (ganadorTandaPenales == 1 ? "Ganador" : "Perdedor") << endl;
    cout << llave.second.nombre << ": " << (ganadorTandaPenales == 2 ? "Ganador" : "Perdedor") << endl;

    if (ganadorTandaPenales == 1) {
        llave.first.golesAFavor += 1;
        llave.second.golesEnContra += 1;
        llave.first.partidosGanados++;
        llave.second.partidosPerdidos++;
        llave.first.puntos += 3;
    } else {
        llave.second.golesAFavor += 1;
        llave.first.golesEnContra += 1;
        llave.second.partidosGanados++;
        llave.first.partidosPerdidos++;
        llave.second.puntos += 3;
    }
}

        if (llave.first.golesAFavor > llave.second.golesAFavor) {
            cout << "Ganador: " << llave.first.nombre << endl;
            equiposGanadoresPilaSemis.push(llave.first);
            equiposPerdedoresPilaSemis.push(llave.second); 
        } else {
            cout << "Ganador: " << llave.second.nombre << endl;
            equiposGanadoresPilaSemis.push(llave.second);
            equiposPerdedoresPilaSemis.push(llave.first); 
        }
    }
    cout << "\nGanadores de las semifinales:" << endl;
stack<Equipo> equiposGanadoresPilaFinal;
while (!equiposGanadoresPilaSemis.empty()) {
    Equipo ganador = equiposGanadoresPilaSemis.top();
    equiposGanadoresPilaSemis.pop();
    cout << "Equipo: " << ganador.nombre << endl;
    equiposGanadoresPilaFinal.push(ganador); 
}
    cout << "\nPerdedores de las semifinales:" << endl;
stack<Equipo> equiposPerdedoresPilaTercerCuarto;

while (!equiposPerdedoresPilaSemis.empty()) {
    Equipo perdedor = equiposPerdedoresPilaSemis.top();
    equiposPerdedoresPilaSemis.pop();
    cout << "Equipo: " << perdedor.nombre << endl;
    equiposPerdedoresPilaTercerCuarto.push(perdedor); 
}
cout << "\nFinal:" << endl;
Llave llaveFinal;
llaveFinal.first = equiposGanadoresPilaFinal.top();
equiposGanadoresPilaFinal.pop();
llaveFinal.second = equiposGanadoresPilaFinal.top();
equiposGanadoresPilaFinal.pop();
cout << "" << llaveFinal.first.nombre << " vs " << llaveFinal.second.nombre << endl;

cout << "\nTercer y Cuarto Puesto:" << endl;
Llave llaveTercerCuarto;
llaveTercerCuarto.first = equiposPerdedoresPilaTercerCuarto.top();
equiposPerdedoresPilaTercerCuarto.pop();
llaveTercerCuarto.second = equiposPerdedoresPilaTercerCuarto.top();
equiposPerdedoresPilaTercerCuarto.pop();
cout << llaveTercerCuarto.first.nombre << " vs " << llaveTercerCuarto.second.nombre << endl;
cout << "\nDesea simular el partido por el Tercer y Cuarto Puesto? (S/N): ";
char respuestaTercerCuarto;
cin >> respuestaTercerCuarto;

if (toupper(respuestaTercerCuarto) == 'S') {
    cout << "\nSimulando partido por el Tercer y Cuarto Puesto: "
        << llaveTercerCuarto.first.nombre << " vs " << llaveTercerCuarto.second.nombre << endl;

    simularPartido(llaveTercerCuarto.first, llaveTercerCuarto.second);

    cout << "Resultado: " << llaveTercerCuarto.first.nombre << " " << llaveTercerCuarto.first.golesAFavor << " - "
        << llaveTercerCuarto.second.golesAFavor << " " << llaveTercerCuarto.second.nombre << endl;
    std::srand(std::time(nullptr));
    while (llaveTercerCuarto.first.golesAFavor == llaveTercerCuarto.second.golesAFavor)
{
    int ganadorTandaPenales = simularTandaPenales();

    cout << "Se procede a la tanda de penales:" << endl;
    cout << llaveTercerCuarto.first.nombre << ": " << (ganadorTandaPenales == 1 ? "Ganador" : "Perdedor") << endl;
    cout << llaveTercerCuarto.second.nombre << ": " << (ganadorTandaPenales == 2 ? "Ganador" : "Perdedor") << endl;

    if (ganadorTandaPenales == 1)
    {
        llaveTercerCuarto.first.golesAFavor += 1;
        llaveTercerCuarto.second.golesEnContra += 1;
        llaveTercerCuarto.first.partidosGanados++;
        llaveTercerCuarto.second.partidosPerdidos++;
        llaveTercerCuarto.first.puntos += 3;
    }
    else
    {
        llaveTercerCuarto.second.golesAFavor += 1;
        llaveTercerCuarto.first.golesEnContra += 1;
        llaveTercerCuarto.second.partidosGanados++;
        llaveTercerCuarto.first.partidosPerdidos++;
        llaveTercerCuarto.second.puntos += 3;
    }
}

    Equipo ganadorTercerCuarto, perdedorTercerCuarto;
    if (llaveTercerCuarto.first.golesAFavor > llaveTercerCuarto.second.golesAFavor) {
        ganadorTercerCuarto = llaveTercerCuarto.first;
        perdedorTercerCuarto = llaveTercerCuarto.second;
    } else {
        ganadorTercerCuarto = llaveTercerCuarto.second;
        perdedorTercerCuarto = llaveTercerCuarto.first;
    }
    cout << "\nGanador del Tercer y Cuarto Puesto: " << ganadorTercerCuarto.nombre << endl;
} else {
    cout << "\nEl usuario ha optado por no simular el partido por el Tercer y Cuarto Puesto." << endl;
}

cout << "\nDesea simular la final? (S/N): ";
char respuestaFinal;
cin >> respuestaFinal;

if (toupper(respuestaFinal) == 'S')
{
    cout << "\nSimulando la final:" << endl;

    simularPartido(llaveFinal.first, llaveFinal.second);

    srand(time(0)); 

    int golEquipo1 = rand() % 6; 
    int golEquipo2 = rand() % 6; 

    llaveFinal.first.golesAFavor = golEquipo1;
    llaveFinal.second.golesAFavor = golEquipo2;

    cout << "Resultado: " << llaveFinal.first.nombre << " " << llaveFinal.first.golesAFavor << " - "
        << llaveFinal.second.golesAFavor << " " << llaveFinal.second.nombre << endl;
    std::srand(std::time(nullptr));
    while (llaveFinal.first.golesAFavor == llaveFinal.second.golesAFavor)
{
    int ganadorTandaPenales = simularTandaPenales();

    cout << "Se procede a la tanda de penales:" << endl;
    cout << llaveFinal.first.nombre << ": " << (ganadorTandaPenales == 1 ? "Ganador" : "Perdedor") << endl;
    cout << llaveFinal.second.nombre << ": " << (ganadorTandaPenales == 2 ? "Ganador" : "Perdedor") << endl;

    if (ganadorTandaPenales == 1)
    {
        llaveFinal.first.golesAFavor += 1;
        llaveFinal.second.golesEnContra += 1;
        llaveFinal.first.partidosGanados++;
        llaveFinal.second.partidosPerdidos++;
        llaveFinal.first.puntos += 3;
    }
    else
    {
        llaveFinal.second.golesAFavor += 1;
        llaveFinal.first.golesEnContra += 1;
        llaveFinal.second.partidosGanados++;
        llaveFinal.first.partidosPerdidos++;
        llaveFinal.second.puntos += 3;
    }
}

    if ((llaveFinal.first.golesAFavor) > (llaveFinal.second.golesAFavor)) {
        cout << "Ganador de la Copa: " << llaveFinal.first.nombre << endl;
    }else{
            cout << "Ganador de la Copa: " << llaveFinal.second.nombre << endl;
}


            cout << "****************************************************" << endl;
cout << "************ FELICITACIONES AL CAMPEON! ************" << endl;
cout << "****************************************************" << endl;
cout << "        ________   ________   __   ________ " << endl;
cout << "       |        | |        | |  | |        |" << endl;
cout << "       |   __   | |    __  | |  | |  __    |" << endl;
cout << "       |  |  |  | |   |  | | |  | | |  |   |" << endl;
cout << "       |  |__|  | |   |__| | |  | | |__|   |" << endl;
cout << "       |        | |        | |  | |        |" << endl;
cout << "       |__    __| |__    __| |__| |__    __|" << endl;
cout << "          |  |       |  |             |  |   " << endl;
cout << "          |__|       |__|             |__|   " << endl;
cout << "****************************************************" << endl;
cout << "   Felicidades al equipo " << ((llaveFinal.first.golesAFavor > llaveFinal.second.golesAFavor) ? llaveFinal.first.nombre : llaveFinal.second.nombre) << "!" << endl;
cout << "    Gracias por participar en este emocionante Mundial." << endl;
cout << "****************************************************" << endl;
}
    return 0;
}
}
}
}