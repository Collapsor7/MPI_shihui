#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);

    // Get the rank and size in the original communicator
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int color = world_rank / 4; // Determine color based on row

    // int color = world_rank % 4; 
    //除以4，结果为0，1，2，三组；但%4，结果为0，1，2，3，四组

    // Split the communicator based on the color and use the original rank for ordering
    MPI_Comm row_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, (world_size - 1 - world_rank), &row_comm);

    //将现有通信器划分为多个子通信器的函数。它是一个强大的工具，特别适合在并行程序中将进程划分为多个子组，每个子组可以独立通信。
    //	•	comm:
    // 输入参数，原始通信器（如 MPI_COMM_WORLD）。
    // 	•	color:
    // 用于划分通信组的标识符（类似颜色）。拥有相同 color 值的进程会被分配到同一个子通信器。
    // 	•	key:
    // 用于决定子通信器中进程的排名。进程会根据 key 的值升序排列，key 值相同的进程按原始通信器中的排名决定次序。
    // 	•	newcomm:
    // 输出参数，分配的新子通信器。如果进程的 color 为 MPI_UNDEFINED，newcomm 将设置为 MPI_COMM_NULL，即该进程不属于任何新通信器。
    // 	•	返回值:
    // 成功时返回 MPI_SUCCESS；否则返回错误代码。

    //在每一组中用(world_size - 1 - world_rank)进行排序， key 的值升序排列进程

    int row_rank, row_size;
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);

    printf("WORLD RANK/SIZE: %d/%d --- ROW RANK/SIZE: %d/%d\n",
    world_rank, world_size, row_rank, row_size);

    MPI_Comm_free(&row_comm);

    MPI_Finalize();
}

// collapsor@CollapsordeMacBook-Pro MPI_shihui % mpirun -np 12 ./a      
// WORLD RANK/SIZE: 2/12 --- ROW RANK/SIZE: 1/4
// WORLD RANK/SIZE: 6/12 --- ROW RANK/SIZE: 1/4
// WORLD RANK/SIZE: 0/12 --- ROW RANK/SIZE: 3/4
// WORLD RANK/SIZE: 4/12 --- ROW RANK/SIZE: 3/4
// WORLD RANK/SIZE: 8/12 --- ROW RANK/SIZE: 3/4
// WORLD RANK/SIZE: 9/12 --- ROW RANK/SIZE: 2/4
// WORLD RANK/SIZE: 3/12 --- ROW RANK/SIZE: 0/4
// WORLD RANK/SIZE: 5/12 --- ROW RANK/SIZE: 2/4
// WORLD RANK/SIZE: 1/12 --- ROW RANK/SIZE: 2/4
// WORLD RANK/SIZE: 11/12 --- ROW RANK/SIZE: 0/4
// WORLD RANK/SIZE: 7/12 --- ROW RANK/SIZE: 0/4
// WORLD RANK/SIZE: 10/12 --- ROW RANK/SIZE: 1/4