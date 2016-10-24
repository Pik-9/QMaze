/******************************************************************************
 *  QMaze - A simple maze game                                                *
 *  Copyright 2016 Daniel Steinhauer <d.steinhauer@mailbox.org>               *
 *                                                                            *
 *                                                                            *
 *  This file is part of QMaze                                                *
 *                                                                            *
 *  QMaze is free software: you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  QMaze is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with QMaze.  If not, see <http://www.gnu.org/licenses/>.            *
 ******************************************************************************/

#include "random.hpp"

#include <fstream>
#include <time.h>
#include <stdlib.h>

bool internal_random::StdGenerator::isInitialised = false;

internal_random::RawGenerator::RawGenerator ()
{}

internal_random::RawGenerator::~RawGenerator ()
{}

internal_random::Intel_RDRAND_generator::Intel_RDRAND_generator ()
  : RawGenerator ()
{}

internal_random::Intel_RDRAND_generator::~Intel_RDRAND_generator ()
{}

uint64_t internal_random::Intel_RDRAND_generator::getRawNumber ()
{
  uint64_t RET;
  asm (
    "rdrand %0;"
    : "=r" (RET)
  );
  return RET;
}

internal_random::UnixGenerator::UnixGenerator ()
  : RawGenerator ()
{
  refreshCache ();
}

internal_random::UnixGenerator::~UnixGenerator ()
{}

bool internal_random::UnixGenerator::urandom_exists ()
{
  bool RET;
  std::fstream dev_rand;
  dev_rand.open ("/dev/urandom", std::ios::in);
  RET = dev_rand.good ();
  dev_rand.close ();
  return RET;
}

void internal_random::UnixGenerator::refreshCache ()
{
  std::fstream dev_rand;
  dev_rand.open ("/dev/urandom", std::ios::in);
  dev_rand.read ((char*) cache, 1024);
  dev_rand.close ();
  
  index = 0;
}

uint64_t internal_random::UnixGenerator::getRawNumber ()
{
  if (index >= 127)  {
    refreshCache ();
  }
  return cache[index++];
}

internal_random::StdGenerator::StdGenerator ()
  : RawGenerator ()
{
  if (!isInitialised)  {
    srand (time (NULL));
    isInitialised = true;
  }
}

internal_random::StdGenerator::~StdGenerator ()
{}

uint64_t internal_random::StdGenerator::getRawNumber()
{
  union BigInt
  {
    uint64_t Big64;
    uint32_t Little32[2];
  } RET;
  
  RET.Little32[0] = rand ();
  RET.Little32[1] = rand ();
  return RET.Big64;
}

Random::Random ()
{
  uint32_t ecx;
  asm (
    "cpuid;"
    : "=c" (ecx)
    : "a" (1)
    : "edx"
  );
  
  if (ecx & (1 << 30))  {
    generator = new internal_random::Intel_RDRAND_generator ();
  } else if (internal_random::UnixGenerator::urandom_exists ())  {
    generator = new internal_random::UnixGenerator ();
  } else {
    generator = new internal_random::StdGenerator ();
  }
}

Random::~Random()
{
  delete generator;
}

uint64_t Random::getNumber(const uint64_t from, const uint64_t to)
{
  uint64_t RET;
  if (from == to)  {
    RET = from;
  } else if (from > to)  {
    RET = (generator->getRawNumber () % (from + 1 - to)) + to;
  } else  {
    RET = (generator->getRawNumber () % (to + 1 - from)) + from;
  }
  return RET;
}