#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "population.hpp"

// Verifica delle condizioni beta e gamma
TEST_CASE("beta = 0") { //non ci si infetta
  Population pop{0, 0.5, 100, 100}; //beta, gamma, N, T
  State initial_state{95, 5, 0};  //S, I, R
  pop.evolve(initial_state);
  CHECK(initial_state.S == 95);
  CHECK(initial_state.S + initial_state.I + initial_state.R ==
        doctest::Approx(100));
};

TEST_CASE("gamma = 0") { //non si può guarire/morire
  Population pop{0.5, 0, 100, 100};
  State initial_state{95, 5, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.R == 0);
  CHECK(initial_state.S + initial_state.I + initial_state.R ==
        doctest::Approx(100));
};

TEST_CASE("favorevole") {
  Population pop{0.2, 0.9, 100, 100};
  State initial_state{95, 5, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.I < 0.5); //verifica che finisce la pandemia
  CHECK(initial_state.S > initial_state.R);
  CHECK(initial_state.S + initial_state.I + initial_state.R ==
        doctest::Approx(100));
};

TEST_CASE("favorevole ma pandemia ancora in corso") {
  Population pop{0.2, 0.9, 100, 2};
  State initial_state{95, 5, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.I != 0); 
  CHECK(initial_state.S > initial_state.R);
  CHECK(initial_state.S + initial_state.I + initial_state.R ==
        doctest::Approx(100));
};

TEST_CASE("sfavorevole") {
  Population pop{0.9, 0.2, 100, 100};
  State initial_state{60, 40, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.S < 0.5);
  CHECK(initial_state.S + initial_state.I + initial_state.R ==
        doctest::Approx(100));
};

TEST_CASE("alta popolazione") {
  Population pop{0.78, 0.27, 10000, 100};
  State initial_state{9500, 500, 0};
  pop.evolve(initial_state);
  CHECK(initial_state.S + initial_state.I + initial_state.R ==
        doctest::Approx(10000));
};

TEST_CASE("La pandemia non comincia") { //beta/gamma < 1, S circa N
Population pop{0.3, 0.5, 10000, 100};
  State initial_state{9999, 1, 0};
  auto dati = pop.evolve(initial_state);
  CHECK(dati.size() == 2);
};
