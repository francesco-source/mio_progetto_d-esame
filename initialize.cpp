#pragma once
#include"board.h"
// q_prob_ è la probabilità che lo stato trovi un malato e lo metta in quarantena 
    Sir_model::Board::Board(int n, double b, double y,double q_prob, Quarantene_parameters quarantene,Mode adv_opt) :
        grid_(n + 2, std::vector<Cell>(n + 2)), dimension_{ n + 2 }, q_prob_{q_prob},
        beta_{ b }, gamma_{ y }, advanced_opt{adv_opt}{
        //Verifico la coerenza dei dati
        if (n < 100) {
            throw std::runtime_error{ "Error: the dimension is too tiny for see any simulation  effect."
                "The minim dimension is 100" };
        }
        assert(b > 0.1 && b < 1);
        assert(y > 0.1 && y < 1);
        quaranten.len_line = dimension_ /2;
        quaranten.first_day = quarantene.first_day;
        quaranten.last_day = quarantene.last_day;
        day = 0;
   };
    Sir& Sir_model::Board::operator()(int riga, int colonna) {
        return (grid_[riga + 1][colonna + 1].state);
    }
