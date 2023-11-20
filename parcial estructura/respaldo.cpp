#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

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

map<string, Equipo> equipos;

void simularPartido(Equipo& equipo1, Equipo& equipo2) {
    if (equipo1.partidosJugados >= 3 || equipo2.partidosJugados >= 3) {
        return;
    }

    int probabilidad_equipo1 = equipo1.habilidad;
    int probabilidad_equipo2 = equipo2.habilidad;

    // Ajustar probabilidades para que la suma no supere el 100%
    probabilidad_equipo1 = min(probabilidad_equipo1, 100 - probabilidad_equipo2);
    probabilidad_equipo2 = min(probabilidad_equipo2, 100 - probabilidad_equipo1);

    // Asegurar que la suma de probabilidades sea mayor que 0
    probabilidad_equipo1 = max(probabilidad_equipo1, 1);
    probabilidad_equipo2 = max(probabilidad_equipo2, 1);

    int resultado = rand() % (probabilidad_equipo1 + probabilidad_equipo2);

    if (resultado < probabilidad_equipo1) {
        equipo1.golesAFavor++;
        equipo2.golesEnContra++;
        equipo1.partidosGanados++;
        equipo2.partidosPerdidos++;
        equipo1.puntos += 3;
    } else if (resultado < probabilidad_equipo1 + probabilidad_equipo2) {
        equipo2.golesAFavor++;
        equipo1.golesEnContra++;
        equipo2.partidosGanados++;
        equipo1.partidosPerdidos++;
        equipo2.puntos += 3;
    } else {
        equipo1.partidosEmpatados++;
        equipo2.partidosEmpatados++;
        equipo1.puntos++;
        equipo2.puntos++;
    }

    equipo1.partidosJugados++;
    equipo2.partidosJugados++;

    equipo1.golesEnContra += equipo2.golesAFavor;
    equipo2.golesEnContra += equipo1.golesAFavor;

    equipo1.diferenciaGol = equipo1.golesAFavor - equipo1.golesEnContra;
    equipo2.diferenciaGol = equipo2.golesAFavor - equipo2.golesEnContra;
}
void simularGrupo(vector<Equipo>& equipos) {
    for (int i = 0; i < equipos.size(); ++i) {
        for (int j = i + 1; j < equipos.size(); ++j) {
            // Simular solo si aún no han jugado 3 partidos
            if (equipos[i].partidosJugados < 3 && equipos[j].partidosJugados < 3) {
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

int main() {
    srand(time(nullptr));
    map<char, vector<Equipo>> equiposAvanzadosPorGrupo;
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
            cout << endl;
        }
        vector<Equipo> equiposAvanzados = equiposQueAvanzan();
        equiposAvanzadosPorGrupo[grupo] = equiposAvanzados;

        equipos.clear();
    }

    cout << "Equipos que avanzan por grupo:" << endl;
    for (const auto& par : equiposAvanzadosPorGrupo) {
        char grupo = par.first;
        const vector<Equipo>& equiposAvanzados = par.second;

        cout << "Grupo " << grupo << ": ";
        for (const auto& equipo : equiposAvanzados) {
            cout << equipo.nombre << " ";
        }
        cout << endl;
    }
    cout << "\nSorteo de Octavos de Final:" << endl;
    vector<pair<Equipo, Equipo>> llavesOctavos;
    vector<vector<Equipo>> equiposPorGrupo;
    for (const auto& par : equiposAvanzadosPorGrupo) {
        equiposPorGrupo.push_back(par.second);
    }
    for (size_t i = 0; i < equiposPorGrupo.size(); ++i) {
        size_t j = (i + 1) % equiposPorGrupo.size(); 
        llavesOctavos.push_back({equiposPorGrupo[i][0], equiposPorGrupo[j][1]});
    }
    for (int i = 0; i < llavesOctavos.size(); ++i) {
        cout << "Llave " << (i + 1) << ": " << llavesOctavos[i].first.nombre << " vs " << llavesOctavos[i].second.nombre << endl;
    }
    cout << "\n¿Desea simular los octavos de final? (S/N): ";
    char respuesta;
    cin >> respuesta;

    if (toupper(respuesta) == 'S') {
        for (size_t i = 0; i < llavesOctavos.size(); ++i) {
            cout << "\nSimulando partido de octavos de final \nLlave " << (i + 1) << ": "
                << llavesOctavos[i].first.nombre << " vs " << llavesOctavos[i].second.nombre << endl;
            simularPartido(llavesOctavos[i].first, llavesOctavos[i].second);
            cout << "Resultado: " << llavesOctavos[i].first.nombre << " " << llavesOctavos[i].first.golesAFavor << " - "
                << llavesOctavos[i].second.golesAFavor << " " << llavesOctavos[i].second.nombre << endl;
        }
    }

    return 0;
}

