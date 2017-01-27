/**
************************************************************************
* 
* @file                Communicator.h
*
* Created:             10-10-2013
*
* Author:              Ian H. Godtliebsen  (ian@chem.au.dk)
*
* Short Description:   Class to hold information on an MPI communicator.
* 
* Last modified: 
*
* Copyright:
*
* Ove Christiansen, Aarhus University.
* The code may only be used and/or copied with the written permission 
* of the author or in accordance with the terms and conditions under 
* which the program was supplied.  The code is provided "as is" 
* without any expressed or implied warranty.
* 
************************************************************************
*/
#ifndef MIDAS_MPI_COMMUNICATOR_H_INCLUDED
#define MIDAS_MPI_COMMUNICATOR_H_INCLUDED

#include <mpi.h>
#include <string>

namespace midas
{
namespace mpi
{

/**
 * Class to hold information on MPI communicators.
 **/
class Communicator
{
   private:
      //! Name of communicator.
      std::string mName;
      //! MPI communicator.
      mutable MPI_Comm mCommunicator;
      //! MPI group
      MPI_Group mGroup;
      //! Rank in communicator
      int mRank;
      //! Nr of processes in communicator
      int mNrProc;

   public:
      //! Constructor
      Communicator(const std::string& aName, MPI_Comm aCommunicator);

      //!@{
      //! Delete copy/move ctor/assignment
      Communicator(const Communicator&) = delete;
      Communicator(Communicator&&) = delete;
      Communicator& operator=(const Communicator&) = delete;
      Communicator& operator=(Communicator&&) = delete;
      //!@}

      //! Destructor
      ~Communicator();

      //!@{
      //! Getters
      const std::string& GetName() const { return mName; }
      MPI_Comm GetCommunicator() const { return mCommunicator; }
      int GetRank() const { return mRank; }
      int GetNrProc() const { return mNrProc; }
      //!@}
      
      //!
      int Barrier() const;
      
};

//!
inline bool operator==(const Communicator& aComm1, const Communicator& aComm2)
{
   return aComm1.GetName() == aComm2.GetName();
}

} /* namespace mpi */
} /* namespace midas */

#endif /* MIDAS_MPI_COMMUNICATOR_H_INCLUDED */
