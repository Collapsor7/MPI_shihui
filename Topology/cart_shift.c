#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[2] = {0, 0};//// 维度大小初始值为 0，后面由dims计算

    MPI_Dims_create(size, 2, dims);
    //MPI_Dims_create 是 MPI 中用于帮助计算网格拓扑的函数。
    //它根据给定的进程数和维度数，计算出每个维度的大小，以便用于创建笛卡尔拓扑结构。

    int periods[2] = {true, true};
    int reorder = false;
    int coord[2];

    MPI_Comm new_communicator;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &new_communicator);

    enum DIRECTIONS {DOWN, UP, LEFT, RIGHT};
    char* neighbours_names[4] = {"down", "up", "left", "right"};  
    int neighbours_ranks[4];
    // •	DIRECTIONS 枚举: 定义了四个方向的索引：
	// •	DOWN = 0、UP = 1、LEFT = 2、RIGHT = 3。
	// •	用于在数组 neighbours_ranks[] 和 neighbours_names[] 中访问对应的邻居信息。
	// •	neighbours_names[]: 存储方向的名称（字符串形式），便于输出调试。
	// •	neighbours_ranks[]: 用于存储当前进程在每个方向上的邻居 rank。索引与 DIRECTIONS 枚举对应。

    MPI_Cart_shift(new_communicator, 0, 1, &neighbours_ranks[DOWN], &neighbours_ranks[UP]);
    MPI_Cart_shift(new_communicator, 1, 1, &neighbours_ranks[LEFT], &neighbours_ranks[RIGHT]);
    //  •	comm:
    // 输入参数，笛卡尔拓扑通信器（由 MPI_Cart_create 创建）。
    // 	•	direction:
    // 输入参数，指定移动的方向（对应笛卡尔拓扑的维度索引）。例如：
    // 	•	direction = 0 表示第 0 维（如行）。
    // 	•	direction = 1 表示第 1 维（如列）。
    // 	•	disp:
    // 输入参数，移动的步数。disp = 1 表示正方向的直接邻居，disp = -1 表示负方向的直接邻居。
    // 	•	source:
    // 输出参数，指定方向和步数上邻居进程的 rank。如果没有邻居（如非周期性边界），返回 MPI_PROC_NULL。
    // 	•	dest:
    // 输出参数，指定方向和步数下邻居进程的 rank。如果没有邻居（如非周期性边界），返回 MPI_PROC_NULL。
    //获取每个进程在横向（行方向）和纵向（列方向）的邻居 rank。
    int my_rank;

    MPI_Comm_rank(new_communicator, &my_rank);

    if (my_rank == 0) {

        MPI_Cart_coords(new_communicator, my_rank, 2, coord);
        printf("I am %d, have coord[0]: %d, coord[1]: %d\n", my_rank, coord[0], coord[1]);

        for (int i = 0; i < 4; i++) {
            if (neighbours_ranks[i] == MPI_PROC_NULL)
                printf("[MPI process %d] I have no %s neighbour.\n", my_rank, neighbours_names[i]);
            else
                printf("[MPI process %d] I have a %s neighbour: process %d.\n", my_rank, neighbours_names[i], neighbours_ranks[i]);
        }
    }
    if (my_rank == 1) {

        MPI_Cart_coords(new_communicator, my_rank, 2, coord);
        printf("I am %d, have coord[0]: %d, coord[1]: %d\n", my_rank, coord[0], coord[1]);

        for (int i = 0; i < 4; i++) {
            if (neighbours_ranks[i] == MPI_PROC_NULL)
                printf("[MPI process %d] I have no %s neighbour.\n", my_rank, neighbours_names[i]);
            else
                printf("[MPI process %d] I have a %s neighbour: process %d.\n", my_rank, neighbours_names[i], neighbours_ranks[i]);
        }
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

// collapsor@CollapsordeMacBook-Pro MPI_shihui % mpirun --map-by :OVERSUBSCRIBE -np 16 ./a 
// I am 0, have coord[0]: 0, coord[1]: 0
// [MPI process 0] I have a down neighbour: process 12.
// [MPI process 0] I have a up neighbour: process 4.
// [MPI process 0] I have a left neighbour: process 3.
// [MPI process 0] I have a right neighbour: process 1.
// I am 1, have coord[0]: 0, coord[1]: 1
// [MPI process 1] I have a down neighbour: process 13.
// [MPI process 1] I have a up neighbour: process 5.
// [MPI process 1] I have a left neighbour: process 0.
// [MPI process 1] I have a right neighbour: process 2.