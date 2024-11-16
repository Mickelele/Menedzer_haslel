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

struct Go{

    Go() {
    }


    /**
     * STWORZENIE 2 MAP KTORE NIE SA SORTOWANE I ICH KLUCZE MOGA SIE POWTARZAC
     */
    std::unordered_multimap<std::string, User> map = {};
    std::unordered_multimap<std::string, User> curMap = {};

    Go(const std::unordered_multimap <std::string, User> &map) : map(map) {}



    /**
     * Szyforwanie pliku
     *
     * Def:
     * Funkcja encrypt otwiera plik o podanej nazwie, a następnie szyfruje
     * jego zawartość na podstawie podanego hasła. Zaszyfrowana treść jest
     * zapisywana z powrotem do pliku, zastępując pierwotną zawartość pliku.
     *
     *
     * @param fileName
     * @param password
     */
    void encrypt(const std::string& fileName, const std::string& password) {
        std::ifstream file(fileName);

        if (!file) {
            fmt::print("Plik nie odczytywalny");
            return;
        }

        std::string encryptedContent;
        char c;

        encryptedContent += "#ENCRYPTED#\n";

        while (file.get(c)) {
            if (c == '\n' || c == ' ' || c == '\r' || c == '\t') {
                encryptedContent += c;  // Dodaj znak nowej linii
            } else {
                int przesuniecie = password[encryptedContent.length() % password.length()] - 33; //OBLICZANIE PRZESUNIECIA
                c = (c + przesuniecie - 33 + 90) % 90 + 33; //PRZESUNIECIE (w zakresie 33-122)
                encryptedContent += c;
            }
        }
        file.close();

        std::ofstream fileOUT(fileName); //URUCHOMIENIE PLIKU
        fileOUT << encryptedContent; //ZAPISANIE DO PLIKU
        fileOUT.close();
    }


    /**
     * DeSzyfrowanie pliku
     *
     * Def:
     * Funkcja decrypt otwiera plik o podanej nazwie i deszyfruje jego
     * zawartość na podstawie podanego hasła. Odszyfrowana treść jest zapisywana
     * z powrotem do pliku, zastępując pierwotną zawartość.
     *
     *
     * @param fileName
     * @param password
     */
    void decrypt(const std::string& fileName, const std::string& password) {
        std::ifstream file(fileName);

        if (!file) {
            fmt::print("Plik nie odczytywalny");
            return;
        }

        std::string decryptedContent;
        char c;

        std::string line;
        std::getline(file, line); // Odczytaj pierwsza linie pliku
        if (line != "#ENCRYPTED#") {
            fmt::print("Nie można odszyfrować danych. Plik nie został zaszyfrowany.\n");
            return;
        }

        while (file.get(c)) {
            if (c == '\n' || c == ' ' || c == '\r' || c == '\t') {
                decryptedContent += c;  // Dodaje znak specjalny jezeli taki wystepuje
            } else {
                int przesuniecie = password[decryptedContent.length() % password.length()] - 33; //OBLICZENIE PRZESUNIECIA
                c = (c - przesuniecie - 33 + 90) % 90 + 33; //PRZESUNIECIE (w zakresie 33-122)
                decryptedContent += c;
            }
        }

        file.close();


        std::string timestamp = getTimeStamp(); //POBRANIE TIMESTAMPA



        std::ofstream fileOUT(fileName); //URUCHOMIENIE PLIKU
        fileOUT << decryptedContent; //ZAPISANIE DO PLIKU
        //fileOUT << timestamp;
        fileOUT.close();

    }


    /**
     * Pozyskanie sciezki do pliku
     *
     * Def:
     * Funkcja getFilePath pobiera wybór użytkownika dotyczący sposobu określenia
     * ścieżki pliku, a następnie zwraca ostateczną ścieżkę pliku na podstawie
     * tego wyboru. Wybór może obejmować podanie nazwy pliku w bieżącym katalogu,
     * podanie pełnej ścieżki do pliku lub utworzenie nowego pliku o podanej nazwie
     * w katalogu nadrzędnym.
     *
     *
     * @param choice
     * @return
     */
    std::string getFilePath(int choice){


        std::string fileName;
        std::string filePath;
        std::string curPath;


        if(choice == 1)
        {
            std::filesystem::path curPath1 = std::filesystem::current_path(); //sciezka obecna
            std::filesystem::path filePath1 = curPath1.parent_path(); //sciezka rodzica
            for (const auto& f : std::filesystem::directory_iterator(filePath1)) {
                if (std::filesystem::is_regular_file(f) && f.path().extension() == ".txt") //wyswietlenie wszystkich plikow o roszerzeniu .txt
                    std::cout << f.path().filename() << std::endl;
            }

            fmt::print("Podaj nazwe pliku zrodlowego\n");
            std::getline(std::cin, fileName);
            filePath = filePath1.string() + "/" + fileName;

        } else if (choice == 2)
        {
            fmt::print("Podaj sciezke do pliku zrodlowego\n");
            std::getline(std::cin, filePath);
        } else if (choice == 3){
            std::string nazwa;
            std::cout << "Podaj nazwe pliku: \n";
            std::getline(std::cin, nazwa);

            std::string nazwaZRoszerzeniem = nazwa + ".txt";

            std::filesystem::path curPath2 = std::filesystem::current_path(); //sciezka obecna
            std::filesystem::path filePath2 = curPath2.parent_path(); //sciezka rodzica
            filePath = filePath2.string() + "/" + nazwaZRoszerzeniem;

            std::ofstream file(filePath);
            if (!file) {
                std::cout << "Nie udalo sie utworzyc pliku.\n" << std::endl;
                return 0;
            }
            file.close();

            fmt::print("{}\n",filePath);
        } else if(choice == 3){
            fmt::print("Niepoprawny wybor.\n");
            return 0;
        } else {}


        return filePath;

    }



