#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int bufOUT = 0;
    MPI_Status statusOUT;
    int rank, nproc;
    int count = 1;
    int msgtag = 13;

    /*
     * initialize MPI system and pass the argument to all processes
     * */
    MPI_Init(&argc, &argv);

    /*
     * get number of all processes in the group
     * MPI_COMM_WORLD - the group's id
     * nproc - size of the group (number of processes
     * */
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    /*
     * get a possition or number of a process in the group
     * MPI_COMM_WORLD - the group's id
     * rank - number of a process in the group (from 0 to size_of_group-1)
     * */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    /*
    * Blocked processes from a group, which called this function till all of them will perform this function
    * MPI_COMM_WORLD - the group's id
    * */
    MPI_Barrier(MPI_COMM_WORLD);
    printf("It's %d of %d\n", rank, nproc);
    fflush(stdout);

    if(rank > 0) {
        /*
         * Get a message with msgtag from a process with blocked
         *
         * bufOUT - an address of the beginning of the buffer of a received message
         * count - max number of elements in a received message
         * MPI_INT - a type of elements of a received message
         * (rank - 1) - id of a process which sending a message
         * MPI_COMM_WORLD - the group's id
         * statusOUT - a status of a received message
         * */

        MPI_Recv(&bufOUT, count, MPI_INT, rank - 1, msgtag, MPI_COMM_WORLD, &statusOUT);
        printf("Process %d received number %d from process %d\n", rank, bufOUT, rank - 1);
    }

    bufOUT++;

    if(rank < nproc - 1) {

        /*
         * Send a message with msgtag from a process with blocked
         *
         * bufOUT - an address of the beginning of the buffer of a sent message
         * count - max number of elements in a sent message
         * MPI_INT - a type of elements of a sent message
         * (rank + 1) - id of a process which receiving a message
         * MPI_COMM_WORLD - the group's id
         * */

        MPI_Send(&bufOUT, 1, MPI_INT, rank + 1, msgtag, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
