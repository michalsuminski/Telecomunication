#ifndef RECEIVER_H
#define RECEIVER_H

#include "typedefs.h"
#include "checksum.h"
#include "Configurator.h"

class Getter{
    Configurator* configurator;
public:
    Getter(Configurator* configurator) : configurator(configurator){}
    ByteVector receiveData(int checksum = algebraic);
};

#endif //RECEIVER_H
