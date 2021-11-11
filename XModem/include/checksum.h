#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "typedefs.h"

const int algebraic = 1;
const int CRC16 = 2;

byte checksumAlgebraic(const ByteVector& bytes);
ByteVector checksumCRC(const ByteVector& bytes);

#endif //CHECKSUM_H
