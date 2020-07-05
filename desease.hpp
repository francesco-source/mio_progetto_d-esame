#pragma once

#include <vector>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

namespace desease {
    int constexpr EDGE = 20;

    //Struct per indicare lo stato del "sistema" composto dai tre tipi s, i, r
    //Gli assert impongono valori positivi essendo una popolazione
    struct State {
        double s;
        double i;
        double r;

        State(double sus, double inf, double rec) : s{ sus }, i{ inf }, r{ rec } {
            assert(s > 0 || s == 0);
            assert(i > 0 || i == 0);
            assert(r > 0 || r == 0);
        };
        State() {
            s = 0.;
            i = 0.;
            r = 0.;
        }
    };
    class Disease {
    private:

        std::string name_;
        std::vector<State> state_;
        double tot_;
        double gamma_;
        double beta_;

        auto evolve_(State const& begin) {
            auto end = State();

            //La funzione dei suscettibili è sempre decrescente, mi limito a metterla uguale a 0 quando, per errori di approssimazione, risulta <0

            end.s = begin.s - beta_ * begin.i * begin.s;
            if (end.s < 0) {
                end.s = 0;
            }

            //Calcolo prima i recoverd in quanto sono sicuro non sforino (al massimo se gamma = 1 diventano gli infetti)

            end.r = begin.r + gamma_ * begin.i;
            if (end.r > tot_) {
                end.r = tot_;
            }

            //Gli infetti calcolati in esubero risultano semplicemente I = N - S - R

            end.i = begin.i + beta_ * begin.i * begin.s - gamma_ * begin.i;
            if (end.i > tot_) {
                end.i = tot_ - end.r - end.s;
            }
            //Verificare che s+i+r=n
            //Verificare la valòidità dei dati
            assert(end.s > 0 || end.s == 0);
            assert(end.i > 0 || end.i == 0);
            assert(end.r > 0 || end.r == 0);

            return end;
        };

    public:

        Disease(std::string p, int n, double  b, double y) : name_{ p } {
            //Verifico che i dati inseriti siano coerenti
            assert(b > 0 && b < 1);
            assert(y > 0 && y < 1);
            assert(n > 1); //Non ha senso un modello con una persona sola (in generale con poche, ma come definire "poche"?)

            tot_ = n;
            beta_ = b / tot_;
            gamma_ = y;
            State state_0{ (double)n - 1., 1., 0. };
            state_.push_back(state_0);
        }

        //Funzione per cambiare il valore di beta
        void setBeta(double b) {
            beta_ = b / tot_;
        }

        //Funzione per cambiare il valore di gamma
        void setGamma(double g) {
            gamma_ = g;
        }

        //Evolvo la malattia per n giorni
        void evolve(int n) {
            for (int i = 0; i < n; ++i) {
                state_.push_back(evolve_(state_[i]));
            }
        }

        //Stampo un report di tutti i giorni (calcolati)
        void print() {
            int i = 0;
            auto tab = std::setw(20);

            std::cout << name_ << '\n';
            std::cout << "Current value of R0: " << (tot_ * beta_) / gamma_ << '\n';
            std::cout << tab << "Day" << tab << "Susceptible" << tab << "Infectuos" << tab << "Recovered" << '\n';
            for (auto const it : state_) {
                std::cout << std::setprecision(10) << tab << i << tab << (int)it.s << tab << (int)it.i << tab << (int)it.r << '\n';
                ++i;
            }
        }

        //Stampa su file un report di tutti i giorni (calcolati)
        void f_print() {

            //Sposto l'output su file
            std::ofstream fp;
            fp.open("report.txt");
            std::cout.rdbuf(fp.rdbuf());

            print();
            fp.close();
        }

        //Disegan il grafico
        void draw(int lenght, int height, const char& c) {
            sf::RenderWindow window1(sf::VideoMode(lenght, height), name_, sf::Style::Close | sf::Style::Resize);
            window1.setVerticalSyncEnabled(true);
            sf::RectangleShape x_axis(sf::Vector2f(static_cast<float>(lenght), 1.));
            sf::RectangleShape y_axis(sf::Vector2f(1.,static_cast<float>(lenght)));
            x_axis.setPosition(0, static_cast<float>(height - EDGE));
            y_axis.setPosition(EDGE, 0);
            x_axis.setFillColor(sf::Color::Black);
            y_axis.setFillColor(sf::Color::Black);

            //Settiamo ora le variabili per mettere tutto in "scala"
            auto x_up = (lenght - EDGE) / state_.size();
            auto y_scale = (height - 2 *static_cast<float>( EDGE)) / tot_;

            sf::RectangleShape bit(sf::Vector2f(2., 2.));
          

            while (window1.isOpen()) {
                window1.clear(sf::Color::White);
                window1.draw(x_axis);
                window1.draw(y_axis);

                //Gestione eventi
                sf::Event evnt;
                while (window1.pollEvent(evnt)) {
                    //Se clicco la X chiude la finestra
                    if (evnt.type == sf::Event::Closed) {
                        window1.close();
                    }
                }
                double j;
                switch (c) {
                case 's': case 'S':
                    j = EDGE;
                    bit.setFillColor(sf::Color::Green);
                    for (auto const it : state_) {
                        bit.setPosition(static_cast<float>(j), height - EDGE - static_cast<float>(it.s * y_scale));
                        window1.draw(bit);
                        j += x_up;
                    }
                    break;
                case 'i': case 'I':
                    j = EDGE;
                    bit.setFillColor(sf::Color::Red);
                    for (auto const it : state_) {
                        bit.setPosition(static_cast<float>(j), height - EDGE - static_cast<float>(it.i * y_scale));
                        window1.draw(bit);
                        j += x_up;
                    }
                    break;
                case 'r': case 'R':
                    j = EDGE;
                    bit.setFillColor(sf::Color::Blue);
                    for (auto const it : state_) {
                        bit.setPosition(static_cast<float>(j), height - EDGE - static_cast<float>(it.r * y_scale));
                        window1.draw(bit);
                        j += x_up;
                    }
                    break;
                case 'a': case 'A': //Stampo tutti i case a=all
                    j = EDGE;
                    bit.setFillColor(sf::Color::Green);
                    for (auto const it : state_) {
                        bit.setPosition(static_cast<float>(j),static_cast<float>( height - EDGE - it.s * y_scale));
                        window1.draw(bit);
                        j += x_up;
                    }
                    j = EDGE;
                    bit.setFillColor(sf::Color::Red);
                    for (auto const it : state_) {
                        bit.setPosition(static_cast<float>(j),static_cast<float>( height - EDGE - it.i * y_scale));
                        window1.draw(bit);
                        j += x_up;
                    }
                    j = EDGE;
                    bit.setFillColor(sf::Color::Blue);
                    for (auto const it : state_) {
                        bit.setPosition(static_cast<float>(j),static_cast<float>( height - EDGE - it.r * y_scale));
                        window1.draw(bit);
                        j += x_up;
                    }
                    break;
                default:
                    throw std::runtime_error("Warning: invalid character passed at the draw function\n");
                }

                window1.display();
            }
           
        }
    };
}
