#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int n = 7;
    const int ranks[7] = {1, 2, 3, 5, 7, 11, 13};

    MPI_Group prime_group;
    MPI_Group_incl(world_group, 7, ranks, &prime_group);
    // MPI_Group_incl 函数通过从现有的进程组中选择特定进程来创建一个新的进程组。
    // int MPI_Group_incl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);
    // •	参数说明:
	// •	group: 输入参数，现有的进程组（类型为 MPI_Group）。
	// •	n: 输入参数，新组中所包含的进程数量。
	// •	ranks: 输入参数，整数数组，表示 group 中进程的序号（从 0 开始）。
	// •	newgroup: 输出参数，指向新创建的进程组。

    //从world_group中选取ranks，组成新的prime_group
    MPI_Comm prime_comm;
    MPI_Comm_create_group(MPI_COMM_WORLD, prime_group, 0, &prime_comm);

    int prime_rank = -1, prime_size = -1;
    
    if (MPI_COMM_NULL != prime_comm) {
        MPI_Comm_rank(prime_comm, &prime_rank);
        MPI_Comm_size(prime_comm, &prime_size);
    }

    printf("WORLD RANK/SIZE: %d/%d --- PRIME RANK/SIZE: %d/%d\n",
    world_rank, world_size, prime_rank, prime_size);

    MPI_Group_free(&world_group);
    MPI_Group_free(&prime_group);

    if (MPI_COMM_NULL != prime_comm) {
    MPI_Comm_free(&prime_comm);
    }

    MPI_Finalize();
}

// collapsor@CollapsordeMacBook-Pro MPI_shihui % mpirun -np 14 ./a
// WORLD RANK/SIZE: 10/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 0/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 12/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 9/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 8/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 6/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 4/14 --- PRIME RANK/SIZE: -1/-1
// WORLD RANK/SIZE: 11/14 --- PRIME RANK/SIZE: 5/7
// WORLD RANK/SIZE: 1/14 --- PRIME RANK/SIZE: 0/7
// WORLD RANK/SIZE: 7/14 --- PRIME RANK/SIZE: 4/7
// WORLD RANK/SIZE: 2/14 --- PRIME RANK/SIZE: 1/7
// WORLD RANK/SIZE: 13/14 --- PRIME RANK/SIZE: 6/7
// WORLD RANK/SIZE: 3/14 --- PRIME RANK/SIZE: 2/7
// WORLD RANK/SIZE: 5/14 --- PRIME RANK/SIZE: 3/7