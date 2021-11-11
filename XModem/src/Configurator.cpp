#include "Configurator.h"
using namespace std;

Configurator::Configurator(string port){
    //https://support.microsoft.com/en-us/topic/howto-specify-serial-ports-larger-than-com9-db9078a5-b7b6-bf00-240f-f749ebfd913e
    //Otwarcie wybranego portu
    //metoda CreateFile() uzywana jest do obsluzania portow szeregowych
    com = CreateFile(("\\\\.\\" + port).c_str(),// nazwa adresu portu
            //Metoda c_str() konwertuje ciąg znaków zapisany
            // w zmiennej typu string na ciąg który może być
            // zapisany w tablicy znaków.
                     GENERIC_READ | GENERIC_WRITE,//dostep: odczyt i zapis
                     0,//tryb udostępniania musi mieć wartość 0,
                     NULL, // deskryptor
                     OPEN_EXISTING,// sposob na utworzenie
                     0,// atrybuty pliku
                     NULL);// handle of file with attributes to copy
    if(com == INVALID_HANDLE_VALUE)//Jesli funkcja sie nie powiedze, zwroc INVALID_HANDLE_VALUE
        throw PortError("Nie mozna otworzyc portu: " + port);
    //ustawianie konfiguracji wybranego portu
    //https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-dcb
    DCB dcbSerialParams = {0};
    GetCommState(com, &dcbSerialParams);
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;//szybkość transmisji w Bits Per Second
    dcbSerialParams.ByteSize = 8;// Liczba bitow do wykorzystywana do transmisji
    dcbSerialParams.StopBits = ONESTOPBIT;//liczba bitow stopu
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(com, &dcbSerialParams);
    //ustawianie konfiguracji time-out
    //https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-commtimeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 5000;      //in milliseconds
    timeouts.ReadTotalTimeoutConstant = 5000;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(com, &timeouts);
}

Configurator::~Configurator(){
    CloseHandle(com);
}

byte Configurator::read(){
    byte tmp;
    DWORD noOfBytesRead = 0;
//	metoda z winapi do zapisywania plików
    ReadFile(com,
             &tmp,
             sizeof(tmp),
             &noOfBytesRead,
             NULL);
    if(noOfBytesRead == 0){
        throw TransmissionError("Nie mozna odczytac bajtow.");
    }
    return tmp;
}

void Configurator::write(const ByteVector& bytes){
	DWORD noOfBytesWritten = 0;
//	metoda z winapi do zapisywania plików
	WriteFile(com,
		bytes.data(),
		bytes.size(),
		&noOfBytesWritten,
		NULL);
	if(noOfBytesWritten != bytes.size()){
		throw TransmissionError("Nie mozna zapisac bajtow.");
	}
}

