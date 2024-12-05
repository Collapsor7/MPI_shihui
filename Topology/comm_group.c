#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int comm_rank;
    MPI_Group g;
    int g_size;
    int g_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    
    MPI_Comm_group(MPI_COMM_WORLD, &g);
    // int MPI_Comm_group(MPI_Comm comm, MPI_Group *group);
    // •	comm: 输入参数，MPI 通信器（如 MPI_COMM_WORLD）。
	// •	group: 输出参数，指向提取的进程组（类型为 MPI_Group）。
    // •	返回值: 成功时返回 MPI_SUCCESS；否则返回错误代码。
    // 使用场景： 当你需要知道某个通信器（如 MPI_COMM_WORLD）所管理的进程集合时，可以用这个函数提取进程组

    MPI_Group_size(g, &g_size);
    MPI_Group_rank(g, &g_rank);

    printf("Group size: %d, my rank in group: %d, in COMM_WORLD: %d\n", g_size, g_rank, comm_rank);

    MPI_Finalize();
}

// collapsor@CollapsordeMacBook-Pro MPI_shihui % mpirun -np 2 ./a
// Group size: 2, my rank in group: 0, in COMM_WORLD: 0
// Group size: 2, my rank in group: 1, in COMM_WORLD: 1