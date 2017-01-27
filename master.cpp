#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>
#include <signal.h>
#include <vector>
#include <string>
#include <algorithm>

using system_command_t = std::vector<std::string>;
using basic_command_t = std::vector<char*>;

char* downcast_char(char const* c)
{
   return const_cast<char*>(c);
}

/*!
 *
 */
basic_command_t make_command
   (  const system_command_t& cmd
   )
{
   basic_command_t cmd_char( cmd.size() + 1 );
   std::transform( cmd.begin(), cmd.end(), cmd_char.begin(),
         []( std::string const& arg ) { return downcast_char(arg.c_str()); } );
   cmd_char[cmd.size()] = nullptr;
   return cmd_char;
}

/*!
 *
 */
std::mutex spawn_mutex;
void spawner(MPI_Info info, char* argv[], int rank)
{
   MPI_Comm intercomm;
   int errcodes[1];
   
   pid_t pid;
   
   {
      std::cout << " MASTER THREAD IS SPAWNING CHILD " << std::endl;
      std::lock_guard<std::mutex> lock(spawn_mutex);
      /* Create 2 more processes - this example must be called spawn_example.exe for this to work. */
      MPI_Comm_spawn( "spawn", &(argv[0]), 1, info, 0, MPI_COMM_SELF, &intercomm, errcodes );

      // Recieve pid
      MPI_Status mpi_status;
      MPI_Recv(reinterpret_cast<char*>(&pid), sizeof(pid), MPI_BYTE, 0, 0, intercomm, &mpi_status);
      MPI_Comm_free(&intercomm);
      
      std::cout << " MASTER THREAD IS DONE SPAWNING CHILD " << std::endl;
   }
   
   std::cout << " ON RANK : " << rank << " I GOT PID " << pid << std::endl;

   while ( kill(pid, 0) == 0 )
   {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
   }
}

/*!
 *
 */
int main( int argc, char *argv[] )
{
   char host[1024];
   gethostname(host, sizeof(host));
   std::string host_str(host);
   host_str = host_str.substr(0, host_str.find("."));

   pid_t pid = getpid();
   
   std::cout << " MASTER STARTET on host : " << host_str << "   with pid " << pid << std::endl;

   // init mpit
   int provided;
   MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided);

   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   //if(provided != MPI_THREAD_MULTIPLE)
   //{
   //   std::cout << " MPI_THREAD_MULTIPLE not provided :C : " << provided << std::endl;
   //}

   char cwd[1024];
   getcwd(cwd, sizeof(cwd));

   std::string cwd_str = std::string(cwd) + std::string("/spawn_dir");

   // Create command
   system_command_t cmd;
   cmd.emplace_back(host_str);
   for(int i = 1; i < argc; ++i)
   {
      cmd.emplace_back(argv[i]);
   }
   basic_command_t cmd_basic = make_command(cmd);
      
   // MPI Stuff
   MPI_Info info;
   MPI_Info_create(&info);
   MPI_Info_set(info, "host", const_cast<char*>(host_str.c_str()));
   MPI_Info_set(info, "wdir", const_cast<char*>(cwd_str.c_str()));
   
   MPI_Comm parentcomm;
   
   MPI_Comm_get_parent( &parentcomm );
   
   {
      int np = 2;
      std::vector<std::thread> vec;
      for(int i = 0; i < np; ++i)
      {
         vec.emplace_back(spawner, info, &(cmd_basic[0]), rank);
      }

      for(int i = 0; i < np; ++i)
      {
         vec[i].join();
      }
   }
   
   // barrier before finalize
   MPI_Barrier(MPI_COMM_WORLD);

   // finalize
   MPI_Finalize();

   std::cout << " MASTER AFTER FINALIZE " << std::endl;

   return 0;
}
