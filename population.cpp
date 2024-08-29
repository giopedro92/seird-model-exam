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
  double Rf = initial_state.R + g * initial_state.I * (1 - m);
  double Ef =
      initial_state.E * (1 - s) + (b * (initial_state.S * initial_state.I) / N);
  double Df = initial_state.D + (m * g * initial_state.I);
  double If = N - Sf - Rf - Ef - Df;

  initial_state = {Sf, Ef, If, Rf, Df};
}

// funzione che definisce il metodo di approssimazione
void Population::approximation(State& initial_state, double number) {
  double s1, e, i, r, d;
  s1 = initial_state.S;
  e = initial_state.E;
  i = initial_state.I;
  r = initial_state.R;
  d = initial_state.D;

  std::vector<double> sired = {s1, e, i, r, d};
  long unsigned int index = 0;
  double max_rounding = -1.;

  for (long unsigned int j = 0; j < 5; j++) {
    double value = std::abs(sired[j] - std::round(sired[j]));
    if (value > max_rounding) {
      max_rounding = value;
      index = j;
    }
  }

  for (unsigned int k = 0; k < sired.size(); ++k) {
    if (k != index) {
      sired[k] = std::round(sired[k]);
    }
  }
  sired[index] = number - sired[(index + 1) % 5] - sired[(index + 2) % 5] -
                 sired[(index + 3) % 5] - sired[(index + 4) % 5];

  assert(sired[0] + sired[1] + sired[2] + sired[3] + sired[4] == number);

  initial_state.S = sired[0];
  initial_state.E = sired[1];
  initial_state.I = sired[2];
  initial_state.R = sired[3];
  initial_state.D = sired[4];
}

// Questa funzione restituisce un vettore contenente lo stato dell'epidemia di
// giorno in giorno
std::vector<State> Population::evolve(State& initial_state) {
  std::vector<State> dati{initial_state};
  for (int c = 2; c <= T && (initial_state.I != 0 || initial_state.E != 0);
       c++) {
    next(initial_state);
    State copy_state =
        initial_state;  // abbiamo creato un copy state in modo tale che
                        // l'iterazione successiva vengo comunque calcolata con
                        // i numeri decimali
    this->approximation(copy_state, N);
    dati.push_back(copy_state);
    if (copy_state.I == 0 && copy_state.E == 0) break;
  };
  int k = static_cast<int>(dati.size());
  assert(k <= T &&
         "il for è stato iterato più volte dei giorni a disposizione per la "
         "simulazione");
  return dati;
}

// Questa funzione calcola dopo quanti giorni l'epidemia inizia a decrescere
int Population::peak(std::vector<State> const& dati) {
  long int offset = static_cast<long int>(1 / s);
  auto It = std::find_if(dati.begin() + offset, dati.end(),
                         [this](const State& state) {
                           return (state.E * this->s) / (state.I * this->g) <
                                  1;  // condizione affinchè l'epidemia inizi ad
                                      // essere in contrazione
                         });
  if (It != dati.end()) {
    int thr = static_cast<int>(std::distance(dati.begin(), It));
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
  size_t c = solution.size();  //
  double s2{}, e{}, i{}, r{}, d{};
  std::cout << "giorno suscettibili esposti infetti guariti morti\n";
  for (size_t j = 0; j < c; j++) {
    s2 = solution[j].S;
    e = solution[j].E;
    r = solution[j].R;
    i = solution[j].I;
    d = solution[j].D;

    std::cout << j << "\t" << s2 << "\t" << e << "\t" << i << "\t" << r << "\t"
              << d << '\n';
  };

  if (i == 0 && e == 0) {
    std::cout << "L'epidemia si è risolta in " << c << " giorni." << '\n';
  } else {
    std::cout << "L'epidemia è ancora in corso." << '\n';
  };

  std::ofstream file;
  file.open("data.dat");
  if (!file) {
    std::cerr << "Errore nell'apertura del file!" << std::endl;
  }
  int day = 1;
  for (State state : solution) {
    file << day << " " << state.S << " " << state.E << " " << state.I << " "
         << state.R << " " << state.D << '\n';
    day++;
  };
  file.close();
}
