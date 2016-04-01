//BST
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

struct node
{
	int data;
	struct node* left;
	struct node* right;
}*root;

class BST
{
public:
	void insert(node*, int);
	int find(node*,int);
	void inorder(node*);

	BST()
	{
		root = NULL;
	}
};

void BST::insert(node *tree, int data)
{
	if(root == NULL)
	{
		root = new node;
		root->data = data;
		root->left = NULL;
		root->right = NULL;
		return;
	}
	
	if(tree->data == data)
	{
		std::cout<<"\n\nDuplicate data is not allowed in a BST";
		return;
	}

	if(tree->data > data)
	{
		if(tree->left != NULL)
		{
			insert(tree->left,data);
		}
		else
		{
			node* temp = new node;
			tree->left = temp;
			temp->data = data;
			temp->left = NULL;
			temp->right = NULL;
			return;
		}
	}
	else
	{
		if(tree->right != NULL)
		{
			insert(tree->right,data);
		}
		else
		{
			node* temp = new node;
			tree->right = temp;
			temp->data = data;
			temp->left = NULL;
			temp->right = NULL;
			return;
		}
	}

}

int BST::find(node* tree,int key)
{
	if( tree->data == key)
	{
		return 1;
	}
	else if(key > tree->data)
	{
		if(tree->right != NULL)
			find(tree->right,key);
		else
			return 0;
	}
	else if(key < tree->data)
	{
		if(tree->right != NULL)
			find(tree->left,key);
		else
			return 0;
	}
}

void BST::inorder(node* tree)
{
	if(root == NULL)
	{
		std::cout<<"Tree is empty\n";
		return;
	}

	if(tree != NULL)
	{
		inorder(tree->left);
		std::cout<<tree->data<<"\t";
		inorder(tree->right);
	}
}

int main()
{
	int input,rank,key=0,rightSize=0,leftSize=0,found=0;
	int right[20],left[20];
	int flag = 0;
	char check;

	MPI::Status status;

	BST bst;

	MPI::Init();

	rank = MPI::COMM_WORLD.Get_rank();
	if(rank == 0)
	{
		do{
			std::cout<<"\nEnter Number :: ";
			std::cin>>input;
	
			if(flag == 0)
			{
				bst.insert(root,input);
				flag = 1;
			}
			else
			{
				if(input > root->data)
				{
					right[rightSize] = input;
					rightSize++;				
				}
				else
				{
					left[leftSize] = input;
					leftSize++;
				}
			}
			
			std::cout<<"Continue (y/n):: ";
			std::cin>>check;
	
		}while(check != 'n');

		MPI::COMM_WORLD.Send(&rightSize,1,MPI::INT,1,100);
		MPI::COMM_WORLD.Send(&right,rightSize,MPI::INT,1,101);

		MPI::COMM_WORLD.Send(&leftSize,1,MPI::INT,2,200);
		MPI::COMM_WORLD.Send(&left,leftSize,MPI::INT,2,201);
	}

	MPI::COMM_WORLD.Barrier();

	//Right Side of the tree
	if(rank == 1)
	{
		MPI::COMM_WORLD.Recv(&rightSize, 1, MPI_INT, 0, 100, status);
		MPI::COMM_WORLD.Recv(&right, rightSize, MPI_INT, 0, 101, status);

		/*std::cout<<"\n\nRight Side (Processor "<<rank<<")\n";
		for(int i=0;i<rightSize;i++)
		{
			std::cout<<right[i]<<"\n";
		}*/

		for(int i=0;i<rightSize;i++)
		{
			bst.insert(root,right[i]);
		}
	}

	//Left Side of the tree
	if(rank == 2)
	{
		MPI::COMM_WORLD.Recv(&leftSize, 1, MPI_INT, 0, 200, status);
		MPI::COMM_WORLD.Recv(&left, leftSize, MPI_INT, 0, 201, status);

		/*std::cout<<"\n\nLeft Side (Processor "<<rank<<")\n";
		for(int i=0;i<leftSize;i++)
		{
			std::cout<<left[i]<<"\n";
		}*/

		for(int i=0;i<leftSize;i++)
		{
			bst.insert(root,left[i]);
		}
	}

	if(rank == 0)
	{
		std::cout<<"\n\nSearch Element :: ";
		std::cin>>key;

		if(root->data == key)
		{
			std::cout<<"Element found at root by Processor "<<rank<<"\n";
		}
		else
		{
			std::cout<<"Element not found at root by Processor "<<rank<<"\n";
			MPI::COMM_WORLD.Send(&key,1,MPI::INT,1,100);
			MPI::COMM_WORLD.Send(&key,1,MPI::INT,2,200);
		}
	}

	MPI::COMM_WORLD.Barrier();

	if(rank == 1)
	{
		MPI::COMM_WORLD.Recv(&key, 1, MPI_INT, 0, 100, status);
		found = bst.find(root,key);

	}

	if(rank == 2)
	{
		MPI::COMM_WORLD.Recv(&key, 1, MPI_INT, 0, 200, status);
		found = bst.find(root,key);
	}

	MPI::COMM_WORLD.Barrier();

	if(rank == 1)
	{
		if(found == 1)
		{
			std::cout<<"Element found by Processor "<<rank<<"\n";
		}
		else
		{
			std::cout<<"Element not found by Processor "<<rank<<"\n";
		}
	}

	MPI::COMM_WORLD.Barrier();

	if(rank == 2)
	{
		if(found == 1)
		{
			std::cout<<"Element found by Processor "<<rank<<"\n";
		}
		else
		{
			std::cout<<"Element not found by Processor "<<rank<<"\n";
		}
	}

	MPI::COMM_WORLD.Barrier();

	if(rank == 2)
	{
		std::cout<<":: :: Inorder Traversal :: ::\n\n";
		bst.inorder(root);
	}

	MPI::COMM_WORLD.Barrier();

	if(rank == 0)
	{
		bst.inorder(root);
	}

	MPI::COMM_WORLD.Barrier();

	if(rank == 1)
	{
		bst.inorder(root);
	}

	MPI::COMM_WORLD.Barrier();

	MPI::Finalize();

	return 0;
}

/*

mpiuser@ubuntu:~$ mpic++ bst.cpp
mpiuser@ubuntu:~$ mpirun -np 3 ./a.out

Enter Number :: 20
Continue (y/n):: y

Enter Number :: 25
Continue (y/n):: y

Enter Number :: 15
Continue (y/n):: y

Enter Number :: 26
Continue (y/n):: y

Enter Number :: 24
Continue (y/n):: y

Enter Number :: 16
Continue (y/n):: y

Enter Number :: 14
Continue (y/n):: n


Search Element :: 24
Element found by Processor 1
Element not found by Processor 2
:: :: Inorder Traversal :: ::

Element not found at root by Processor 0
14	15	16	20	24	25	26 

*/