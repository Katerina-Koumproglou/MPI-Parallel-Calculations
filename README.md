# MPI Parallel Calculations

This is a simple MPI-based C program that calculates some statistical metrics (mean, maximum, and variance) of a list of integers distributed across multiple processes.

### Input Format
The program prompts the user (only on rank 0) to:
- Enter the total number of integers
  
- Enter the integers themselves

Each process then computes partial results, which are summed to calculate the full result.

*Works best when the number of integers is evenly divisible by the number of processes.*

### Compilation
`mpicc -o mpi mpi.c`

### Execution
`mpiexec -n <num_processes> ./mpi`

### Some examples
Using 2 processors:

![VirtualBoxVM_qdSYYfJnFu](https://github.com/user-attachments/assets/4e05080b-7a03-4c7c-888a-23bdc848098c)

Using 4 processors:

![VirtualBoxVM_Tf39wOShw3](https://github.com/user-attachments/assets/0ca52101-b3dc-49cb-8414-fb9fbed96033)
