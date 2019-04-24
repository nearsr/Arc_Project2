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

    //This prefetcher uses a combination of the Stride method,
    // and sequential prefetch with optimized parameters
    //it cycles between the states init, transient, steady, and no_predict
    //and adjusts he number based on that

    numPrefetch = 9;

    if (state == init){
        state == transient;

        for (int i = 1; i <= numPrefetch; i++){

            sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
        }
        return numPrefetch;
    } else if (state == steady) {
        //don't change state, just fetch more

        for (int i = 1; i <= numPrefetch+10; i++){

            sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
        }
        return numPrefetch;
    } else if (state == transient) {
        state == steady;

        for (int i = 1; i <= numPrefetch; i++){

            sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
        }
        return numPrefetch;
    } else if (state == no_predict) {
        state == init;

        for (int i = 1; i <= numPrefetch; i++){

            sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
        }
        return numPrefetch;
    }


    return 0;
}

int BestEffortPrefetch::prefetchHit(uint64_t address __attribute__((unused)),
                                    unsigned int tid __attribute__((unused)),
                                    System* sys __attribute__((unused)))
{

    //This prefetcher uses a combination of the Stride method,
    // and sequential prefetch with optimized parameters
    //it cycles between the states init, transient, steady, and no_predict
    //and adjusts he number based on that

    //The state is updated, but unlike in miss, we do not modify behavior depending on state

    numPrefetch = 9;

    //enum State { init =0, steady =1, transient =2, no_predict =3 };
    if (state == init){
        state = no_predict;
    } else if (state == steady) {
        state = transient;
    } else if (state == transient) {
        state = init;
    } else if (state == no_predict) {
        //do nothing
    }

    for (int i = 1; i <= numPrefetch; i++){

        sys->memAccess(address + (1 << sys->SET_SHIFT)*i, 'R', tid, true);
    }
    return numPrefetch;

}
