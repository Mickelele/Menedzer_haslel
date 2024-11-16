struct User{

     /**
     * Konstruktor Klasy
     */

    User(const std::string &nazwa, const std::string &haslo, const std::string &login,
         const std::string &strona) : nazwa(nazwa), haslo(haslo), login(login), strona(strona) {}

    virtual ~User() {

    }

     /**
     * GETTER NAZWY
     */
    const std::string &getNazwa() const {
        return nazwa;
    }



     /**
     * GETTER HASLA
     */
    const std::string &getHaslo() const {
        return haslo;
    }


     /**
     * GETTER LOGINU
     */
    const std::string &getLogin() const {
        return login;
    }


     /**
     * GETTER STRONY
     */
    const std::string &getStrona() const {
        return strona;
    }

    std::string nazwa;
    std::string haslo;
    std::string login;
    std::string strona;

    User(){}


     /**
     * SETTER NAZWY
     */
    void setNazwa(const std::string &nazwa) {
        User::nazwa = nazwa;
    }


     /**
     * SETTER HASLA
     */
    void setHaslo(const std::string &haslo) {
        User::haslo = haslo;
    }


     /**
     * SETTER LOGINU
     */
    void setLogin(const std::string &login) {
        User::login = login;
    }


     /**
     * SETTER STRONY
     */
    void setStrona(const std::string &strona) {
        User::strona = strona;
    };



};