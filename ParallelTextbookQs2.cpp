#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  

  stringstream proctext;
  procno = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  nprocs = MPI_Comm_size(MPI_COMM_WORLD, &size);
  proctext << "Hello"<<" nprocs:"<<nprocs<<" procno:"<<procno
	   << endl;
  cerr << proctext.str(); proctext.clear();

  if(rank == 0){
      protext<<"No.0 process,nprocs:"<<nprocs<<endl;
      cerr<<proctext.str();
      protext.clear
  }
  
  return 0;
}