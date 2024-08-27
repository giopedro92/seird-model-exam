#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "population.hpp"

struct Parameters {
  double b;
  double g;
  double N;
  double T;
  double i;
  double t;  // tempo medio di incubazione
  double m;
  double s;
};

bool isNotInteger(double n) { return n - (int)n != 0; };
Parameters readVariablesFromFile(const std::string& filename);

int main() {
  State initial_state{};
  Parameters pars;
  /*double b{};
  double g{};
  double N{};   poiché provengono da cin, a priori sono double,
  double T{};   una verifica successiva li restringe a int
  double i{};*/

  enum inputWay {  // unscoped enumeration
    FILE = 1,
    INTERACTIVE
  };

  std::cout << "Come vuoi inserire i parametri?" << '\n';
  std::cout << "1. Da file di configurazione" << '\n';
  std::cout << "2. Dal terminale" << '\n';
  std::cout << "Scegli un'opzione: ";
  int choice;
  std::cin >> choice;
  // std::cin.ignore();

  switch (choice) {
    case FILE: {
      std::string configFile = "ConfigFile.txt";
      pars = readVariablesFromFile(configFile);
      break;
    }
    case INTERACTIVE: {
      std::cout << "Inserire la durata in giorni della simulazione." << '\n';
      std::cin >> pars.T;
      while (pars.T <= 0 || isNotInteger(pars.T)) {
        std::cout << "Inserire la durata in giorni della simulazione intera e "
                     "maggiore di zero."
                  << '\n';
        std::cin >> pars.T;
      };

      std::cout << "Inserire la dimensione della popolazione." << '\n';
      std::cin >> pars.N;
      while (pars.N <= 0 || isNotInteger(pars.N)) {
        std::cout << "Inserire la dimensione della popolazione intera e "
                     "maggiore di zero."
                  << '\n';
        std::cin >> pars.N;
      };

      do {
        std::cout << "Inserire il numero iniziale di infetti." << '\n';
        std::cin >> pars.i;
      } while (pars.i <= 0 || pars.i > pars.N || isNotInteger(pars.i));

      do {
        std::cout << "\u03B2 è la probabilità di contagio." << '\n'
                  << "Inserire il valore di \u03B2 (valori accettati tra 0 e "
                     "1 inclusi)."
                  << '\n';
        std::cin >> pars.b;
      } while (pars.b < 0. || pars.b > 1.);

      std::cout << "Inserire il tempo di incubazione in giorni." << '\n';
      std::cin >> pars.t;
      while (pars.t <= 0 || isNotInteger(pars.t)) {
        std::cout << "Inserire il tempo di incubazione intero e "
                     "maggiore di zero."
                  << '\n';
        std::cin >> pars.t;
        pars.s = 1 / pars.t;
      };

      do {
        std::cout << "\u03B3 è l'inverso del tempo medio di risoluzione "
                     "dell'infezione."
                  << '\n'
                  << "Inserire il valore di \u03B3 (valori accettati tra 0 e "
                     "1 inclusi)."
                  << '\n';
        std::cin >> pars.g;
      } while (pars.g < 0. || pars.g > 1.);

      do {
        std::cout << "\u03BC è il tasso di mortalità del virus." << '\n'
                  << "Inserire il valore di \u03BC (valori accettati tra 0 e "
                     "1 inclusi)."
                  << '\n';
        std::cin >> pars.g;
      } while (pars.g < 0. || pars.g > 1.);

      break;
    }
    default:
      std::cout << "Scelta non valida." << '\n';
  }

  initial_state.S = pars.N - pars.i;
  initial_state.E = 0;
  initial_state.D = 0;
  initial_state.I = pars.i;

  Population result(pars.b, pars.g, pars.s, pars.m, pars.N, pars.T);

  std::vector<State> solution = result.evolve(initial_state);
  int threshold = result.peak(solution);
  std::ofstream file1;
  file1.open("threshold.dat");
  file1 << threshold;
  file1.close();
  std::ofstream file2;
  file2.open("population.dat");
  file2 << pars.N;
  file2.close();
  result.print(solution);
}

Parameters readVariablesFromFile(const std::string& filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Errore nell'apertura del file: " + filename);
  }

  std::vector<double> values;

  double value;
  while (file >> value) {
    values.push_back(value);
  }

  file.close();
  Parameters input_data{};
  input_data.b = values[0];
  input_data.g = values[1];
  input_data.N = values[2];
  input_data.T = values[3];
  input_data.i = values[4];
  input_data.t = values[5];
  input_data.m = values[6];
  input_data.s = 1 / input_data.t;
  return input_data;
}