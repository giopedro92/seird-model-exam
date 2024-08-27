#include "population.hpp"

#include <assert.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

// Questa funzione, dato lo stato iniziale, calcola come si sviluppa l'epidemia
// il giorno successivo
void Population::next(State& initial_state) {
  double Sf = initial_state.S - (b * initial_state.S * initial_state.I) / N;

  // double Sf = std::round(Sf_decimale);

  double Rf = initial_state.R + g * initial_state.I * (1 - m);

  // double Rf = std::round(Rf_decimale);

  double Ef =
      initial_state.E * (1 - s) + (b * (initial_state.S * initial_state.I) / N);

  // double Ef = std::round(Ef_decimale);
  double Df = initial_state.D + (m * g * initial_state.I);

  // double Df = std::round(Df_decimale);

  double If = N - Sf - Rf - Ef - Df;

  initial_state = {Sf, Ef, If, Rf, Df};
};

// Questa funzione riempie un vettore contenente lo stato dell'epidemia di
// giorno in giorno
std::vector<State> Population::evolve(State& initial_state) {
  std::vector<State> dati{initial_state};
  // int i;
  // int e;
  for (int c = 2; c <= T && (initial_state.I != 0 || initial_state.E != 0);
       c++) {
    next(initial_state);
    dati.push_back(initial_state);
    // e = initial_state.E;
    // i = N - initial_state.S - initial_state.R - initial_state.D -
    // initial_state.E;
  };
  int k = dati.size();
  assert(k <= T &&
         "il for è stato iterato più volte dei giorni a disposizione per la "
         "simulazione");
  return dati;
};

// Questa funzione calcola dopo quanti giorni l'epidemia inizia a decrescere
int Population::peak(std::vector<State> const& dati) {
  auto It = std::find_if(dati.begin(), dati.end(), [this](const State& state) {
    return (state.S * this->b) / (this->N * (this->g + this->m)) < 1;
  });
  if (It != dati.end()) {
    int thr = std::distance(dati.begin(), It);
    if (thr == 0) {
      std::cout << "L'epidemia è già in fase di contrazione." << '\n';
    } else {
      std::cout << "E' cominciata la fase di contrazione dell'epidemia dopo "
                << thr << " giorni." << '\n';
    }
    return thr;
  } else {
    std::cout << "La fase di espansione dell'epidemia non è ancora terminata."
              << std::endl;
    return -1;
  }
}

// Questa funzione permette di stampare su terminale e su file i dati ottenuti
// dalle precedenti funzioni
void Population::print(std::vector<State> const& solution) {
  int c = solution.size();  //
  double s{}, e{}, i{}, r{}, d{};
  std::cout << "giorno\tsuscettibili\tesposti\tinfetti\tguariti\tmorti\n";
  for (int j = 0; j < c; j++) {
    s = solution[j].S;  // c++ arrotonda troncando le cifre
                        // decimali.
    e = solution[j].E;
    r = solution[j].R;  // serve una funzione per farlo arrotondare normalmente
    i = solution[j].I;  // N - s - r - e - d;
    d = solution[j].D;  // N - s - r - e - i;
    // assert(s + i + r + e + d == N);

    std::cout << j << "   " << s << "   " << e << "   " << i << "   " << r
              << "   " << d << '\n';
  };
  /*if (i == 0 && c == 2) {
    std::cout << "L'epidemia non è avvenuta" << '\n';
  } else if (i == 0 && e == 0) {*/
  std::cout << "L'epidemia si è risolta in " << c << " giorni." << '\n';
  /*} else {
    std::cout << "L'epidemia è ancora in corso." << '\n';
  };*/

  std::ofstream file;
  file.open("data.dat");
  if (!file) {
    std::cerr << "Errore nell'apertura del file!" << std::endl;
  }
  int day = 1;
  for (State state : solution) {
    file << day << " " << std::round(state.S) << " " << std::round(state.E)
         << " " << N - std::round(state.S) - std::round(state.R) << " "
         << std::round(state.R) << '\n';
    day++;
  };
  file.close();
};
