#include<mpi.h>
#include<stdio.h>


int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Comm comm;
    int dim[2], period[2], reorder;
    int coord[2], id;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    dim[0]=4; dim[1]=3;

    period[0]=1; period[1]=0;
    
    reorder=0;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
    //  •	comm_old:
    // 输入参数，原始通信器（如 MPI_COMM_WORLD）。
    // 	•	ndims:
    // 输入参数，网格的维度数。
    // 	•	dims[]:
    // 输入参数，每个维度的大小（进程数）。
    // 	•	periods[]:
    // 输入参数，数组，指定每个维度是否是周期性的（环绕边界条件）。0 表示非周期性，1 表示周期性。
    // 	•	reorder:
    // 输入参数，是否允许 MPI 重新排列进程的 rank：
    // 	•	0: 不允许重新排列。
    // 	•	1: 允许 MPI 重新排列，以优化通信性能。
    // 	•	comm_cart:
    // 输出参数，创建的笛卡尔拓扑通信器。
    // 	•	返回值:
    // 成功时返回 MPI_SUCCESS，否则返回错误代码。

    int new_rank;

    MPI_Comm_rank(comm, &new_rank);

    if (rank == 5)
    {
        MPI_Cart_coords(comm, rank, 2, coord);
        printf("Rank %d (old rank is %d) coordinates are %d %d\n", new_rank, rank, coord[0], coord[1]);
    }
    //  •	comm:
    // 输入参数，笛卡尔拓扑通信器（由 MPI_Cart_create 创建）。
    // 	•	rank:
    // 输入参数，要查询的进程 rank（在通信器 comm 中的 rank）。
    // 	•	maxdims:
    // 输入参数，coords[] 数组的最大维度大小，通常等于拓扑维度数（ndims）。
    // 	•	coords[]:
    // 输出参数，数组，存储对应 rank 的笛卡尔坐标。
    // 	•	返回值:

    if(rank==0)
    {
        coord[0]=3; coord[1]=1;
        MPI_Cart_rank(comm, coord, &id);
        printf("The processor at position (%d, %d) has rank %d\n", coord[0], coord[1], id);
    }
    // 如果已知坐标 (x, y)，可以通过 MPI_Cart_rank 获取对应的 rank。

    MPI_Finalize();
    return 0;
}