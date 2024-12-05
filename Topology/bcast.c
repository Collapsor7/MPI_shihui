#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int myid, numprocs;
    int color, new_id, new_nodes;
    MPI_Comm New_Comm;
    int broad_val = -1;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    color = myid%2;
    
    MPI_Comm_split(MPI_COMM_WORLD, color, myid, &New_Comm);

    MPI_Comm_rank(New_Comm, &new_id);
    MPI_Comm_size( New_Comm, &new_nodes);
    
    if (new_id == 0) broad_val = color;

    // •	每个子通信器的 rank = 0 的进程负责初始化 broad_val，值为 color。
	// •	MPI_Bcast 会从每个新通信器的 rank = 0 的进程广播 broad_val 的值给同一通信器内的所有其他进程。
    
    MPI_Bcast(&broad_val, 1, MPI_INT, 0, New_Comm);
    //int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
    //	•	buffer:
    // 指向要广播数据的缓冲区。对于根进程，它包含要广播的数据；对于其他进程，它是接收广播数据的缓冲区。
    // 	•	count:
    // 要广播的元素数量。
    // 	•	datatype:
    // 数据类型，例如 MPI_INT, MPI_FLOAT, MPI_DOUBLE 等。
    // 	•	root:
    // 广播的根进程（Root Process）的排名（Rank）。只有该进程中的 buffer 数据会被广播。
    // 	•	comm:
    // 用于广播的通信器（如 MPI_COMM_WORLD）。
    // 	•	返回值:
    // 成功时返回 MPI_SUCCESS；否则返回错误代码。

    //MPI_Bcast 是 MPI 中的广播函数，用于将一个数据从一个进程（称为根进程，Root Process）广播到通信器中的所有其他进程。它是并行程序中用于同步和数据共享的关键通信操作之一。

    printf("Old proc %d has new rank %d and received value %d\n", myid, new_id, broad_val);

        

    MPI_Finalize();
}

// collapsor@CollapsordeMacBook-Pro MPI_shihui % mpirun -np 4 ./a  
// Old proc 0 has new rank 0 and received value 0
// Old proc 3 has new rank 1 and received value 1
// Old proc 2 has new rank 1 and received value 0
// Old proc 1 has new rank 0 and received value 1

//实际上，广播并不会影响其他子通信器中的进程。MPI_Bcast 仅在其所属的通信器（这里是 New_Comm）中生效。原始通信器 MPI_COMM_WORLD 并未参与广播。