    /**
     * TimeStamp
     *
     * Def:
     * Funkcja getTimeStamp pobiera aktualny czas systemowy i tworzy timestamp
     * w formacie "TS-hh-mm-ss", reprezentujący godzinę, minutę i sekundę.
     * Timestamp jest zwracany jako wartość typu std::string.
     *
     *
     * @return
     */
    std::string getTimeStamp() {
        std::time_t curr = std::time(nullptr);
        std::tm* aktualnyCzas = std::localtime(&curr); //POBRANIE AKTUALNEGO CZASU
        int h = aktualnyCzas->tm_hour; //GODZINY
        int min = aktualnyCzas->tm_min; //MINUTY
        int s = aktualnyCzas->tm_sec; //SEKUNDY

        std::string hh = std::to_string(h); //GODZINY DO STRINGA
        std::string mm = std::to_string(min); //MINUTY DO STRINGA
        std::string ss = std::to_string(s); //SEKUNDY DO STRINGA

        // Formatowanie timestampu: hh:mm:ss
        std::string timestamp = "TS" + hh +"-"+ mm+"-"+ss;

        return timestamp;
    }



    /**
     * Przepisanie pliku do mapy.
     *
     * Def:
     * Funkcja ZczytanieZpliku odczytuje zawartość pliku o podanej nazwie i przetwarza
     * go w celu utworzenia mapy danych użytkowników. Każda linia pliku reprezentuje
     * informacje o jednym użytkowniku, a funkcja tworzy obiekty użytkowników na podstawie
     * tych informacji i dodaje je do mapy, gdzie kluczem jest kategoria. Ostatecznie,
     * funkcja zwraca utworzoną mapę danych użytkowników.
     *
     *
     * @param fileName
     * @return
     */
    auto ZczytanieZpliku(const std::string& fileName) {
        std::ifstream file(fileName);
        std::vector<std::string> curVector = {};

        std::string nazwa = "";
        std::string haslo = "";
        std::string kategoria = "";
        std::string login = "";
        std::string strona = "";


        //PRZEJSCIE PO PLIKU LINIA PO LINI I DODAWANIE DO WEKTORA ELEMENTOW
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                curVector.push_back(line);
            } else {
                if (!curVector.empty()) {
                    for (int i = 0; i < curVector.size(); i++) {
                        switch (i) {
                            case 0:
                                kategoria = curVector.at(0);
                                break;
                            case 1:
                                nazwa = curVector.at(1);
                                break;
                            case 2:
                                haslo = curVector.at(2);
                                break;
                            case 3:
                                login = curVector.at(3);
                                break;
                            case 4:
                                strona = curVector.at(4);
                                break;
                        }
                    }
                //TWORZENIE USERA O DANYCH Z WEKTORA
                    auto user = User(nazwa, haslo, login, strona);
                    map.insert(std::pair<std::string, User>(kategoria, user));
                    curVector.clear();
                }
            }
        }

        //OBSLUGA OSTATNICH LINI W PLIKU (uzywane jezeli w ostatniej lini jest znak nowej lini)

        if (!curVector.empty()) {
            for (int i = 0; i < curVector.size(); i++) {
                switch (i) {
                    case 0:
                        kategoria = curVector.at(0);
                        break;
                    case 1:
                        nazwa = curVector.at(1);
                        break;
                    case 2:
                        haslo = curVector.at(2);
                        break;
                    case 3:
                        login = curVector.at(3);
                        break;
                    case 4:
                        strona = curVector.at(4);
                        break;
                }
            }

            //TWORZENIE NOWEGO USERA ELEMENTAMI Z WEKTORA
            auto user = User(nazwa, haslo, login, strona);
            map.insert(std::pair<std::string, User>(kategoria, user));
            curVector.clear();
        }



        file.close();
        return map;
    }



    /**
     * Zapisanie mapy do pliku.
     *
     * Def:
     * Funkcja zapiszMapeDoPliku zapisuje zawartość mapy danych użytkowników do pliku
     * o podanej nazwie. Każdy element mapy reprezentuje informacje o jednym użytkowniku,
     * a funkcja zapisuje te informacje w odpowiednim formacie do pliku tekstowego,
     * oddzielając poszczególne pola nową linią i dodając puste linie między poszczególnymi
     * użytkownikami.
     *
     *
     * @param map
     * @param fileName
     */
    void zapiszMapeDoPliku(const std::unordered_multimap<std::string, User>& map, const std::string& fileName) {
        std::ofstream file(fileName);


        //SPRAWDZENIE PLIKU (CZY ODCZYTYWALNY)
        if (!file) {
            fmt::print("Nie można otworzyć pliku do zapisu");
            return;
        }


        //ITERACJA PO MAPIE
        for (const auto& pair : map) {
            const std::string& kategoria = pair.first;
            const User& user = pair.second;


            //DOPISANIE DO PLIKU DANYCH Z MAPY
            file << kategoria << '\n';
            file << user.getNazwa() << '\n';
            file << user.getHaslo() << '\n';
            if (!user.getLogin().empty())
                file << user.getLogin() << '\n';
            if (!user.getStrona().empty())
                file << user.getStrona() << '\n';
            file << '\n';
        }

        file.close();
    }



    /**
     * Wyszukanie hasla.
     *
     * Funkcja wykonuje dekodowanie pliku, odczytuje zawartość pliku,
     * porównuje kryterium wyszukiwania z odpowiednimi polami danych
     * i tworzy nową mapę zawierającą pasujące hasła. Na koniec,
     * mapa jest zaszyfrowana i wypisywana na ekranie, a wszystkie
     * tymczasowe kontenery są czyszczone. W przypadku nieprawidłowej
     * wartości wyboru, użytkownik zostaje poproszony o ponowne wprowadzenie
     * poprawnej wartości.
     *
     *
     * @param fileName
     * @param password
     * @return
     */
    auto wyszukajHaslo(const std::string& fileName, const std::string& password) {

        b:


        //WYBRANIE OPCJI WYSZUKIWANIA
        fmt::print("1. Wyszukaj po loginie.\n");
        fmt::print("2. Wyszukaj po kategorii.\n");
        fmt::print("3. Wyszukaj po nazwie.\n");
        fmt::print("Wybor: \n");
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        std::string kryterium;
        User user{};


        //OBSLUGA WYBORU
        switch(choice)
        {
            case 1: {
                fmt::print("Wpisz login, ktory chcesz wyszukac.\n");
                std::getline(std::cin, kryterium);
                decrypt(fileName, password); //DESZYFROWANIE
                ZczytanieZpliku(fileName); //ZCZYTANIE Z PLIKU
                for (const auto& pair : map) {
                    std::string kategoria;
                    const std::string& key = pair.first;
                    const User& value = pair.second;
                    if(value.getLogin() == kryterium)
                    {
                        kategoria = key;
                        user.nazwa = value.getNazwa();
                        user.login = value.getLogin();
                        user.haslo = value.getHaslo();
                        user.strona = value.getStrona();
                        curMap.insert(std::pair<std::string, User>(kategoria,user));
                    }
                }
                encrypt(fileName, password); //SZYFROWANIE
                wypiszMape(curMap); //WYPISANIE MAPY O DANYCH KRYTERIACH
                map.clear();
                curMap.clear();
            } break;

            case 2: {

                fmt::print("Wpisz kategorie, po ktorej chcesz wyszukac.\n");
                std::getline(std::cin, kryterium);
                decrypt(fileName, password); //DESZYFROWANIE
                ZczytanieZpliku(fileName); //ZCZYTANIE Z PLIKU

                std::string kategoria;

                for (const auto& pair : map) {
                    const std::string& key = pair.first;
                    const User& value = pair.second;
                    if (key == kryterium)
                    {
                        kategoria = key;
                        user.nazwa = value.getNazwa();
                        user.login = value.getLogin();
                        user.haslo = value.getHaslo();
                        user.strona = value.getStrona();
                        curMap.insert(std::pair<std::string, User>(kategoria,user));
                    }
                }
                encrypt(fileName, password); //SZYFROWANIE
                wypiszMape(curMap); //WYPISANIE MAPY O DANYCH KRYTERIACH
                map.clear();
                curMap.clear();

            } break;

            case 3: {

                fmt::print("Wpisz nazwe, po ktorej chcesz wyszukac.\n");
                std::getline(std::cin, kryterium);
                decrypt(fileName, password); //DESZYFROWANIE
                ZczytanieZpliku(fileName); //ZCZYTANIE Z PLIKU

                std::string kategoria;

                for (const auto& pair : map) {
                    const std::string& key = pair.first;
                    const User& value = pair.second;
                    if(value.getNazwa() == kryterium)
                    {
                        kategoria = key;
                        user.nazwa = value.getNazwa();
                        user.login = value.getLogin();
                        user.haslo = value.getHaslo();
                        user.strona = value.getStrona();
                        curMap.insert(std::pair<std::string, User>(kategoria,user));
                    }
                }
                encrypt(fileName, password); //SZYFROWANIE
                wypiszMape(curMap); //WYPISANIE MAPY
                map.clear();
                curMap.clear();

            } break;


            //NIEPRAWIDLOWA WARTOSC
            default: {
                fmt::print("Podalo nieprawdlowa wartosc.\n");
                goto b;
            } break;


        }

    }


    /**
     * Sortowanie hasel.
     *
     * Def:
     * Funkcja posortujHasla otrzymuje nazwę pliku i hasło jako argumenty oraz
     * wykonuje sortowanie zawartości pliku z danymi użytkowników. Użytkownik
     * jest najpierw proszony o wybór dwóch kryteriów sortowania, a następnie
     * dane są odczytywane z pliku, sortowane zgodnie z wybranymi kryteriami
     * i zapisywane z powrotem do pliku.
     *
     *
     * @param fileName
     * @param password
     */
    void posortujHasla(const std::string& fileName, const std::string& password) {
        map.clear();

        fmt::print("Wybierz pierwsze kryterium sortowania:\n");
        fmt::print("1. Po kategorii\n");
        fmt::print("2. Po Nazwie\n");
        fmt::print("3. Po Haśle\n");
        fmt::print("4. Po Loginie\n");
        fmt::print("5. Po Stronie\n");
        int choice1;
        std::cin >> choice1;

        fmt::print("Wybierz drugie kryterium sortowania:\n");
        fmt::print("1. Po kategorii\n");
        fmt::print("2. Po Nazwie\n");
        fmt::print("3. Po Haśle\n");
        fmt::print("4. Po Loginie\n");
        fmt::print("5. Po Stronie\n");
        int choice2;
        std::cin >> choice2;

        std::cin.ignore();


        //TWORZENIE COMPARATORA
        auto comp = std::function<bool(const std::pair<std::string, User>&, const std::pair<std::string, User>&)>();


        //WYBORY
        switch (choice1) {
            case 1: {
                comp = [choice2](const std::pair<std::string, User>& pair1, const std::pair<std::string, User>& pair2) {
                    // PO KATEGORII
                    if (pair1.first < pair2.first) {
                        return true;
                    } else if (pair1.first > pair2.first) {
                        return false;
                    }

                    // DRUGIE KRYTERIUM
                    switch (choice2) {
                        case 1: return pair1.first < pair2.first;
                        case 2: return pair1.second.getNazwa() < pair2.second.getNazwa();
                        case 3: return pair1.second.getHaslo() < pair2.second.getHaslo();
                        case 4: return pair1.second.getLogin() < pair2.second.getLogin();
                        case 5: return pair1.second.getStrona() < pair2.second.getStrona();
                        default: return false;
                    }
                };
                break;
            }
            case 2: {
                comp = [choice2](const std::pair<std::string, User>& pair1, const std::pair<std::string, User>& pair2) {
                    // PO NAZWIE
                    if (pair1.second.getNazwa() < pair2.second.getNazwa()) {
                        return true;
                    } else if (pair1.second.getNazwa() > pair2.second.getNazwa()) {
                        return false;
                    }

                    // Porównywanie po drugim wybranym kryterium
                    switch (choice2) {
                        case 1: return pair1.first < pair2.first;
                        case 3: return pair1.second.getHaslo() < pair2.second.getHaslo();
                        case 4: return pair1.second.getLogin() < pair2.second.getLogin();
                        case 5: return pair1.second.getStrona() < pair2.second.getStrona();
                        default: return false;
                    }
                };
                break;
            }
            case 3: {
                comp = [choice2](const std::pair<std::string, User>& pair1, const std::pair<std::string, User>& pair2) {
                    // PO HASLE
                    if (pair1.second.getHaslo() < pair2.second.getHaslo()) {
                        return true;
                    } else if (pair1.second.getHaslo() > pair2.second.getHaslo()) {
                        return false;
                    }
                    switch (choice2) {
                        case 1: return pair1.first < pair2.first;
                        case 2: return pair1.second.getNazwa() < pair2.second.getNazwa();
                        case 4: return pair1.second.getLogin() < pair2.second.getLogin();
                        case 5: return pair1.second.getStrona() < pair2.second.getStrona();
                        default: return false;
                    }
                };
                break;
            }
            case 4: {
                comp = [choice2](const std::pair<std::string, User>& pair1, const std::pair<std::string, User>& pair2) {
                    // PO LOGINIE
                    if (pair1.second.getLogin() < pair2.second.getLogin()) {
                        return true;
                    } else if (pair1.second.getLogin() > pair2.second.getLogin()) {
                        return false;
                    }
                    switch (choice2) {
                        case 1: return pair1.first < pair2.first;
                        case 2: return pair1.second.getNazwa() < pair2.second.getNazwa();
                        case 3: return pair1.second.getHaslo() < pair2.second.getHaslo();
                        case 5: return pair1.second.getStrona() < pair2.second.getStrona();
                        default: return false;
                    }
                };
                break;
            }
            case 5: {
                comp = [choice2](const std::pair<std::string, User>& pair1, const std::pair<std::string, User>& pair2) {
                    // PO STRONIE
                    if (pair1.second.getStrona() < pair2.second.getStrona()) {
                        return true;
                    } else if (pair1.second.getStrona() > pair2.second.getStrona()) {
                        return false;
                    }
                    switch (choice2) {
                        case 1: return pair1.first < pair2.first;
                        case 2: return pair1.second.getNazwa() < pair2.second.getNazwa();
                        case 3: return pair1.second.getHaslo() < pair2.second.getHaslo();
                        case 4: return pair1.second.getLogin() < pair2.second.getLogin();
                        default: return false;
                    }
                };
                break;
            }
            default:
                break;
        }

        decrypt(fileName, password); //ODSZYFROWANIE

        ZczytanieZpliku(fileName); //ZCZYTANIE DANCYH Z PLIKU DO MAPY

        std::vector<std::pair<std::string, User>> vec(map.begin(), map.end()); //TWORZENIE WEKTORA NA PODSTAWIE MAPY

        fmt::print("Czy wypisac przed sortowaniem? Wpisz -> TAK/NIE.\n");
        std::string wybor;
        std::getline(std::cin,wybor);
        if(wybor == "TAK")
        {
            wypiszVector(vec);
        }

        std::ranges::sort(vec, comp); //SORTOWANIE DANYCH W WEKTORZE

        map.clear();

        fmt::print("Czy wypisac po sortowaniu? Wpisz -> TAK/NIE.\n");
        std::string wybor2;
        std::getline(std::cin,wybor2);
        if(wybor == "TAK")
        {
            wypiszVector(vec);
        }

        for (const auto& pair : vec) {
            const std::string& kategoria = pair.first;
            const User& user = pair.second;

            map.emplace(kategoria, user); //PONOWNE PRZEPISANIE DANYCH Z WEKTORA DO MAPY
        }
        vec.clear();

        zapiszMapeDoPliku(map,fileName); //ZAPIS MAPY DO PLIKU
        encrypt(fileName,password); //ZASZYFROWANIE

        map.clear();
    }


    /**
     * Dodawanie Hasla
     *
     * Def:
     * Funkcja dodajHaslo umożliwia dodanie nowego hasła do pliku, który przechowuje dane
     * użytkowników. Użytkownik wprowadza kategorię, nazwę, wybiera sposób generowania
     * hasła (własne lub automatyczne), wprowadza login i stronę. Następnie dane są
     * zapisywane w pliku po odszyfrowaniu i ponownie zaszyfrowane.
     *
     *
     * @param fileName
     * @param password
     * @return
     */
    auto dodajHaslo(const std::string& fileName, const std::string& password){

        std::ofstream file(fileName, std::ios::app); //URUCHOMIENIE PLIKU DO NADPISU

        std::cin.ignore();

        fmt::print("Wpisz kategorie:\n");
        std::string kategoria;
        std::getline(std::cin, kategoria);

        fmt::print("Wpisz nazwe:\n");
        std::string nazwa;
        std::getline(std::cin, nazwa);

        wybor:
        fmt::print("1. Utworz wlasne haslo\n");
        fmt::print("2. Wygeneruj haslo\n");
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        std::string haslo;
        switch(choice)
        {
            case 1:{
                fmt::print("Wpisz haslo:\n");

                std::getline(std::cin, haslo);
                fmt::print("Trudnosc hasla: {}/10\n",trudnoscHasla(haslo));
            }break;
            case 2:{
                fmt::print("Brak danych znakow, wpisz -> 0.\n");
                fmt::print("Wpisz ilosc liczb.\n");
                int iloscLiczb;
                std::cin >> iloscLiczb;
                std::cin.ignore();

                fmt::print("Wpisz ilosc malych liter.\n");
                int iloscMalych;
                std::cin >> iloscMalych;
                std::cin.ignore();

                fmt::print("Wpisz ilosc duzych liter.\n");
                int iloscDuzych;
                std::cin >> iloscDuzych;
                std::cin.ignore();

                fmt::print("Wpisz ilosc znakow specjlanych.\n");
                int iloscSpecjalnych;
                std::cin >> iloscSpecjalnych;
                std::cin.ignore();

                haslo = generatePassword(iloscLiczb,iloscMalych,iloscDuzych,iloscSpecjalnych);
                fmt::print("Twoje haslo to: {}\n",haslo);
                fmt::print("Trudnosc hasla: {}/10\n",trudnoscHasla(haslo));

            }break;
            default:{
                "Podano zly numer";
                goto wybor;
            }
        }

        fmt::print("Wpisz login:\n");
        std::string login;
        std::getline(std::cin, login);

        fmt::print("Wpisz strona:\n");
        std::string strona;
        std::getline(std::cin, strona);


        decrypt(fileName, password); //ODSZYFROWANIE


        //DOPISYWANIE DO PLIKU
        file << "\n";
        file << "\n";

        file << kategoria << "\n";

        file << nazwa << "\n";

        file << haslo << "\n";

        file << login << "\n";

        file << strona;

        file.close();

        encrypt(fileName,password); //ZASZYFROWANIE
        map.clear();

    }



    /**
     * Edytowanie Hasla
     *
     * Def:
     * Funkcja edytujHaslo umożliwia edycję istniejącego hasła w pliku na podstawie
     * podanych kategorii, nazwy i loginu. Użytkownik wprowadza nowe hasło, a następnie
     * funkcja odszyfrowuje plik, znajduje odpowiednie hasło w mapie, aktualizuje jego
     * wartość, zapisuje zmiany do pliku i ponownie go szyfruje. Jeśli hasło nie zostanie
     * znalezione, zostanie wyświetlony odpowiedni komunikat.
     *
     *
     * @param fileName
     * @param password
     * @param kategoria
     * @param nazwa
     * @param login
     * @return
     */
    auto edytujHaslo(const std::string& fileName, const std::string& password, const std::string& kategoria, const std::string& nazwa, const std::string& login){

        std::fstream file(fileName, std::ios::in | std::ios::out); //URUCHOMIENIE PLIKU DO NADPISU I ODCZYTU
        map.clear();

        fmt::print("Nowe haslo: \n");
        std::string noweHaslo;
        std::getline(std::cin, noweHaslo);

        decrypt(fileName,password); //ODSZYFROWANIE
        ZczytanieZpliku(fileName); //ZCZYTANIE PLIKU DO MAPY

        bool logiczna = false;


        //ITERACJA PO MAPIE I ZMIANA HASLA O DANYCH PARAMETRACH
        for (auto& pair : map)
        {
            std::string Ckategoria = pair.first;
            User& Cuser = pair.second;

            if(Ckategoria == kategoria && Cuser.getNazwa() == nazwa && Cuser.getLogin() == login)
            {

                Cuser.setHaslo(noweHaslo);

                logiczna = true;
                break;
            }

        }


        //BRAK HASLA O DANYCH PARAMETRACH
        if(!logiczna)
        {
            fmt::print("Nie znaleziono hasla.\n");
            encrypt(fileName,password);
            return;
        }

        file.close();
        std::ofstream file2(fileName,std::ios::out);

        zapiszMapeDoPliku(map,fileName); //PONOWNY ZAPIS DANYCH Z MAPY DO PLIKU

        encrypt(fileName,password); //SZYFORWANIE

        file2.close();
        map.clear();


    }



    /**
     * Usuwanie Hasla
     *
     * Def:
     * Funkcja usunHaslo umożliwia usunięcie hasła z pliku na podstawie wprowadzonego
     * hasła do usunięcia. Najpierw plik jest odszyfrowywany, a następnie hasła są
     * odczytywane i przechowywane w mapie. Jeśli hasło zostanie znalezione, użytkownik
     * jest pytany, czy chce je usunąć. Jeśli odpowiedź jest twierdząca, hasło zostaje
     * usunięte z mapy, a zmiany są zapisywane i plik jest ponownie szyfrowany.
     * Jeśli hasło nie zostanie znalezione, odpowiedni komunikat jest wyświetlany.
     *
     *
     * @param fileName
     * @param password
     * @return
     */
    auto usunHaslo(const std::string& fileName, const std::string& password){

        std::fstream file(fileName, std::ios::in | std::ios::out);
        map.clear();

        fmt::print("Wpisz haslo do usuniecia: \n");
        std::string delHaslo;
        std::cin.ignore();
        std::getline(std::cin, delHaslo);

        decrypt(fileName,password);
        ZczytanieZpliku(fileName);


        int licznik = 0;

        bool logiczna = false;

        auto vec = std::vector<User>();

        //ITERACJA PO MAPIE I USUWANIE HASLA O DANYCH PARAMETRACH
        for (auto& pair : map)
        {
            std::string Ckategoria = pair.first;
            User& Cuser = pair.second;

            if(Cuser.getHaslo() == delHaslo)
            {

                vec.push_back(User(Cuser.getNazwa(),Cuser.getHaslo(),Cuser.getLogin(),Cuser.getStrona()));
                licznik++;

                logiczna = true;
                break;
            }

        }


        //BRAK HASLA O DANYCH PARAMETRACH
        if(!logiczna)
        {
            fmt::print("Nie znaleziono hasla.\n");
            encrypt(fileName,password);
            return;
        }


        //POTWIERDZENIE USUNIECIA HASLA I ILOSC WYSTAPIEC HASLA
        if (logiczna) {
            std::cout << "Licznik wynosi: " << licznik << " czy usunac? TAK/NIE.\n"; // ILOSC WYSTAPIEN HASLA
            std::string wybor;
            std::getline(std::cin, wybor);
            if (wybor == "TAK") {
                auto it = std::find_if(map.begin(), map.end(), [&](const auto& pair) {
                    return pair.second.getHaslo() == delHaslo;
                }); //SZUKANIE ITERATORA HASLA ODPOWIADAJACEGO HASLU KTORE ZOSTALO PODANE JAKO TE DO USUNIECIA

                if (it != map.end()) {
                    map.erase(it); //USUNIECIE ELEMENTU O ITERATORZE HASLA KTORE MA ZOSTAC USUNIETE
                    std::cout << "Usunieto haslo.\n";
                } else {
                    fmt::print("Nie znaleziono hasla.\n");
                }
            } else {
                fmt::print("Nie usunieto.");
            }
        }

        file.close();
        std::ofstream file2(fileName,std::ios::out);


        zapiszMapeDoPliku(map,fileName);

        encrypt(fileName,password);

        file2.close();
        map.clear();


    }



    /**
     * Dodawanie Kategorii
     *
     * Def:
     * Funkcja dodajKategorie umożliwia dodanie nowej kategorii do pliku. Użytkownik
     * wprowadza nazwę nowej kategorii, a następnie plik jest odszyfrowywany i zawartość
     * jest odczytywana i przechowywana w mapie. Następnie nowa kategoria jest dodawana
     * do mapy, zmiany są zapisywane i plik jest ponownie szyfrowany.
     *
     *
     * @param fileName
     * @param password
     * @return
     */
    auto dodajKategorie(const std::string& fileName, const std::string& password){

        std::fstream file(fileName, std::ios::in | std::ios::out);
        map.clear();

        fmt::print("Wpisz nazwe nowej Kategorii:\n");
        std::string nowaKat;
        std::cin.ignore();
        std::getline(std::cin, nowaKat);

        decrypt(fileName,password); //ODSZYFROWANIE
        ZczytanieZpliku(fileName); //ZAPIS PLIKU DO MAPY

        map.emplace(nowaKat,User()); //DODANIE NOWEJ KATEGORII

        zapiszMapeDoPliku(map,fileName); //ZAPIS MAPY DO PLIKU

        encrypt(fileName,password); //ZASZYFROWANIE
        map.clear();

    }


    /**
     * Usuwanie Kategorii
     *
     * Def:
     * Funkcja usunKategorie usuwa kategorię z pliku. Użytkownik wprowadza nazwę kategorii
     * do usunięcia, a następnie plik jest odszyfrowywany i zawartość jest odczytywana
     * i przechowywana w mapie. Następnie wybrane kategorie są usuwane z mapy, zmiany
     * są zapisywane i plik jest ponownie szyfrowany.
     *
     *
     * @param fileName
     * @param password
     * @return
     */
    auto usunKategorie(const std::string& fileName, const std::string& password) {

        std::fstream file(fileName, std::ios::in | std::ios::out);
        map.clear();

        fmt::print("Wpisz kategorie do usuniecia: \n");
        std::string delKat;
        std::cin.ignore();
        std::getline(std::cin, delKat);

        decrypt(fileName,password); //ODSZYFROWANIE
        ZczytanieZpliku(fileName); //ZCZYTANIE PLIKU DO MAPY


        bool logiczna = false;

        auto vec = std::vector<std::string>();

        for (auto it = map.begin(); it != map.end();) {
            const std::string& Ckategoria = it->first;
            const User& Cuser = it->second;

            if (Ckategoria == delKat) {
                logiczna = true;
                it = map.erase(it); //USUWANIE WSZYSTKICH HASEL O DANEJ KATEGORII
            } else {
                it++;
            }
        }

        if(!logiczna)
        {
            fmt::print("Nie znaleziono hasla.\n");
            encrypt(fileName,password); //ZASZYFROWANIE
            return;
        }


        file.close();
        std::ofstream file2(fileName,std::ios::out); //URUCHOMIENIE PLIKU


        zapiszMapeDoPliku(map,fileName); //ZAPISANIE MAPY DO PLIKU
        encrypt(fileName,password); //SZYFROWANIE
        file2.close();
        map.clear();


    }



    /**
     * Wypisywanie mapy
     * @param map
     */
    void wypiszMape(const std::unordered_multimap<std::string, User>& map) {
        std::string previousKey;
        bool firstKey = true;

        for (const auto& pair : map) {
            const std::string& kategoria = pair.first;
            const User& user = pair.second;

            if (firstKey || kategoria != previousKey) {
                // Wypisz klucz tylko jeśli jest to nowy klucz
                std::cout << "Kategoria: " << kategoria << std::endl;
                std::cout << std::endl;
                previousKey = kategoria;
                firstKey = false;
            }
            if (user.getNazwa().size() != 0 )
                std::cout << "Nazwa: " << user.getNazwa() << std::endl;
            if (user.getHaslo().size() != 0 )
                std::cout << "Haslo: " << user.getHaslo() << std::endl;
            if (user.getLogin().size() != 0 )
                std::cout << "Login: " << user.getLogin() << std::endl;
            if (user.getStrona().size() != 0 )
                std::cout << "Strona: " << user.getStrona() << std::endl;
            std::cout << std::endl;
        }
    }



    /**
     * Wypisywanie vectora
     * @param vec
     */
    void wypiszVector(const std::vector<std::pair<std::string, User>>& vec) {
        std::string previousKey;
        bool firstKey = true;

        for (const auto& pair : vec) {
            const std::string& kategoria = pair.first;
            const User& user = pair.second;

            if (firstKey || kategoria != previousKey) {
                // Wypisz klucz tylko jeśli jest to nowy klucz
                std::cout << "Kategoria: " << kategoria << std::endl;
                std::cout << std::endl;
                previousKey = kategoria;
                firstKey = false;
            }
            if (user.getNazwa().size() != 0 )
                std::cout << "Nazwa: " << user.getNazwa() << std::endl;
            if (user.getHaslo().size() != 0 )
                std::cout << "Haslo: " << user.getHaslo() << std::endl;
            if (user.getLogin().size() != 0 )
                std::cout << "Login: " << user.getLogin() << std::endl;
            if (user.getStrona().size() != 0 )
                std::cout << "Strona: " << user.getStrona() << std::endl;
            std::cout << std::endl;
        }
    }


    /**
     * Generowanie Hasla
     *
     * Def:
     * Funkcja generatePassword generuje losowe hasło na podstawie podanych parametrów
     * liczby, male, duze i specjalne. Wykorzystuje zestawy znaków: LICZBY, MALE, DUZE
     * i SPECJALNE. Tworzy hasło poprzez losowe wybieranie znaków z poszczególnych
     * zestawów w zależności od podanych liczności. Następnie przemiesza uzyskane znaki
     * w hasle przed jego zwróceniem.
     *
     *
     * @param liczby
     * @param male
     * @param duze
     * @param specjalne
     * @return
     */
    std::string generatePassword(int liczby, int male, int duze, int specjalne) {
        const std::string LICZBY = "0123456789";
        const std::string MALE = "abcdefghijklmnopqrstuvwxyz";
        const std::string DUZE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string SPECJALNE = "!@#$%^&*";

        std::string password;
        std::random_device rand;
        std::mt19937 generate(rand());

        std::uniform_int_distribution<> zakresLiczb(0, LICZBY.size() - 1); //USTAWANIE ZAKRESU GENEROWANYCH LICZB
        std::uniform_int_distribution<> zakresMalych(0, MALE.size() - 1); //USTAWANIE ZAKRESU GENEROWANYCH MALYCH LITER
        std::uniform_int_distribution<> zakresDuzych(0, DUZE.size() - 1); //USTAWANIE ZAKRESU GENEROWANYCH DUZYCH LITER
        std::uniform_int_distribution<> zakresSpecjalnych(0, SPECJALNE.size() - 1); //USTAWANIE ZAKRESU GENEROWANYCH ZNAKOW SPECJALNYCH

        for (int i = 0; i < liczby; ++i) {
            password += LICZBY[zakresLiczb(generate)];
        }

        for (int i = 0; i < male; ++i) {
            password += MALE[zakresMalych(generate)];
        }

        for (int i = 0; i < duze; ++i) {
            password += DUZE[zakresDuzych(generate)];
        }

        for (int i = 0; i < specjalne; ++i) {
            password += SPECJALNE[zakresSpecjalnych(generate)];
        }

        std::shuffle(password.begin(), password.end(), generate); //MIESZANIE

        return password;
    }


    /**
     * Sprawdzanie poziomu trudnosci hasla.
     *
     * Def:
     * Funkcja trudnoscHasla oblicza skalę trudności hasła na podstawie analizy jego składu.
     * Przeszukuje podane hasło i zlicza liczbę wystąpień cyfr, liter oraz znaków
     * specjalnych. Na podstawie tych liczników oblicza wartość skali trudności hasła,
     * która wynosi od 0 do 10. Im większa liczba znaków spełniających określone
     * kryteria , tym wyższa wartość skali.
     * Funkcja zwraca obliczoną skalę trudności hasła.
     *
     *
     * @param password
     * @return
     */
    int trudnoscHasla(std::string password){
        const std::string LICZBY = "0123456789";
        const std::string MALE = "abcdefghijklmnopqrstuvwxyz";
        const std::string DUZE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string SPECJALNE = "!@#$%^&*";

        int licznikLiczb = 0;
        int licznikLiter = 0;
        int licznikSpecjalnych = 0;

        int skala = 0;

        //OBLICZANIE ILOSCI DANYCH ZNAKOW
        for(int i = 0 ; i < password.size() ; i++)
        {
            if(password.at(i) == (char)41 || password.at(i) == 64 || password.at(i) == (char)35 || password.at(i) == (char)36 || password.at(i) == (char)37 || password.at(i) == (char)94 || password.at(i) == (char)38 || password.at(i) == (char)42)
            {
                licznikSpecjalnych++;
            }
            if((password.at(i) >= 65 && password.at(i) <= 90) || (password.at(i) >= 97 && password.at(i) <= 122))
            {
                licznikLiter++;
            }
            if(password.at(i) >= 48 && password.at(i) <= 57)
            {
                licznikLiczb++;
            }
        }

        if(licznikLiter >= 5)
        {
            if((skala + 5) <= 10) skala += 5;
        } else {
            if((skala + 1) <= 10) skala += 1;
        }

        if(licznikLiczb >= 2)
        {
            if((skala + 3) <= 10) skala += 3;
        } else {
            if((skala + 1) <= 10) skala += 1;
        }

        if(licznikSpecjalnych >= 2)
        {
            if((skala + 2) <= 10) skala += 2;
        }
        return skala;
    }


};