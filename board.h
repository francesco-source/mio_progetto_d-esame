#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

//Definisco una variabile per pulire la console a seconda del sistema operativo in uso
#if defined _WIN32
#define cls "cls"
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined (__APPLE__)
#define cls "clear"
#endif


enum class Sir :char {
    s = 45, i = 43, r = -80, q = 81
};

Sir& operator++(Sir& hs) {
    switch (hs) {
    case Sir::s: hs = Sir::i; return hs; //++s = i
    case Sir::i: hs = Sir::r; return hs; //++i = r
    case Sir::q: hs = Sir::r; return hs; //++q = r
    default: return hs;
    }
};

struct Cell {
    Sir state;
    double inf_prob = 0; //Probabilità di infettarsi
    double rec_prob = 0; //Probabilità di guarire
    Cell() {
        state = Sir::s;
        inf_prob = 0;
        rec_prob = 0;
    }
};

class Board {
    std::vector<std::vector<Cell>> grid_;
    int dimension_;
    double beta_;
    double gamma_;
    int day;
    bool adv_opt;
    int number_infected = 0;//Nuova variabile per attivare opzioni avanzate
public:
    Board(int n, double b, double y, bool f) : grid_(n + 2, std::vector<Cell>(n + 2)), adv_opt{ f }{
        //Verifico la coerenza dei dati
        assert(b > 0 && b < 1);
        assert(y > 0 && y < 1);
        assert(n > 3);

        beta_ = b;
        gamma_ = y;
        dimension_ = n + 2;
        day = 0;
    };
    Sir& operator()(int riga, int colonna) {
        return (grid_[riga + 1][colonna + 1].state);
    }
    void print_() {
        std::cout << "Day " << day << '\n';
        for (int l = 0; l <= dimension_ - 1; ++l) {
            for (int c = 0; c <= dimension_ - 1; ++c) {
                if (c == 0 || l == 0 || l == dimension_ - 1 || c == dimension_ - 1) {
                    std::cout << '#';       //stampa il bordo
                }
                else {
                    std::cout << static_cast<char>(grid_[l][c].state);
                }
            }
            std::cout << '\n';
        }
    }
    void copy_(std::vector<std::vector<Sir>>& end, bool choose_copy) {
        if (choose_copy) {
            for (int l = 1; l <= dimension_ - 1; ++l) {
                for (int c = 0; c <= dimension_ - 1; ++c) {
                    if (l == 0 || c == 0 || l == dimension_ - 1 || c == dimension_ - 1)
                    {
                        grid_[l][c].state = Sir::s;
                    }
                    else {
                        grid_[l][c].state = end[l - 1][c - 1];
                    }
                }
            }
        }

    }
    void quarantene_() {
        for (int i = 50; i < 80; ++i) {
            for (int j = 50; j < 80; ++j)
                grid_[i][j].state = Sir::q;
        }
    }
    //Funzione per muovere le celle
    void move_() {
        for (int l = 1; l < dimension_ - 1; ++l) {
            for (int c = 1; c < dimension_ - 1; ++c) {
                if (grid_[l][c].state != Sir::q) {
                    int swap; // indicatore, se ha valore da 0 a 7 fa scambiare la cella con una delle 8 adiacenti
                    int newc, newl; //indici della cella da scambiare
                    if (day < 8)
                        swap = (rand() + time(0)) % 8;
                    else
                        swap = (rand() + time(0)) % day; //col passare dei giorni diminuisce la probabilit� che una cella si muova( non mi piace da migliorare).
                    switch (swap) {
                    case 0: newc = c - 1;   newl = l - 1;   break;
                    case 1: newc = c;       newl = l - 1;   break;
                    case 2: newc = c + 1;   newl = l - 1;   break;
                    case 3: newc = c - 1;   newl = l;       break;
                    case 4: newc = c + 1;   newl = l;       break;
                    case 5: newc = c - 1;   newl = l + 1;   break;
                    case 6: newc = c;       newl = l + 1;   break;
                    case 7: newc = c + 1;   newl = l + 1;   break;
                    default: return;
                    }
                    if (newc == 0)
                        ++newc;
                    if (newl == 0)
                        ++newl;
                    if (newc == dimension_ - 1)
                        --newc;
                    if (newl == dimension_ - 1)
                        --newl;
                    assert(newc > 0 && newc < dimension_ - 1 && newl>0 && newl < dimension_ - 1);
                    if (grid_[newl][newc].state != Sir::q) {
                        auto old = new Cell;
                        *old = grid_[l][c];
                        grid_[l][c].state = grid_[newl][newc].state;
                        grid_[newl][newc] = *old;
                        delete old;
                    }
                }
            }

        }
    }

