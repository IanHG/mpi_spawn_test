#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Communicator.h"

#define NUM_SPAWNS 2

int main( int argc, char *argv[] )
{
   char host[1024];
   gethostname(host, sizeof(host));
   
   std::cout << " SPAWNER STARTET on host : " << host << "   and was spawned by : " << argv[1] << std::endl;

   // MPI region
   // --------------------------------------------------------------------

   std::cout << " BEFORE INIT " << std::endl;
   int provided;
   MPI_Init_thread( &argc, &argv, MPI_THREAD_SINGLE, &provided);

   std::cout << " AFTER INIT " << std::endl;
   //if(provided != MPI_THREAD_MULTIPLE)
   //{
   //   std::cout << " MPI_THREAD_MULTIPLE not provided :C" << std::endl;
   //}
   
   MPI_Comm parentcomm;
   MPI_Comm_get_parent( &parentcomm );
   
   std::cout << " BEFORE SEND " << std::endl;
   pid_t pid = getpid();
   MPI_Send(reinterpret_cast<char*>(&pid), sizeof(pid), MPI_BYTE, 0, 0, parentcomm);
   std::cout << " AFTER SEND " << std::endl;
   
   MPI_Comm_free(&parentcomm);
   
   MPI_Finalize();

   std::cout << " DONE WITH MPI STUFF " << std::endl;
   
   // Non-MPI region
   // --------------------------------------------------------------------
   char cwd[1024];
   getcwd(cwd, sizeof(cwd));
   std::string cwd_str = std::string(cwd) + std::string("/../work_dir");

   std::cout << " TRYING TO CHANGE DIR " << std::endl;

   if(chdir(cwd_str.c_str()) == -1)
   {
      std::cout << "COULD NOT CHDIR: " << cwd_str << std::flush;
   }
   
   std::cout << " TRYING TO EXECUTE " << std::endl;

   // make call
   if(execvp(argv[2], &(argv[2])) == -1)
   {
      std::cout << " DID NOT SUCCEED CALL " << std::endl;
   }
   
   
   return 0;
}
