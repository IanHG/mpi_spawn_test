#include "Communicator.h"

namespace midas
{
namespace mpi
{

/**
 * Communicator contructor.
 * Will get rank and nr of processes on the communicator.
 *
 * @param aName          A name/label for the communicator for later reference.
 * @param aCommunicator  The actual MPI communicator.
 **/
Communicator::Communicator
   (  const std::string& aName
   ,  MPI_Comm aCommunicator
   )
   :  mName(aName)
   ,  mCommunicator(aCommunicator)
{
   MPI_Comm_group(mCommunicator, &mGroup);
   MPI_Comm_rank(mCommunicator, &mRank);
   MPI_Comm_size(mCommunicator, &mNrProc);
}

/**
 * Communicator destructor.
 * If it is not MPI_COMM_WORDL, destructor will free the held communicator.
 **/
Communicator::~Communicator
   (
   )
{
   if(mCommunicator != MPI_COMM_WORLD)
   {
      MPI_Comm_free(&mCommunicator);
   }
}

/**
 * Put a barrier on communicator.
 **/
int Communicator::Barrier
   (
   ) const
{
   return MPI_Barrier(mCommunicator);
}


} /* namespace mpi */
} /* namespace midas */

