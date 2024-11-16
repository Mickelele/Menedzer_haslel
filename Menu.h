#include <fmt/ranges.h>


//========================MENU=========================

/**
 * MENU1
 */

void WyswietlMenu() {
    std::cout << "======== MENU ========" << std::endl;
    fmt::print("1. Wybierz plik zrodlowy.\n"); //PODAJ NAZWE
    fmt::print("2. Podaj bezposrednia sciezke do pliku.\n"); //PODAJ SCIEZKE
    fmt::print("3. Utworz nowy plik.\n"); //TWORZY NOWY PLIK
    fmt::print("0. ZAKONCZ.\n"); //TWORZY NOWY PLIK
    std::cout << "======================" << std::endl;
    std::cout << "Wybierz opcje: \n";
}
//=====================================================


//=======================MENU 2========================

/**
 * MENU 2
 */

void WyswietlMenu2() {
    std::cout << "======== MENU ========" << std::endl;
    fmt::print("1. Wyszukaj hasla.\n");
    fmt::print("2. Posortuj hasla.\n");
    fmt::print("3. Dodaj haslo.\n");
    fmt::print("4. Edytuj haslo.\n");
    fmt::print("5. Usun haslo.\n");
    fmt::print("6. Dodaj kategorie.\n");
    fmt::print("7. Usun kategorie.\n");
    fmt::print("8. Zmien plik.\n");
    fmt::print("0. ZAKONCZ.\n");
    std::cout << "======================" << std::endl;
    std::cout << "Wybierz opcje: \n";
}

//=====================================================