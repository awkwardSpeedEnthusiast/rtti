#pragma once

#include <cstdint>

class ExcessiveArgument
{
public:
  int tenArguments(uint8_t one, uint16_t two, uint32_t three, uint64_t four, int8_t five,
                   int16_t six, int32_t seven, int64_t eight, float nine, double ten)
  {
    (void) one;
    (void) two;
    (void) three;
    (void) four;
    (void) five;
    (void) six;
    (void) seven;
    (void) eight;
    (void) nine;
    (void) ten;
    return 0;
  }
};
