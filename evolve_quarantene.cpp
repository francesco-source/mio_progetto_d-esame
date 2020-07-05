#pragma once
#include"board.h"
#include<ctime>
void Sir_model::Board::evolve_() {
    std::vector<std::vector<Sir>> end(dimension_ - 2, std::vector<Sir>(dimension_ - 2));
    int teoretical_ill = static_cast<int>(dimension_* beta_);
    int malati_trovati = static_cast<int>(teoretical_ill * q_prob_);
    for (int l = 1; l < dimension_ - 1; ++l) {
        for (int c = 1; c < dimension_ - 1; ++c) {
            if (grid_[l][c].state == Sir::s) {
                for (int j = -1; j <= 1; ++j) {
                    for (int i = -1; i <= 1; ++i) {
                        if (grid_[l + j][c + i].state == Sir::i && grid_[l + j][c + i].state != Sir::s){
                            grid_[l][c].inf_prob += beta_; //somma delle probabilita di infettarsi attorno alla cella.
                            //quando tale probabilità diventa uno la cella si ammala
                        }
                        else {
                            grid_[l][c].inf_prob += 0;
                        }
                    }
                }
                if (grid_[l][c].inf_prob >= 1) {
                    ++end[l - 1][c - 1];
                    grid_[l][c].inf_prob = 0;
                }
            }
            //metodo di greg corretto: quando una persona è malata o guarisce però dopo 14 giorni ,oppure viene messa in quarantena prima di guarire.
            //definisco un intervallo( teoretical ill), genero un numero fra 0 e il  sup di tale intervallo.
            //matati trovati è un numero dipendete da q_prob_ compreso fra zero ed il precedente sup. da qui si comprende l if.
            // deve introdurre un opzione avanzata il fatto che ci si possa quarantenare. Fusco deve agire qui
            else if (grid_[l][c].state == Sir::i) {
                int n =(rand()+ time(0))%(teoretical_ill);             
                if (n<malati_trovati && day>10 && ((int)advanced_opt==3||(int)advanced_opt==5)) {
                    end[l - 1][c - 1] = Sir::q;  
                }
                else {
                    grid_[l][c].clock += 1*gamma_;
                    if (grid_[l][c].clock > 14) {
                        end[l - 1][c - 1] = Sir::r;
                        grid_[l][c].clock = 0;
                    }
                    else {
                        end[l - 1][c - 1] = Sir::i;
                    }
                }
            }
            else if (grid_[l][c].state == Sir::r) {
                end[l - 1][c - 1] = Sir::r;
            }
            //questo if è per l ultima opzione avanzata,dove ho il borod della quarantena.
            else if (grid_[l][c].state == Sir::bordo_q) {
                if (day < quaranten.last_day)
                    end[l - 1][c - 1] = Sir::bordo_q;
                else
                    end[l - 1][c - 1] = Sir::s;
            }
            // quando un malatom viene trovato malato viene messo in quarantena. Ci sta per 40 giorni poi è libero. dve essere un opzione avanzata di fusco.
            else if (grid_[l][c].state == Sir::q) {
                if (grid_[l][c].clock >40) {
                    end[l - 1][c - 1] = Sir::r;
                    grid_[l][c].clock = 0;
                }
                else {
                    ++grid_[l][c].clock;
                    end[l - 1][c - 1] = Sir::q;
                }
            }

        }
    }
    counter_infected();
    counter_quarantene_infected();
    ++day;
    copy_(end);
}
void Sir_model::Board::quarantene_() {
    for (int l = 1; l < dimension_; ++l) {
        for (int c = 1; c < dimension_; ++c) {
            if ((((l == quaranten.len_line || l == quaranten.len_line * 2) 
                || (c == quaranten.len_line * 2 || c == quaranten.len_line)) &&
                (l >= quaranten.len_line && l <= quaranten.len_line * 2) 
                && (c >= quaranten.len_line && c <= quaranten.len_line * 2)))
                grid_[l][c].state = Sir::bordo_q;
        }

    }
}