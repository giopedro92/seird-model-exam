#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "population.hpp"

// Verifica delle condizioni beta e gamma
TEST_CASE("beta = 0") { //non ci si infetta
  Population pop{0, 0.5, 0.5, 0.3, 100, 100}; //beta, gamma, sigma, mu, N, T
  State initial_state{95, 0, 5, 0, 0};  //S, E, I, R, D
  pop.evolve(initial_state);
  CHECK(initial_state.S == 95);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("gamma = 0") { //non si può guarire/morire
  Population pop{0.5, 0, 0.9, 0,7, 100, 100};
  State initial_state{95, 0, 5, 0, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.R == 0);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("sigma = 0") { //tasso di latenza nullo, tempo di incubazione infinito
  Population pop{0.5, 0.2, 0, 0.9, 100, 100};
  State initial_state{95, 5, 0, 0, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.R == 0 && initial_state.D == 0);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("mu = 1") { //non si può sopravvivere
  Population pop{0.8, 0.3, 0.3, 1, 100, 100};
  State initial_state{95, 0, 5, 0, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.R == 0);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("mu = 0") { //non si può morire, la malattia non genera sintomi gravi
  Population pop{0.8, 0.3, 0.3, 1, 100, 100};
  State initial_state{95, 0, 5, 0, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.D == 0);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("favorevole") {
  Population pop{0.2, 0.9, 0.9, 0.1, 100, 2000};
  State initial_state{90, 0, 10, 0 , 0};
  pop.evolve(initial_state);
  CHECK(initial_state.I < 0.5); //verifica che finisce la pandemia
  CHECK(initial_state.S > (initial_state.R + initial_state.D));
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("favorevole ma pandemia ancora in corso") {
  Population pop{0.2, 0.9, 0.9, 0.1, 100, 30};
  State initial_state{90, 0, 10, 0 , 0};
  pop.evolve(initial_state);
  CHECK(initial_state.I != 0); 
  CHECK(initial_state.S > initial_state.R);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("sfavorevole") {
  Population pop{0.9, 0.2, 0.2, 0.9, 100, 100};
  State initial_state{60, 0, 40, 0, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.S < 0.5);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(100));
};

TEST_CASE("alta popolazione") {
  Population pop{0.78, 0.27, 0.5, 0.5, 10000, 100};
  State initial_state{9500, 0, 500, 0, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(10000));
};

TEST_CASE("La pandemia non comincia") { //non ho un picco. beta/gamma < 1, S circa N
Population pop{0.3, 0.5, 0.5, 0.5, 10000, 100};
  State initial_state{9999, 0, 1, 0, 0};
  auto dati = pop.evolve(initial_state);
  CHECK(dati.size() <= 2);
};

TEST_CASE("Ho un picco") { //Dati COVID per emilia-romagna
Population pop{0.35, 0.14, 0.19, 0.009, 45000000, 365};
  State initial_state{44999980, 0, 20, 0, 0};
  auto dati = pop.evolve(initial_state);
  CHECK( CHECK(initial_state.S + initial_state.I + initial_state.R + initial_state.E + initial_state.D ==
        doctest::Approx(1000000)););
};

//guarda cosa fa cmake(README anno 2023)