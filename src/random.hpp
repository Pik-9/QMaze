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