#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    MPI_Comm duplicated_communicator;
    MPI_Comm_dup(MPI_COMM_WORLD, &duplicated_communicator);
    //通信器副本
    //创建 MPI_COMM_WORLD 的副本，并通过新通信器进行独立的通信。
    //•	新通信器与原始通信器 comm 具有相同的进程组和属性。
	//•	但新通信器与原始通信器有独立的上下文（即通信不相互干扰）。

    int a, b;

    if (comm_rank == 0) {
        a = 1;
        b = 2;
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Send in COMM_WORLD: %d \n", a);

        MPI_Send(&b, 1, MPI_INT, 1, 0, duplicated_communicator);
        printf("Send in DUP: %d \n", b);
    }

    if (comm_rank == 1) {
        MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Received in COMM_WORLD: %d \n", a);
        MPI_Recv(&b, 1, MPI_INT, 0, 0, duplicated_communicator, MPI_STATUS_IGNORE);
        printf("Received in DUP: %d \n", b);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}