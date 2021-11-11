#include "checksum.h"
using namespace std;

// https://en.wikipedia.org/wiki/Cyclic_redundancy_check -> tabela polynomial representations
const dword polynomial = 0x1021;
const byte polynomialLength = 17;

// oba algorytmy przyjmują blok danych(128 bajtów) jako argumenty w celu wyliczenia sum kontrolnych

//oblicza checksum
byte checksumAlgebraic(const ByteVector& bytes){
	int sum = 0;
	for(byte addend : bytes){
//	    suma algebraiczna poszczególnych bajtów danych
		sum += addend;
//		modulo 256 bo suma kontolna jest zapisana na 8 bitach, czyli max 256
		sum %= 256;
	}
//	zwraca 1 bajt (8 bitów)
	return (byte)sum;
}

//oblicza CRC-16-CCITT (wykorzystywane w XMODEM)
ByteVector checksumCRC(const ByteVector& bytes){
    ByteVector a(bytes);
//    dodanie na końcu bloku danych 2 bajtów, które będą przechowywały wynik CRC (2 bajty (16 bitów))
    a.push_back(0); a.push_back(0);

//    konwersja na bity
    for(int bitIndex = 0; bitIndex < bytes.size() * 8 - polynomialLength; bitIndex++){
//        numer bajtu
        int noOfByte = bitIndex / 8;
//        numer bitu w bajcie
        int noOfBit = bitIndex % 8;
//        operacje przesunięć bitowych  "<<" powiększanie , ">>" zmniejszanie
        bool bit = (bool)(a[noOfByte] & (1 << (7 - noOfBit))); // & -> bitowe AND (bitand)
        if(bit){
            dword toXor = polynomial << (7 - noOfBit);
//            ^= -> bitowe wykluczające OR pierwszego i drugiego operandu; przechowuje wynik w obiekcie określonym przez pierwszy operand.
            a[noOfByte] ^= (byte)((toXor >> 16) & 0xff); // ffH = 255
            a[noOfByte + 1] ^= (byte)((toXor >> 8) & 0xff);
            a[noOfByte + 2] ^= (byte)(toXor & 0xff);
        }
    }
//    zwraca 2 bajty (16 bitów) CRC
    return ByteVector(a.end() - 2, a.end());
}
