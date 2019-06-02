#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

typedef vector<vector<int>> tabela;

void citanje(tabela &nekiOpis, vector<int> &zavrsnaStanja, int &pocetnoStanje) {    //cita podatke iz fajla
    ifstream file;
    int broj;   //varijabla za procitane brojeve iz fajla
    file.open("DFAconf.txt");
    file.peek();    //file.eof() ne radi ako ovog nema
    if (file.eof()) {       //baca izuzetak ukoliko je fajl prazan
        file.close();
        throw "DFAconf file je prazan!";
    }
    file.ignore(1000, '=');
    file >> broj;
    pocetnoStanje = broj;   //ucitava pocetno stanje
    file.ignore(1000, '=');
    while (file >> broj) {      //zavrsna stanja stavlja u vektor
        zavrsnaStanja.push_back(broj);
        if (file.peek() == '\n') break;
    }
    file.ignore(1000, '=');
    int brojac = 0;
    vector<int> pom;
    while (file >> broj) {      //funkciju tranzicije stavlja u vektor vektora
        brojac++;
        pom.push_back(broj);
        if (brojac == 2) {      //nakon svaka dva procitana broja ubacuje vektor u nekiOpis
            nekiOpis.push_back(pom);
            brojac = 0;
            pom.clear();
        }
    }
    file.close();
    if (brojac != 0) {      //baca izuzetak ukoliko funkcija tranzicije nije djeljiva sa 2
        throw "Tabela tranzicije nije potpuna!";
    }
    if (pocetnoStanje >= nekiOpis.size() || pocetnoStanje < 0) {        //baca izuzetak ukoliko pocetno stanje izlazi iz okvira svih stanja
        throw "Pocetno stanje nije ispravno!";
    }
    for (auto &i : nekiOpis) {      //baca izuzetak ukoliko funkcija tranzicije pokusa preci na nepostojece stanje
        for (auto &j : i) if (j >= nekiOpis.size()) throw "Funkcija tranzicije nije ispravna!";
    }
}

bool provjeraUnosa(const string &nekiUnos) {        //provjera ispravnosti unosa stringa, mogu biti samo '0','1' ili prazan string
    for (auto &i : nekiUnos) if (i != '0' && i != '1') return false;
    return true;
}

bool provjeraUpita(const string &nekiUpit) {        // provjera ispravnosti unosa za nastavak programa
    if (nekiUpit != "d" && nekiUpit != "n") return false;
    return true;
}

void ispis(const string &nekiUnos, const int &i, const int &trenutnoStanje) {          //ispisuje korake '011q10'
    for(int j=0; j<i; j++) {         //ispisuje procitane '0' i '1'
        cout << nekiUnos[j];
    }
    cout << ".q" << trenutnoStanje << ".";        //ispisuje trenutno stanje
    for(int j=i; j<nekiUnos.size(); j++) {      //ispisuje preostale '0' i '1'
        cout << nekiUnos[j];
    }
    cout << endl;
}

int funTranzicije(const string &nekiUnos, const tabela &nekiOpis, const int &pocetnoStanje) {   //racuna konacno stanje
    int trenutnoStanje = pocetnoStanje;
    ispis(nekiUnos, 0, trenutnoStanje);     //ispisuje pocetno 'q0101'
    for (int i=0; i<nekiUnos.size(); i++) {
        if (nekiUnos[i] == '1') trenutnoStanje = nekiOpis[trenutnoStanje][1];
        else trenutnoStanje = nekiOpis[trenutnoStanje][0];
        ispis(nekiUnos, i+1, trenutnoStanje);   //ispisuje ostale '101q101'
    }
    return trenutnoStanje;
}

bool daLiPrihvata (const vector<int> &zavrsnaStanja, const int &konacnoStanje) {       //provjera da li je string prihvacen
    for (auto &i : zavrsnaStanja) if (i == konacnoStanje) return true;
    return false;
}

int main() {

    tabela opis;       //funkcija tranzicije data kao vektor vektora
    vector<int> zavrsnaStanja;      //vektor zavrsnih stanja
    int pocetnoStanje = -1;
    try {
        citanje(opis, zavrsnaStanja, pocetnoStanje);  //cita podatke iz fajla
    }
    catch (const char *izuzetak) {
        cout << izuzetak << endl;
        return 1;   //zavrsava program u slucaju izuzetka
    }
    bool radi = true;

    while (radi) {      //glavna petlja
        string unos;        //varijabla koja prima string nula i jedinica
        cout << "Unesite string: ";
        getline(cin, unos);

        while (!provjeraUnosa(unos)) {          //provjera ispravnosti unosa stringa, mogu biti samo '0' i '1'
            cout << "Unijeli ste pogresan string!" << endl;
            cout << "\nUnesite string: ";
            getline(cin, unos);
        }

        int konacnoStanje = funTranzicije(unos, opis, pocetnoStanje); //racuna konacno stanje i ispisuje korake '011q10'

        if (daLiPrihvata(zavrsnaStanja, konacnoStanje)) cout << "String je prihvacen!" << endl << endl; //provjera da li je string prihvacen
        else cout << "String je odbijen!" << endl << endl;

        string upit;    //varijabla za nastavak programa ili kraj, ako nije tipa string cita samo prvo slovo pa je unos tipa "neko" prihvacen kao "n"
        while (!provjeraUpita(upit)) {              // provjera ispravnosti unosa za varijablu 'upit'
            cout << "Da li zelite nastaviti?(d/n)";
            getline(cin, upit);
            if (upit == "n") radi = false;   //izlaz iz glavne petlje
        }
    }

    return 0;
}
