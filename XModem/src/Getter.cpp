#include "Getter.h"
#include "Configurator.h"
#include <chrono>
#include <windows.h>
using namespace std;
using namespace std::chrono;

ByteVector Getter::receiveData(int checksum){
    ByteVector data;
    system_clock::time_point start = system_clock::now(); //start = the current point in time. (poczatek odbierania z pliku)
    bool isSOHFound = false;
    //odbieranie rozpoczyna sie wysylaniem znaku NAK lub C
    while(system_clock::now() - start < seconds(60)){//warunek = aktualny czas - czas startu jest mniejszy od 60 sekund
        if(checksum == algebraic) // dla checksum wypisuj nak
            configurator->write(ByteVector({NAK}));
        else // dla crc wypisuj C
        configurator->write(ByteVector({C}));
        try{
            if(configurator->read() == SOH){//sprobuj odczytac SOH
                isSOHFound = true;
                break;
            }
        }catch(TransmissionError e){}
        Sleep(5);
    }
    if(isSOHFound){ //odczytano SOH
        byte signal = SOH;
        do{
            byte blockNumber1 = configurator->read();
            byte blockNumber2 = configurator->read();
            ByteVector block;
            for(int i = 0; i < 128; i++){//odczytaj bloki 128 bajtowe
                block.push_back(configurator->read());
            }
            if(checksum == algebraic){//dla checksum
                byte checksumRead = configurator->read();
                if(checksumAlgebraic(block) == checksumRead){ //jesli obliczony checksum rowna sie przekazanemu w transmisji
                    configurator->write(ByteVector({ACK}));//odpowiedz ACK - potwierdzenie
                    data.insert(data.end(), block.begin(), block.end());//do ByteVector data dolacz 128 bajtow bloku
                }else{
                    configurator->write(ByteVector({NAK}));//jesli obliczony checksum nie rowna sie przekazanemu w transmisji
                }//odpowiedz negative acknowledgment
            }else{//dla CRC
                ByteVector checksumFromSender({configurator->read(), configurator->read()});//pobieranie CRC z transmisji
                ByteVector calculatedChecksum = checksumCRC(block);//obliczanie CRC danego bloku
                if(checksumFromSender[0] == calculatedChecksum[0] &&
                   checksumFromSender[1] == calculatedChecksum[1]){//jesli rowna sie to ACK, jesli nie to NAK
                    configurator->write(ByteVector({ACK}));
                    data.insert(data.end(), block.begin(), block.end());//do ByteVector data dolacz 128 bajtow bloku
                }else{
                    configurator->write(ByteVector({NAK}));
                }
            }
            signal = configurator->read();//sprawdzenie czy jest kolejny blok
        }while(signal == SOH);
        if(signal == EOT){//jesli End Of Transmission wyslij ACK
            configurator->write(ByteVector({ACK}));
        }else{
            throw TransmissionError("Protocol error!");
        }
    }
    return data;
}


