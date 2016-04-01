//Quick-Sort
#include<iostream>
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>
#include<unistd.h>

using namespace std;
class Qsorter
{
private:
	int *a;
	int size;
	int no_comp;
public:
	Qsorter()
	{
		a=NULL;
		size=0;
		no_comp=0;

	}
	void sort(int *a, int size)
	{
		this->a=a;
		this->size=size;
		this->no_comp=0;

		quicksort(this->a,0,this->size-1);
		cout<<"\nNumber of comparisons::"<<no_comp;
	}

	void quicksort(int *a,int p, int r)
	{
		int div;
		if(p<r)
		{
			div=partition(a,p,r);
			#pragma omp parallel sections
			{
				#pragma omp section
				{
					quicksort(a,p,div-1);
				}
				#pragma omp section
				{
					quicksort(a,div+1,r);
				}
			}
		}

	}

	int partition(int *a,int l, int h){
		int i=l;
		int j=h;
		int p=a[l];

		do{
			do{
				i=i+1;
				no_comp++;

			}while(a[i]<p && i<=h);

			while(a[j]>p && j>=l)
			{
				j=j-1;
				no_comp++;
			}
			no_comp++;
			if(i<j)
			{
				swap(a,i,j);
			}

		}while(i<j);
		a[l]=a[j];
		a[j]=p;
		return j;
	}

	void swap(int *a, int i, int j)
	{
		int temp;
		temp=a[i];
		a[i]=a[j];
		a[j]=temp;
	}

};

int main() {

	int *a,*w, no;
	Qsorter q;

	cout<<"\nEnter the size of array::";
	cin>>no;
	a= new int[no];
	w= new int[no];

	for(int i=0;i<no;i++)
	{
		a[i]=rand()%100;
	}
	
	cout<<"\n\n*****************Average Case****************\n";
	cout<<"\nInput  Array::\n";
	for(int i=0;i<no;i++)
	{
			cout<<a[i]<<"\t";
	}

	double start = omp_get_wtime();
	q.sort(a,no);
	double end = omp_get_wtime();

	
	cout<<"\n\nSorted array is::\n";
	for(int i=0;i<no;i++)
	{
			cout<<a[i]<<"\t";
	}
	cout<<"\n";
	printf("\nThe Execution time is:: %.16g\n",end-start);
	
	cout<<"\n\n****************Worst Case****************\n";
	for(int i=0;i<no;i++)
	{
	w[i]=i;
	}
	cout<<"\nInput Array::\n";
	for(int i=0;i<no;i++)
	{
	cout<<w[i]<<"\t";
	}
	

	 start = omp_get_wtime();
	q.sort(w,no);
	 end = omp_get_wtime();

	
	cout<<"\n\nSorted array is::\n";
	for(int i=0;i<no;i++)
	{
			cout<<w[i]<<"\t";
	}
	cout<<"\n";
	printf("\nThe Execution time is:: %.16g\n",end-start);


	return 0;
}

/*
[prajput@localhost ~]$ g++ -fopenmp qs.cpp
[prajput@localhost ~]$ ./a.out

Enter the size of array::100


*****************Average Case****************

Input  Array::
83	86	77	15	93	35	86	92	49	21	62	27	90	59	63	26	40	26	72	36	11	68	67	29	82	30	62	23	67	35	29	2	22	58	69	67	93	56	11	42	29	73	21	19	84	37	98	24	15	70	13	26	91	80	56	73	62	70	96	81	525	84	27	36	5	46	29	13	57	24	95	82	45	14	67	34	64	43	50	87	876	78	88	84	3	51	54	99	32	60	76	68	39	12	26	86	94	39	
Number of comparisons::979

Sorted array is::
2	3	5	5	8	11	11	12	13	13	14	15	15	19	21	21	22	23	24	24	25	26	26	26	26	27	27	29	29	29	29	30	32	34	35	35	36	36	37	39	39	40	42	43	45	46	49	50	51	54	56	56	57	58	59	60	62	62	62	63	64	67	67	67	67	68	68	69	70	70	72	73	73	76	76	77	78	80	81	82	82	83	84	84	84	86	86	86	87	88	90	91	92	93	93	94	95	96	98	99	

The Execution time is:: 0.007097452999971665


****************Worst Case****************

Input Array::
0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27	28	29	30	31	32	33	34	35	36	37	38	39	40	41	42	43	44	45	46	47	48	49	50	51	52	53	54	55	56	57	58	59	60	61	62	63	64	65	66	67	68	69	70	71	72	73	74	75	76	77	78	79	80	81	82	83	84	85	86	87	88	89	90	91	92	93	94	95	96	97	98	99	
Number of comparisons::5148

Sorted array is::
0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27	28	29	30	31	32	33	34	35	36	37	38	39	40	41	42	43	44	45	46	47	48	49	50	51	52	53	54	55	56	57	58	59	60	61	62	63	64	65	66	67	68	69	70	71	72	73	74	75	76	77	78	79	80	81	82	83	84	85	86	87	88	89	90	91	92	93	94	95	96	97	98	99	

The Execution time is:: 0.003163275000019894
[prajput@localhost ~]$ 
*/
