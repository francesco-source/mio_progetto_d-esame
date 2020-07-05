#pragma once
#include"board.h"
#include<ctime>
void Sir_model::Board::airplane_() {
    //creo una funzione lambda che mi creerà una cella sullo hype per permettermi di muoverle.
    auto create_hype_cell = [&](int l, int c, int colonna, int riga){
        std::unique_ptr<Cell> sposto = std::make_unique<Cell>();
        *sposto = grid_[l][c];
        sposto->state = grid_[colonna][riga].state;
        sposto->inf_prob = grid_[colonna][riga].inf_prob;
        grid_[colonna][riga].state = grid_[l][c].state;
        grid_[colonna][riga].inf_prob = grid_[l][c].inf_prob;
        grid_[l][c] = *sposto;
    };
    //se ho l'opzione avanzata della quarantena di Pivi allora ho opzioni avanzate.Fusco da implementare l'enum deve aggiungere qui l opzione 5.
    if (day > quaranten.first_day && day < quaranten.last_day && (int)advanced_opt>=4) {
        for (int l = 1; l < dimension_; ++l) {
            for (int c = 1; c < dimension_; ++c) {
                if (((l < quaranten.len_line || l > quaranten.len_line * 2) || (c < quaranten.len_line || c > quaranten.len_line * 2))) {
                    int colonna = (rand() + time(0)) % dimension_;
                    int riga = (rand() + time(0)) % dimension_;
                    // ci ho messo 3 ore per fare questo if. Non toccatelo please ve lo spiego in chiamata.
                    if (((grid_[l][c].state !=Sir::bordo_q && grid_[colonna][riga].state != Sir::bordo_q)||
                        (grid_[l][c].state!=Sir::q && grid_[colonna][riga].state != Sir::q))
                        && ((riga <  quaranten.len_line || riga > quaranten.len_line * 2) ||
                            (colonna < quaranten.len_line || colonna > quaranten.len_line * 2))) {
                        if (riga == 1) {
                            create_hype_cell(l, c, colonna, riga);
                        }
                    }
                }
            }
        }
    }
    // se non ci sono le opzioni avanzate di PIvi.
    else {
        for (int l = 1; l < dimension_; ++l) {
            for (int c = 1; c < dimension_; ++c) {
                int colonna = (rand() + time(0)) % dimension_;
                int riga = (rand() + time(0)) % dimension_;
                if (grid_[l][c].state != Sir::bordo_q && grid_[colonna][riga].state != Sir::bordo_q) {
                    if (riga == 1) {
                        create_hype_cell(l, c, colonna, riga);
                    }
                }
            }
        }
    }
}
//permette alle celle di muoversi una adiacente all'altra.Simula gli spostamenti
//giornalieri di prima necessità di ognuno di noi
void Sir_model::Board::move_() {
    for (int line = 1; line < dimension_ - 2; ++line) {
        for (int colonna = 1; colonna < dimension_ - 2; ++colonna) {
            int cambio_linea = (rand() + time(0)) % 3;
            int cambio_colonna = (rand() + time(0)) % 3;//ricorda il meno due;
            int uno_su_tre = (rand() + time(0)) % 9;
            if (uno_su_tre == 3 && ((grid_[line][colonna].state != (Sir::bordo_q) &&
                grid_[line + cambio_linea - 1][colonna + cambio_colonna - 1].state != (Sir::bordo_q)) &&
                grid_[line][colonna].state != Sir::q && grid_[line + cambio_linea - 1][colonna + cambio_colonna - 1].state != Sir::q)) {
                std::unique_ptr<Cell> old = std::make_unique<Cell>();
                *old = grid_[line][colonna];
                grid_[line][colonna] = grid_[line + cambio_linea - 1][colonna + cambio_colonna - 1];
                grid_[line + cambio_linea - 1][colonna + cambio_colonna - 1] = *old;
            }
        }
    }
}
