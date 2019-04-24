#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include "system.h"
#include <string.h>

using namespace std;


int main(int argc, char** argv)
{
   // tid_map is used to inform the simulator how
   // thread ids map to NUMA/cache domains. Using
   // the tid as an index gives the NUMA domain.
   unsigned int arr_map[] = {0};
   vector<unsigned int> tid_map(arr_map, arr_map +
         sizeof(arr_map) / sizeof(unsigned int));


   //Deal with args
   //NullPrefetch prefetch;
    Prefetch* prefetch;

    /*
   cout << "You have entered " << argc
        << " arguments:" << "\n";
   for (int i = 0; i < argc; ++i)
      cout << argv[i] << "\n";
      */
   int n = 4;

    if (argc >= 3){
       if(strcmp(argv[1], "--prefetcher") == 0) {
          if(strcmp(argv[2], "AdjPrefetch") == 0){
             cout << "Using adjacent prefetcher." << endl;
             prefetch = new AdjPrefetch();
          }
          else if(strcmp(argv[2], "SeqPrefetch") == 0){
             cout << "Using sequential prefetcher." << endl;
             prefetch = new SeqPrefetch();

             if (argc >= 5){
                if(strcmp(argv[3], "--numPrefetch") == 0){
                   //atol stoi
                   n = std::stoi (argv[4]);
                   cout << "n = " << n << endl;
                }
             }
             else {
                cout << "n = " << n << endl;
             }
          }
          else if(strcmp(argv[2], "BestEffortPrefetch") == 0){
             cout << "Using optimized prefetcher." << endl;
             prefetch = new BestEffortPrefetch();

          }
          else {
             cout << "Invalid args" << endl;
             cout << "Please enter --prefetcher {AdjPrefetch, SeqPrefetch --numPrefetch #, BestEffortPrefetch}" << endl;
             cout << "If no numPrefetch, default value of 4 will be chosen." << endl;
             return -1;
          }
       }
       else {
          cout << "Invalid args" << endl;
          cout << "Please enter --prefetcher {AdjPrefetch, SeqPrefetch --numPrefetch #, BestEffortPrefetch}" << endl;
          cout << "If no numPrefetch, default value of 4 will be chosen." << endl;
          return -1;
       }
    }
    else {
       cout << "Please enter --prefetcher {AdjPrefetch, SeqPrefetch --numPrefetch #, BestEffortPrefetch}" << endl;
       cout << "If no numPrefetch, default value of 4 will be chosen." << endl;
       return -1;
    }

   prefetch->numPrefetch = n;


   // The constructor parameters are:
   // the tid_map, the cache line size in bytes,
   // number of cache lines, the associativity,
   // the prefetcher object,
   // whether to count compulsory misses,
   // whether to do virtual to physical translation,
   // and number of caches/domains
   // WARNING: counting compulsory misses doubles execution time
   MultiCacheSystem sys(tid_map, 64, 1024, 64, prefetch, true, false, 1);
   char rw;
   uint64_t address;
   unsigned long long lines = 0;
   ifstream infile;
   // This code works with the output from the
   // ManualExamples/pinatrace pin tool
   infile.open("pinatrace.out", ifstream::in);
   assert(infile.is_open());

   string line;
   while(getline(infile,line))
   {
      stringstream ss(line);
      ss >> rw;
      assert(rw == 'R' || rw == 'W');
      ss >> hex >> address;
      if(address != 0) {
         // By default the pinatrace tool doesn't record the tid,
         // so we make up a tid to stress the MultiCache functionality
         sys.memAccess(address, rw, 0);
      }

      ++lines;
   }

   cout << "Accesses: " << lines << endl;
   cout << "Hits: " << sys.stats.hits << endl;
   cout << "Misses: " << lines - sys.stats.hits << endl;
   cout << "Local reads: " << sys.stats.local_reads << endl;
   cout << "Local writes: " << sys.stats.local_writes << endl;
   cout << "Remote reads: " << sys.stats.remote_reads << endl;
   cout << "Remote writes: " << sys.stats.remote_writes << endl;
   cout << "Other-cache reads: " << sys.stats.othercache_reads << endl;
   //cout << "Compulsory Misses: " << sys.stats.compulsory << endl;

   infile.close();

   return 0;
}
