#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <stdint.h>

class Random
{
private:
  uint64_t cache[128];
  uint8_t index;
  
public:
  Random ();
  virtual ~Random ();
  
  void refreshCache ();
  uint64_t getRawNumber ();
  uint64_t getNumber (const uint64_t, const uint64_t);
};

#endif