    //Funzione per evolvere di un giorno la tabella.Questo è il cuore del programma
    void evolve_() {
        std::vector<std::vector<Sir>> end(dimension_ - 2, std::vector<Sir>(dimension_ - 2));
        number_infected = 0;
        for (int m = 1; m < dimension_ - 1; ++m) {//conta il numero di infetti in ogni vettore grid_
            for (int n = 1; n < dimension_ - 1; ++n) {
                if (grid_[m][n].state == Sir::i || grid_[m][n].state == Sir::r) {
                    ++number_infected;
                }
            }
        }
        for (int l = 1; l < dimension_ - 1; ++l) {
            for (int c = 1; c < dimension_ - 1; ++c) {
                if (grid_[l][c].state == Sir::s) {
                    for (int j = -1; j <= 1; ++j) {
                        for (int i = -1; i <= 1; ++i) {
                            if (grid_[l + j][c + i].state == Sir::i && grid_[l + j][c + i].state != Sir::s) {
                                grid_[l][c].inf_prob += beta_; //somma delle probabilita di infettarsi attorno alla cella.
                            }
                            else {
                                grid_[l][c].inf_prob += 0;
                            }
                        }
                    }
                    if (grid_[l][c].inf_prob >= 1) {
                        end[l - 1][c - 1] = Sir::i;
                        grid_[l][c].inf_prob = 0;
                    }
                    else {
                        end[l - 1][c - 1] = Sir::s;
                    }
                }
                else if (grid_[l][c].state == Sir::i) {
                    int teoretical_rescued = (dimension_ - 2) * (dimension_ - 2) * beta_ * gamma_;
                    //int num = (dimension_*dimension_* gamma_);
                    int num = 9;
                    int factor = (rand() + time(0)) % num;
                    if (factor == num - 1 && day > 6) { //factor e days li ho messi liberamente uguali a questi valori && (number_infected * gamma_ < teoretical_rescued)
                        end[l - 1][c - 1] = Sir::r;
                    }
                    else {
                        end[l - 1][c - 1] = Sir::i;
                    }
                }
                else if (grid_[l][c].state == Sir::r) {
                    end[l - 1][c - 1] = Sir::r;
                }
                else {
                    if (day < 90) {
                        end[l - 1][c - 1] = Sir::q;
                    }

                    else {
                        end[l - 1][c - 1] = Sir::s;
                    }
                }
            }

        }
        ++day;
        bool vero = true;
        copy_(end, vero);
    }

    void airplane_() {
        for (int l = 1; l < dimension_; ++l) {
            for (int c = 1; c < dimension_; ++c) {
                int colonna = (rand() + time(0)) % dimension_;
                int riga = (rand() + time(0)) % dimension_;
                if (grid_[l][c].state != Sir::q && grid_[colonna][riga].state != Sir::q) {

                    if (riga == 1) {
                        auto sposto = new Cell();
                        sposto->state = grid_[colonna][riga].state;
                        sposto->inf_prob = grid_[colonna][riga].inf_prob;
                        sposto->rec_prob = grid_[colonna][riga].rec_prob;
                        grid_[colonna][riga].state = grid_[l][c].state;
                        grid_[colonna][riga].inf_prob = grid_[l][c].inf_prob;
                        grid_[colonna][riga].rec_prob = grid_[l][c].rec_prob;
                        grid_[l][c].state = sposto->state;
                        grid_[l][c].inf_prob = sposto->inf_prob;
                        grid_[l][c].rec_prob = sposto->rec_prob;
                        delete sposto;
                    }
                }
            }
        }
    }
    //Funzione per avare la grafica
    void draw() {
        float bit_size = 1.;
        //L'if di seguito setta di fatto un fattore di scala nel caso la board sia troppo piccola
        if (dimension_ < 100) {
            bit_size = 10.;
        }
        auto win_size = bit_size * dimension_;
        assert(win_size > 99 && win_size < 1001);
        sf::RenderWindow window(sf::VideoMode(win_size, win_size), "Disease BETA", sf::Style::Close | sf::Style::Resize); //Creo una finestra (ogni cella = BIT_SIZE px)
        //Attivo il VSync (Max framerate = framerate del monitor utilizzato)
        window.setVerticalSyncEnabled(true);
        //Setto i bit dei quadratini e i rispettivi colori
        sf::RectangleShape sus_bit(sf::Vector2f(bit_size, bit_size));
        sf::RectangleShape inf_bit(sf::Vector2f(bit_size, bit_size));
        sf::RectangleShape rec_bit(sf::Vector2f(bit_size, bit_size));
        sf::RectangleShape q_bit(sf::Vector2f(bit_size, bit_size));

        sus_bit.setFillColor(sf::Color::Green);
        inf_bit.setFillColor(sf::Color::Red);
        rec_bit.setFillColor(sf::Color::Blue);
        q_bit.setFillColor(sf::Color::Yellow);
        int buleano = 1;
        while (window.isOpen()) {
            if (day > 10 && buleano < 90) {
                ++buleano;
                quarantene_();
            }
            //Gestione eventi
            sf::Event evnt;
            while (window.pollEvent(evnt)) {
                //Se clicco la X chiude la finestra
                if (evnt.type == sf::Event::Closed) {
                    window.close();
                }
            }

            std::cout << "Day " << day << "  number of infected = " << number_infected << '\n';
            evolve_();
            if (adv_opt) {
                move_();
                airplane_();
            }
            for (int l = 1; l < dimension_ - 1; ++l) {
                for (int c = 1; c < dimension_ - 1; ++c) {
                    //Stampa le celle
                    if (grid_[l][c].state == Sir::s) {
                        sus_bit.setPosition(bit_size * c, bit_size * l);
                        window.draw(sus_bit);
                    }
                    else if (grid_[l][c].state == Sir::i) {
                        inf_bit.setPosition(bit_size * c, bit_size * l);
                        window.draw(inf_bit);
                    }
                    else if (grid_[l][c].state == Sir::r) {
                        rec_bit.setPosition(bit_size * c, bit_size * l);
                        window.draw(rec_bit);
                    }
                    else if (grid_[l][c].state == Sir::q) {
                        q_bit.setPosition(bit_size * c, bit_size * l);
                        window.draw(q_bit);
                    }
                }
            }
            window.display();
        }
    }
};
#endif
