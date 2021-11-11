#ifndef READERWRITER_H
#define READERWRITER_H

#include "typedefs.h"
#include <string>
#include <stdexcept>
#include <windows.h>
using namespace std;

const byte SOH = 0x01;
const byte EOT = 0x04;
const byte ACK = 0x06;
const byte NAK = 0x15;
const byte CAN = 0x18;
const byte C   = 0x43;

class Configurator{
	HANDLE com;
public:
	Configurator(std::string port);
	~Configurator();
	void write(const ByteVector& data);
	byte read();
};

class PortError : public std::runtime_error{
public:
	PortError(const std::string& what_arg) : std::runtime_error(what_arg){}
};
class TransmissionError : public std::runtime_error{
public:
	TransmissionError(const std::string& what_arg) : std::runtime_error(what_arg){}
};

#endif //READERWRITER_H
