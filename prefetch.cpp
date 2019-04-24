#include "prefetch.h"
#include "system.h"

int NullPrefetch::prefetchMiss(uint64_t address __attribute__((unused)),
                              unsigned int tid __attribute__((unused)),
                              System* sys __attribute__((unused)))
{
   return 0;
}
int NullPrefetch::prefetchHit(uint64_t address __attribute__((unused)),
                           unsigned int tid __attribute__((unused)),
                           System* sys __attribute__((unused)))
{
   return 0;
}


//Adjacent ---------------------------------------------------------------------------------

int AdjPrefetch::prefetchMiss(uint64_t address __attribute__((unused)),
                               unsigned int tid __attribute__((unused)),
                               System* sys __attribute__((unused)))
{
    //Fetch the next cache line, which can be found using the SET_SHIFT parameter
    //Note that we are 'R', Reading the line, and Prefetch is 'true'
    sys->memAccess(address + (1 << sys->SET_SHIFT), 'R', tid, true);

    return 1;
}
int AdjPrefetch::prefetchHit(uint64_t address __attribute__((unused)),
                              unsigned int tid __attribute__((unused)),
                              System* sys __attribute__((unused)))
{
    sys->memAccess(address + (1 << sys->SET_SHIFT), 'R', tid, true);

    return 1;
}

//Sequential ---------------------------------------------------------------------------------

int SeqPrefetch::prefetchMiss(uint64_t address __attribute__((unused)),
                              unsigned int tid __attribute__((unused)),
                              System* sys __attribute__((unused)))
{
    //i << sys-> SET_SHIFT would also work
    //Fetch the next 'numPrefetch' lines using a loop
    //Multiply shift by i to get i cache lines down for the next iteration

    for (int i = 1; i <= numPrefetch; i++){
        sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
    }
    return numPrefetch;
}
int SeqPrefetch::prefetchHit(uint64_t address __attribute__((unused)),
                             unsigned int tid __attribute__((unused)),
                             System* sys __attribute__((unused)))
{
    for (int i = 1; i <= numPrefetch; i++){
        sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
    }
    return numPrefetch;
}


//Best Effort ---------------------------------------------------------------------------------

int BestEffortPrefetch::prefetchMiss(uint64_t address __attribute__((unused)),
                                     unsigned int tid __attribute__((unused)),
                                     System* sys __attribute__((unused)))
{

    for (int i = 1; i <= numPrefetch; i++){
        sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
    }
    return numPrefetch;

    //enum State { init =0, steady =1, transient =2, no_predict =3 };
    if (state == init){

    } else if (state == steady) {

    } else if (state == transient) {

    } else if (state == no_predict) {

    }


    return 0;
}
int BestEffortPrefetch::prefetchHit(uint64_t address __attribute__((unused)),
                                    unsigned int tid __attribute__((unused)),
                                    System* sys __attribute__((unused)))
{

    if (state == init){
        state == transient;
    } else if (state == steady) {
        for (int i = 1; i <= numPrefetch; i++){

            sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
        }
        return numPrefetch;
    } else if (state == transient) {
        state == steady;
    } else if (state == no_predict) {
        state == init;
    }


    return 0;
}