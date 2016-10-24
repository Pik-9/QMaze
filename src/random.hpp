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

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <stdint.h>

namespace internal_random {
  class RawGenerator
  {
  public:
    RawGenerator ();
    virtual ~RawGenerator ();
    
    virtual uint64_t getRawNumber () = 0;
  };
  
  class Intel_RDRAND_generator : public RawGenerator
  {
  public:
    Intel_RDRAND_generator ();
    ~Intel_RDRAND_generator ();
    
    uint64_t getRawNumber ();
  };
  
  class UnixGenerator : public RawGenerator
  {
  private:
    uint64_t cache[128];
    uint8_t index;
    
  protected:
    void refreshCache ();
    
  public:
    UnixGenerator ();
    ~UnixGenerator ();
    
    static bool urandom_exists ();
    
    uint64_t getRawNumber ();
  };
  
  class StdGenerator : public RawGenerator
  {
  private:
    static bool isInitialised;
    
  public:
    StdGenerator ();
    ~StdGenerator ();
    
    uint64_t getRawNumber ();
  };
}

class Random
{
private:
  internal_random::RawGenerator *generator;
  
public:
  Random ();
  virtual ~Random ();
  
  uint64_t getNumber (const uint64_t, const uint64_t);
};

#endif