#include "random.hpp"

#include <fstream>

Random::Random ()
{
  refreshCache ();
}

Random::~Random()
{}

void Random::refreshCache ()
{
  std::fstream dev_rand;
  dev_rand.open ("/dev/urandom", std::ios::in);
  dev_rand.read ((char*) cache, 1024);
  dev_rand.close ();
  
  index = 0;
}

uint64_t Random::getRawNumber()
{
  if (index >= 127)  {
    refreshCache ();
  }
  return cache[index++];
}

uint64_t Random::getNumber(const uint64_t from, const uint64_t to)
{
  uint64_t RET;
  if (from == to)  {
    RET = from;
  } else if (from > to)  {
    RET = (getRawNumber () % (from + 1 - to)) + to;
  } else  {
    RET = (getRawNumber () % (to + 1 - from)) + from;
  }
  return RET;
}