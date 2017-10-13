#include <stdio.h>
#include <mpi.h>

void barrier(int rank, int nproc);

int main(int argc, char *argv[]) {
    int rank, nproc;

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
    if (nproc < 0) {
        printf("Bad number of processes\n");
        return 0;
    }

    /*
     * get a possition or number of a process in the group
     * MPI_COMM_WORLD - the group's id
     * rank - number of a process in the group (from 0 to size_of_group-1)
     * */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    barrier(rank, nproc);
    MPI_Finalize();

    return 0;
}

void barrier(int rank, int nproc) {

    int bufOUT = 0;
    MPI_Status statusOUT;
    int count = 1;
    int multiple = 1;

    //Stage 1
    while (multiple < nproc) {
        multiple *= 2;
        if (rank % multiple) {
            int receiver = rank - multiple / 2;
            if (receiver >= 0) {
                MPI_Send(&bufOUT, count, MPI_INT, receiver, 1, MPI_COMM_WORLD);
                break;
            }
        } else {
            int sender = rank + multiple / 2;
            if (sender < nproc) {
                MPI_Recv(&bufOUT, count, MPI_INT, sender, 1, MPI_COMM_WORLD, &statusOUT);
                printf("Process %d sent to process %d. Stage %d\n", sender, rank, 1);
            }
        }
    }

    //Stage 2
    //printf("Multiple - %d\n", multiple);
    while (multiple > 1) {
        if (rank % multiple) {
            int sender = rank - multiple / 2;
            if (sender >= 0) {
                MPI_Recv(&bufOUT, count, MPI_INT, sender, 1, MPI_COMM_WORLD, &statusOUT);
                printf("Process %d sent to process %d. Stage %d\n", sender, rank, 2);
            }
        } else {
            int receiver = rank + multiple / 2;
            if (receiver < nproc) {
                MPI_Send(&bufOUT, count, MPI_INT, receiver, 1, MPI_COMM_WORLD);
            }
        }
        multiple /= 2;
    }
}
