#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
	int count, target;
	int tag1 = 50, tag2 = 60, tag3 = 70, tag4 = 80, tag5 = 90, tag6 = 100;
	int res, num, a1, b1, k, finres, source, i, max, choice = 0;
	float average, var, local_var, global_var;
	int data[100];
	int data_loc[100];
	int rank, size;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	do{		
		if (rank == 0){
			printf("Enter number of values: ");
			scanf ("%d", &count);
			printf("Enter the %d values:\n", count);
			//Enter the values into the "data" array
			for (k = 0; k < count; k++)
				scanf("%d", &data[k]);
			//end the count to all the processors
			for (target = 1; target < size; target++)
				MPI_Send(&count, 1, MPI_INT, target, tag1, MPI_COMM_WORLD);
			num = count / size; //Number of values each processor will handle
			k = num;

			for (target = 1; target < size; target++){
				MPI_Send(&data[k], num, MPI_INT, target, tag2, MPI_COMM_WORLD);
				k+= num;
			}
			//k is the index of the array being moved
			//data_loc is the local array, the part of the array that will be used
			for (k = 0; k < num; k ++)
				data_loc[k] = data[k];
		
		}else{
			MPI_Recv(&count, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
			num = count / size;
			MPI_Recv(&data_loc[0], num, MPI_INT, 0, tag2, MPI_COMM_WORLD, &status);
			
		
		}
	
		//Each processor calculates the sum of its values
		res = 0;
		for (k = 0; k < num; k++)
			res += data_loc[k];
		
		if (rank != 0)
			MPI_Send(&res, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD);
		else{
			finres = res;
			for (source = 1; source < size; source++){
				MPI_Recv(&res, 1, MPI_INT, source, tag3, MPI_COMM_WORLD, &status);
				finres = finres + res;
			}
			average = (float)finres / count;
			printf("\na) Average value %f\n", average);
		
		
			//Calculate the maximum value
			max = data[0];
			for (i = 1; i < count; i++){
				if (data[i] > max)
					max = data[i];
			}
		
			printf("b) Maximum value: %d\n", max);
		}
		
		//Reset all variance variables for the next iteration
		local_var = 0;
		global_var = 0;
		var = 0;
		if(rank == 0){
			for (target = 1; target < size; target++)  	
    				MPI_Send(&average, 1, MPI_FLOAT, target, tag5, MPI_COMM_WORLD);
		}else{
			MPI_Recv(&average, 1, MPI_FLOAT, 0, tag5, MPI_COMM_WORLD, &status);
		}
		
		//Each processor calculates (x–μ)^2 for its local x
		for (k = 0; k < num; k++)
			local_var = local_var + (data_loc[k] - average) * (data_loc[k] - average);
		
		//Send results to processor 0
		if (rank != 0)														
       			MPI_Send(&local_var, 1, MPI_FLOAT, 0, tag6, MPI_COMM_WORLD);			
   		else{																	
		   global_var = local_var;
		       
		   for (source = 1; source < size; source++){ 	
				MPI_Recv(&local_var, 1, MPI_FLOAT, source, tag6, MPI_COMM_WORLD, &status);
				global_var = global_var + local_var;
		   }
			
			var = global_var / count;
			printf("c) Variance: %f\n", var);
   		}
	
		//Μενού επιλογών
		if (rank == 0){
			while(1){
				printf("\n1. Continue.\n");
				printf("2. Exit.\n");
				printf("Please enter your choice: ");
				scanf("%d", &choice);
				if (choice != 1 && choice != 2){
					printf("\n--------------------------------------\n");
					printf("Invalid choice!\n");
					printf("--------------------------------------\n");
					continue;
				}else{
					break;
				}
			}
		}	
	}while (choice != 2);
	
	printf("MPI program exiting...\n\n");
	MPI_Abort(MPI_COMM_WORLD, 0);
	MPI_Finalize();
}
