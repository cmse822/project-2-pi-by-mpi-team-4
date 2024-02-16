#include<stdio.h>
#include<iostream>
#include<mpi.h>
using namespace std;
int main(int argc, char** argv) 
{
    MPI_Init(&argc, &argv);
    /*2.3*/
    int name_length = MPI_MAX_PROCESSOR_NAME;
    char proc_name[name_length];
    int procid = MPI_Get_processor_name(proc_name,&name_length);
    printf(" %d is running on node <<%s>>\n",procid,proc_name);
    
    MPI_Finalize();
}
