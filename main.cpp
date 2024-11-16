#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fmt/ranges.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <map>
#include <random>

#include "User.h"
#include "Go.h"
#include "Menu.h"



int main() {


    //=========================================================================
/**
* Funkcja główna programu.
* Interakcja i menu programu.
* @return Wartość 0 oznaczająca poprawne zakończenie programu.
*/




        bool koniec = false;
        Go g;

//=========================================================================

/**
* Powrot do menu poczatkowego
 * @WyswietlMenu
*/
        beg:

        WyswietlMenu();

        beg2:

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        std::string curPass;

        auto file = g.getFilePath(choice);

//=========================================================================

/**
* Wybór opcji w menu początkowym
*/
        if (choice == 1) {

            fmt::print("Wpisz haslo do pliku: \n");
            std::cin >> curPass;
            goto menu;

        } else if (choice == 2) {

            fmt::print("Wpisz haslo do pliku: \n");
            std::cin >> curPass;
            goto menu;

        } else if (choice == 3) {

            fmt::print("Ustaw haslo do pliku: \n");
            std::cin >> curPass;
            g.encrypt(file, curPass);
            goto menu;
        } else if (choice == 0) {
            return 0;
        } else {
            fmt::print("Podano zly numer. Wpisz ponownie: \n");
            goto beg2;
        }

//=========================================================================

/**
* Etykieta menu głównego
*/
        menu:
        while (!koniec) {
/**
*Wyświetlenie menu głównego
* @WyswietlMenu2
*/
            WyswietlMenu2();
            int choice2;
            std::cin >> choice2;

/**
* Wybór opcji w menu głównym
 * @wyszukajHaslo
 * @posortujHasla
 * @dodajHaslo
 * @edytujHaslo
 * @usunHaslo
 * @dodajKategorie
 * @usunKategorie
*/
            switch (choice2) {
                case 1:
                    g.wyszukajHaslo(file, curPass);
                    break;
                case 2:
                    g.posortujHasla(file, curPass);
                    break;
                case 3:
                    g.dodajHaslo(file, curPass);
                    break;
                case 4: {
                    fmt::print("Wpisz kategorie hasla, ktore chcesz edytowac:\n");
                    std::string ckat;
                    std::cin.ignore();
                    std::getline(std::cin, ckat);

                    fmt::print("Wpisz nazwe hasla, ktore chcesz edytowac:\n");
                    std::string cnaz;
                    std::getline(std::cin, cnaz);

                    fmt::print("Wpisz login hasla, ktore chcesz edytowac:\n");
                    std::string clog;
                    std::getline(std::cin, clog);

                    g.edytujHaslo(file, curPass, ckat, cnaz, clog);
                }
                    break;
                case 5:
                    g.usunHaslo(file, curPass);
                    break;
                case 6:
                    g.dodajKategorie(file, curPass);
                    break;
                case 7:
                    g.usunKategorie(file, curPass);
                    break;
                case 8: {
                    goto beg;
                }
                case 0: {
                    return 0;
                }
            }


        }


        return 0;



}