#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
 
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
 
    MPI_Group group_a;
    int group_a_processes[2] = {0, 2};
    MPI_Group_incl(world_group, 2, group_a_processes, &group_a);
 
    MPI_Group group_b;
    int group_b_processes[2] = {2, 3};
    MPI_Group_incl(world_group, 2, group_b_processes, &group_b);
 
    MPI_Group intersection_group;
    MPI_Group_intersection(group_a, group_b, &intersection_group);

    //  MPI 中用于计算两个组（MPI_Group 类型）的交集的函数。它会创建一个新组，新组包含两个输入组中公共的进程。
    //int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
    //  •	group1:
    // 输入参数，第一个组（MPI_Group 类型）。
    // 	•	group2:
    // 输入参数，第二个组（MPI_Group 类型）。
    // 	•	newgroup:
    // 输出参数，指向包含 group1 和 group2 交集的组。如果交集为空，新组将为空组。
    //	新组的顺序由 group1 决定。如果某个进程在 group1 和 group2 中都存在，则会出现在新组中，并保持在 group1 中的相对顺序。
    // 	•	返回值:
    // 成功时返回 MPI_SUCCESS，否则返回错误代码。


 
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
    MPI_Comm new_communicator;
    MPI_Comm_create(MPI_COMM_WORLD, intersection_group, &new_communicator);
    //MPI_Comm_create 是 MPI 中用于从现有通信器中创建一个新的子通信器的函数。新通信器只包含属于指定进程组（MPI_Group）的进程，适合需要划分通信子组的场景。
    //创建子集通信
    
    if(new_communicator == MPI_COMM_NULL)
    {
        printf("Process %d is not part of the intersection group.\n", my_rank);
    }
    else
    {
        printf("Process %d is part of the intersection group.\n", my_rank);
    }
 
    MPI_Finalize();
 
    return EXIT_SUCCESS;
}

// collapsor@CollapsordeMacBook-Pro MPI_shihui % mpirun -np 4 ./a           
// Process 1 is not part of the intersection group.
// Process 0 is not part of the intersection group.
// Process 3 is not part of the intersection group.
// Process 2 is part of the intersection group.