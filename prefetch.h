#ifndef PREFETCH_H
#define PREFETCH_H

#include <cstdint>

class System;

class Prefetch {
public:

    int numPrefetch;

    //state++;
    enum State { init =0, steady =1, transient =2, no_predict =3 };
    State state = init;

   virtual int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys)=0;
   virtual int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys)=0;
};

//"Prefetcher" that does nothing
class NullPrefetch : public Prefetch {
public:
   int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys);
   int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys);
};

//Adj
class AdjPrefetch : public Prefetch {
public:
    int prefetchMiss(uint64_t address, unsigned int tid,
                     System* sys);
    int prefetchHit(uint64_t address, unsigned int tid,
                    System* sys);
};

//Seq
class SeqPrefetch : public Prefetch {
public:
    int prefetchMiss(uint64_t address, unsigned int tid,
                     System* sys);
    int prefetchHit(uint64_t address, unsigned int tid,
                    System* sys);
};


//Best
class BestEffortPrefetch : public Prefetch {
public:
    int prefetchMiss(uint64_t address, unsigned int tid,
                     System* sys);
    int prefetchHit(uint64_t address, unsigned int tid,
                    System* sys);
};

#endif
