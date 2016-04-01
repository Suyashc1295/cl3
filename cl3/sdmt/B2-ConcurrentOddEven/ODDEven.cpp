//Odd Even
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main()
{
	int rank,nproc,i=0,j=0,k=0,swap,n;
	int *a, *sndEven, *sndOdd, *rcvEven, *rcvOdd, *mergeFinal;
	int evenSize, oddSize;
	MPI::Status status;

	MPI::Init();
	rank = MPI::COMM_WORLD.Get_rank();
	nproc = MPI::COMM_WORLD.Get_size();

	n = 16;
	a = new int[n];
	for(i=0;i<n;i++)
	{
		a[i] = rand()%100;
	}
	
	//For finding out the size of the array
	if(n % 2 == 0){
		evenSize = n/2;
		oddSize = n/2;
	}
	else{
		evenSize = (n-1)/2+1;
		oddSize = ((n-1)/2);
	}

	//Allocate memory
	sndEven = new int[evenSize];
	sndOdd = new int[oddSize];
	rcvEven = new int[evenSize];
	rcvOdd = new int[oddSize];
	mergeFinal = new int[evenSize+oddSize];

	//Divide the array
	j=0;
	k=0;

	//Even Phase
	if(rank == 1){
		for(i=0;i<n;i++)
		{
			if(i%2 == 0)
			{
				sndEven[j] = a[i];
				++j;
			}
		}

		for (i = 0 ; i < ( evenSize - 1 ); i++)
  		{
    			for (j = 0 ; j < evenSize - i - 1; j++)
    			{
      				if (sndEven[j] > sndEven[j+1])
      				{
        				swap = sndEven[j];
        				sndEven[j] = sndEven[j+1];
        				sndEven[j+1] = swap;
      				}
    			}
  		}

		MPI::COMM_WORLD.Send(sndEven, evenSize, MPI_INT, 0, 100);
	}

	//Odd Phase
	if(rank == 2){
		for(i=0;i<n;i++)
		{
			if(i%2 != 0)
			{
				sndOdd[k] = a[i];
				++k;
			}
		}

		for (i = 0 ; i < ( oddSize - 1 ); i++)
  		{
    			for (j = 0 ; j < oddSize - i - 1; j++)
    			{
      				if (sndOdd[j] > sndOdd[j+1])
      				{
        				swap = sndOdd[j];
        				sndOdd[j] = sndOdd[j+1];
        				sndOdd[j+1] = swap;
      				}
    			}
  		}

		MPI::COMM_WORLD.Send(sndOdd, oddSize, MPI_INT, 0, 200);
	}

	MPI::COMM_WORLD.Barrier();

	//Printing Values
	if(rank == 0){
		std::cout<<"\nNumber of elements :: "<<n;
	}

	if(rank == 0){
		std::cout<<"\n\n:: :: Input Array :: ::\n\n";
		for(i=0;i<n;i++)
                	std::cout<<a[i]<<"\n";
	}
	MPI::COMM_WORLD.Barrier();

	if(rank == 1){
		std::cout<<"\n\n:: :: Even Phase (Procesor "<<rank<<") :: ::\n\n";
		for(i=0;i<evenSize;i++)
			std::cout<<sndEven[i]<<"\n";
	}
	MPI::COMM_WORLD.Barrier();

	if(rank == 2){
		std::cout<<"\n\n:: :: Odd Phase (Processor "<<rank<<") :: ::\n\n";
		for(i=0;i<oddSize;i++)
			std::cout<<sndOdd[i]<<"\n";
	}
	MPI::COMM_WORLD.Barrier();

if(rank == 0)
	{
		MPI::COMM_WORLD.Recv(rcvEven, evenSize, MPI_INT, 1, 100, status);
		MPI::COMM_WORLD.Recv(rcvOdd, oddSize, MPI_INT, 2, 200, status);
		
		i=0;
		j=0;
		k=0;

		while(j<evenSize && k<oddSize)
		{
			if(rcvEven[j] <= rcvOdd[k])
			{
				mergeFinal[i] = rcvEven[j];
				++j;
			}
			else
			{
				mergeFinal[i] = rcvOdd[k];
				++k;
			}
			++i;
		}

		if(j<evenSize)
		{
			for(;j<evenSize;j++)
			{
				mergeFinal[i] = rcvEven[j];
				++i;
			}
		}
		else if(k<oddSize)
		{
			for(;k<oddSize;k++)
			{
				mergeFinal[i] = rcvOdd[k];
				++i;
			}
		}

		std::cout<<"\n\n:: :: Final Sorted Array (Processor "<<rank<<"):: ::\n";
		for(i=0;i<(evenSize+oddSize);i++)
		{
			std::cout<<mergeFinal[i]<<"\n";
		}
	}

	MPI::Finalize();
	return 0;
}

/*

mpiuser@ubuntu:~$ mpic++ ODDEven.cpp
mpiuser@ubuntu:~$ mpirun -np 3 ./a.out

Number of elements :: 50

:: :: Input Array :: ::

83
86
77
15
93
35
86
92
49
21
62
27
90
59
63
26
40
26
72
36
11
68
67
29
82
30
62
23
67
35
29
2
22
58
69
67
93
56
11
42
29
73
21
19
84
37
98
24
15
70


:: :: Even Phase (Procesor 1) :: ::

11
11
15
21
22
29
29
40
49
62
62
63
67
67
69
72
77
82
83
84
86
90
93
93
98


:: :: Odd Phase (Processor 2) :: ::

2
15
19
21
23
24
26
26
27
29
30
35
35
36
37
42
56
58
59
67
68
70
73
86
92


:: :: Final Sorted Array (Processor 0):: ::
2
11
11
15
15
19
21
21
22
23
24
26
26
27
29
29
29
30
35
35
36
37
40
42
49
56
58
59
62
62
63
67
67
67
68
69
70
72
73
77
82
83
84
86
86
90
92
93
93
98

*/