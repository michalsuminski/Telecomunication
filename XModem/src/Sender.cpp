#include "Sender.h"
#include "Configurator.h"
#include <cmath>
using namespace std;

void Sender::sendData(ByteVector data, int checksum){
	//oczekiwanie na odbiornik
	if(checksum == algebraic)
		while(configurator->read() != NAK);
	else
		while(configurator->read() != C);
	//rozpoczynanie nadawania
	int numberOfBlocks = (int)ceil(data.size() / 128.0); //	zaokrąglenie w góre liczby bloków (128 bajtowe bloki, może nie być idealnie równo się dzieliło)
//	przejście po wszystkich blokach pliku
	for(int blockIndex = 0; blockIndex < numberOfBlocks; blockIndex++){
		//wyodrebnianie bloku
		auto blockBegin = data.begin() + blockIndex * 128;
		ByteVector::iterator blockEnd;
		if(data.end() - blockBegin <= 128) // ostatni blok
			blockEnd = data.end();
		else
			blockEnd = data.begin() + (blockIndex + 1) * 128; // przejście do kolejnego bloku
		ByteVector block(blockBegin, blockEnd); // tworzymy wektor zawierający bajty bloku
		//dopelnianie zerami do 128 bajtów
		if(block.size() < 128)
			block.insert(block.end(), 128 - block.size(), 0);
		//tworzenie naglowka
		ByteVector header = {
			SOH,
			(byte)(blockIndex + 1), // numer bloku
			(byte)(255 - (blockIndex + 1))}; // dopełnienie do bloku
		//wysylanie
		configurator->write(header);
		configurator->write(block); // 128 bajtow bloku
		if(checksum == algebraic)
			configurator->write(ByteVector({checksumAlgebraic(block)}));
		else
			configurator->write(checksumCRC(block));
		//odbieranie potwierdzenia
		byte typeOfResponse = configurator->read();
		if(typeOfResponse == NAK){ // jeśli odebrano NAK to powtarzamy wysłanie bloku
			blockIndex--;
		}else if(typeOfResponse == CAN){
			throw TransmissionError("Koniec polaczenia");
		}else if(typeOfResponse != ACK){
			throw TransmissionError("Blad, brak ACK");
		}
	}
	configurator->write(ByteVector({EOT})); // zakonczenie transmisji
}
