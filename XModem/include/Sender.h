#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "typedefs.h"
#include "checksum.h"
#include "Configurator.h"

class Sender{
    Configurator* configurator;
public:
    Sender(Configurator* configurator) : configurator(configurator){}
    void sendData(ByteVector data, int checksum = algebraic);
};

#endif //TRANSMITTER_H
