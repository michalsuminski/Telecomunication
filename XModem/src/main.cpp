#include <iostream>
#include <fstream>
#include <string>
#include "Configurator.h"
#include "Sender.h"
#include "Getter.h"

using namespace std;

int main(){
	string number;
	cout << "Podaj numer portu: ";
	getline(cin, number);
	string port = "COM" + number;
	Configurator rw(port);
	int procedure;
	int checksum;

	cout << "Podaj tryb pracy\n" ;
	cout << "Wysylanie (1) lub odbieranie(2):";
	cin >> procedure;
	cout << "Suma kontrolna: algebraiczna suma kontrolna(1) lub crc16(2):";
	cin >> checksum;
	cin.get();
    switch (procedure) {
        case 1: {
            string filename;
            ByteVector data;
            cout << "Podaj sciezke do pliku, ktory chcesz wyslac: " << endl;
            getline(cin, filename);
            ifstream is(filename, ios::binary);
            while (is) {
                byte b = is.get();
                if (is)
                    data.push_back(b);
            }
            is.close();
            Sender sender(&rw);
            sender.sendData(data, checksum);}
            break;
        case 2:{
            cout << "Podaj sciezke do pliku, do ktorego chcesz odebrac: " << endl;
            string filename;
            ByteVector data;
            getline(cin, filename);
            Getter getter(&rw);
            data = getter.receiveData(checksum);
            ofstream os(filename, ios::binary);
            os.write((char*)data.data(), data.size());
            os.close();}
            break;
        default:
            cout << "Błąd" << endl;
            return -1;
    }
}
