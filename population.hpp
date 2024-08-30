#ifndef EPIDEMIA_HPP
#define EPIDEMIA_HPP

#include <string>
#include <vector>

struct State {
  double S;  // suscettibili
  double E;  // esposti
  double I;  // infetti
  double R;  // guariti
  double D;  // morti
};

class Population {
  double b;  // transmission rate
  double g;  // inverso del periodo medio di infezione
  double s;  // inverso del tempo di incubazione
  double m;  // mortality rate
  double N;
  double T;

 public:
  Population(double beta, double gamma, double sigma, double mu, double n,
             double time) {
    b = beta;
    g = gamma;
    s = sigma;
    m = mu;
    N = n;
    T = time;
  }

  void approximation(State& initial_state, double number);

  void next(State& initial_state);

  std::vector<State> evolve(State& initial_state);

  int peak(std::vector<State> const& dati);

  void print(std::vector<State> const& solution);
};

#endif  // EPIDEMIA_HPP