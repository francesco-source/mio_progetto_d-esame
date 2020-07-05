#pragma once
#include<iostream>
// struttura che definisce i vari stati possibili di una celletta.
    enum class Sir :char {
        s, i, r, q, bordo_q
    };
    // struttura che definisce i paramtri della quarantena
    struct Quarantene_parameters {
        int first_day = 0;
        int last_day = 0;
        int len_line = 0;
    };
    //struttura che definisce i parametri di ogni cella del vettore grid della classe board.
    struct Cell {
        Sir state;
        double inf_prob = 0; //Probabilità di infettarsi
        double clock;
        inline Cell() {
            state = Sir::s;
            inf_prob = 0;
            clock = 0;
        }
    };
    struct Counter {
        int num_s=0;
        int num_i=0;
        int num_r=0;
    };
    enum class Mode : int {
        Still,Move,move_plus,Quarantene_1,Quarantene_2,Quarantene_1_and_2
    };
    inline Sir operator++(Sir& hs) {
        switch (hs) {
        case Sir::s: hs = Sir::i; return hs;
        case Sir::i: hs = Sir::r; return hs;
        case Sir::q: hs = Sir::r; return hs;
        default: return hs;
        }
    };