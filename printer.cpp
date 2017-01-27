#include <iostream>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>

int main()
{
   char host[1024];
   gethostname(host, sizeof(host));
   
   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   
   std::cout << " PRINTER STARTET on host : " << host << "   with pid : " << getpid() << std::endl;

   std::string cmd = std::string("touch lol") + std::to_string(getpid());
   std::system(cmd.c_str());

   return 0;
}
