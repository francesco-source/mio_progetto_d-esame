#pragma once
#include"board.h"
#include <SFML/Graphics.hpp>
#include<cassert>
void Sir_model::Board::draw(int& millisecondi) {
  
    float bit_size = 1.;
    //L'if di seguito setta di fatto un fattore di scala nel caso la board sia troppo piccola
   /* if (dimension_ < 100) {
        bit_size = 10.;
    }*/
    int win_size = static_cast<int>(bit_size * dimension_);
    assert(win_size > 99 && win_size < 1001);
    sf::RenderWindow window(sf::VideoMode(win_size + static_cast<int>((2 * win_size / 3)), win_size),
        "LEFT WINDOW DESEASE RIGHT-DOWN QUARANTENE  RIGHT-UP-FREE", sf::Style::Close | sf::Style::Resize);

    window.setVerticalSyncEnabled(true);
    //Setto i bit dei quadratini e i rispettivi colori
    sf::RectangleShape sus_bit(sf::Vector2f(bit_size, bit_size));
    sf::RectangleShape inf_bit(sf::Vector2f(bit_size, bit_size));
    sf::RectangleShape rec_bit(sf::Vector2f(bit_size, bit_size));
    sf::RectangleShape confine_fra_i_due_grafici(sf::Vector2f(bit_size, bit_size));
    sf::RectangleShape quarantenato(sf::Vector2f(bit_size, bit_size));

    sus_bit.setFillColor(sf::Color::Green);
    inf_bit.setFillColor(sf::Color::Red);
    rec_bit.setFillColor(sf::Color::Blue);
    confine_fra_i_due_grafici.setFillColor(sf::Color::Yellow);
    quarantenato.setFillColor(sf::Color::Black);

    std::cout << "day            " << "number_infected" << '\n';
    sf::Clock clock;
    sf::Time time;
    bool flag = true;
    while (window.isOpen())
    {
        if (day >= quaranten.first_day && flag && (int)advanced_opt>=4) {
            quarantene_();
            flag = false;
        }
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed) {
                window.close();
            }
        }
        // questo if dovrà comprendere le funzioni avanzate di fusco.La funzione evolve,move e airplane devono attivarsi a seconde dell unum che gli passiamo.
    //scendi piu' in basso a leggere per il commento con i grafici.
        time = clock.getElapsedTime();
        if (time.asMilliseconds() > millisecondi) {
            
            evolve_();
            if ((int)advanced_opt >=1) {
                move_();
            }
            if ((int)advanced_opt >=2) {
                airplane_();
            }
            clock.restart();
            std::cout << "" << day << "            " << counter.num_i << '\n';
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
                    else if (grid_[l][c].state == Sir::bordo_q) {
                        quarantenato.setPosition(bit_size * c, bit_size * l);
                        window.draw(quarantenato);
                    }
                    else if (grid_[l][c].state == Sir::q) {
                        quarantenato.setPosition(bit_size*c, bit_size * l);
                        window.draw(quarantenato);
                    }
                }
            }
            std::vector<int> punto_grafico(3);
            std::vector<int> q_point(3);
            // questo è il grafico che si attiva solo se la mia opzione( l'ultima) è attiva.
            if ((int)advanced_opt>=4) {           
                 q_point[0] = static_cast<int>(q_counter.num_s  / (quaranten.len_line));
                 q_point[1] = static_cast<int>(q_counter.num_i  / (quaranten.len_line));
                 q_point[2] = static_cast<int>(q_counter.num_r  / (quaranten.len_line));
                int proportion = dimension_ - ((quaranten.len_line * quaranten.len_line) / dimension_);
                 punto_grafico[1] = static_cast<int>((counter.num_i-q_counter.num_i)*0.5 /proportion);
                 punto_grafico[0] =static_cast<int>((counter.num_s-q_counter.num_s)*0.5/(proportion));
                 punto_grafico[2] = static_cast<int>((counter.num_r-q_counter.num_r)*0.5 /(proportion));
                grafico_in_quarantene.push_back({ q_point[0],q_point[1],q_point[2] });
                grafico_out_quarantene.push_back({ punto_grafico[0] , punto_grafico[1] , punto_grafico[2] });
                //confine fra i due grafici.
                for (int i = dimension_ + 1; i < (5 * dimension_*bit_size / 3); ++i) {
                    confine_fra_i_due_grafici.setPosition(i * bit_size, static_cast<float>(dimension_ / 2));
                    window.draw(confine_fra_i_due_grafici);
                    // liberi grafico
                    for (int counter = 0; counter != grafico_out_quarantene.size(); ++counter) {
                        if (grafico_out_quarantene[counter].num_i <= 1) {
                            inf_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_ / 2) - 3);
                            window.draw(inf_bit);
                        }
                        else {
                            inf_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ / 2 - grafico_out_quarantene[counter].num_i)-3);
                            window.draw(inf_bit);
                        }
                        if (grafico_out_quarantene[counter].num_s <= 1) {
                            sus_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_ / 2) - 1);
                            window.draw(sus_bit);
                        }
                        else {
                            sus_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ / 2 - grafico_out_quarantene[counter].num_s) + 1);
                            window.draw(sus_bit);
                        }
                        if (grafico_out_quarantene[counter].num_r <= 1) {
                            rec_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_ / 2) - 2);
                            window.draw(rec_bit);
                        }
                        else {
                            rec_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ / 2 - grafico_out_quarantene[counter].num_r) -2);
                            window.draw(rec_bit);
                        }
                        // quarantenati stato
                            if (grafico_in_quarantene[counter].num_i <= 1) {
                                inf_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_) - 2);
                                window.draw(inf_bit);
                            }
                            else {
                                inf_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_)- grafico_in_quarantene[counter].num_i + 1);
                                window.draw(inf_bit);
                            }
                            if (grafico_in_quarantene[counter].num_s <= 1) {
                                sus_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_) - 1);
                                window.draw(sus_bit);
                            }
                            else {
                                sus_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ - grafico_in_quarantene[counter].num_s+2));
                                window.draw(sus_bit);
                            }
                            if (grafico_in_quarantene[counter].num_r <= 1) {
                                rec_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_) - 1);
                                window.draw(rec_bit);
                            }
                            else {
                                rec_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ - grafico_in_quarantene[counter].num_r) + 1);
                                window.draw(rec_bit);
                            }
                        
                    }
                }
                window.display();
                if (day > 250) {
                    window.close();

                }
            }    
                // questo grafico si attiva in tutti gli altri casi.
            else {
                     punto_grafico[1] = static_cast<int>(counter.num_i / dimension_);
                     punto_grafico[0] = static_cast<int>(counter.num_s / dimension_);
                     punto_grafico[2] = static_cast<int>(counter.num_r / dimension_);
                    // grafico_out_quarantene.num_i.push_back(punto_del_grafico);
                    grafico_out_quarantene.push_back({ punto_grafico[0] , punto_grafico[1] , punto_grafico[2] });
                    for (int counter = 0; counter != grafico_out_quarantene.size(); ++counter) {
                        if (grafico_out_quarantene[counter].num_i <= 1) {
                            inf_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_) - 2);
                            window.draw(inf_bit);
                        }
                        else {
                            inf_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ - grafico_out_quarantene[counter].num_i) + 1);
                            window.draw(inf_bit);
                        }
                        if (grafico_out_quarantene[counter].num_s <= 1) {
                            sus_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_) - 1);
                            window.draw(sus_bit);
                        }
                        else {
                            sus_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ - grafico_out_quarantene[counter].num_s) + 1);
                            window.draw(sus_bit);
                        }
                        if (grafico_out_quarantene[counter].num_r <= 1) {
                            rec_bit.setPosition(static_cast<float>(dimension_ + counter), static_cast<float>(dimension_) - 1);
                            window.draw(rec_bit);
                        }
                        else {
                            rec_bit.setPosition(static_cast<float>(dimension_ + counter + 1), static_cast<float>(dimension_ - grafico_out_quarantene[counter].num_r) + 1);
                            window.draw(rec_bit);
                        }
                    }
                    window.display();
                    if (day > 150) {
                        window.close();
                    }
                }

            }
            
        }
}
