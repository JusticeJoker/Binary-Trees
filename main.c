#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
						Garantir sempre o balanceamento da árvore
						
 		Permitir inserir, atualizar e eliminar contribuintes;
 		Apresentar os contribuintes de uma cidade indicada pelo utilizador, utilizando a ordenação
			ascendente pelo número de contribuinte;
		 Apresentar a média dos rendimentos dos contribuintes de uma profissão indicada pelo utilizador;
*/

// defines
#define bool int
#define debug 1

// structs
typedef struct taxPayer
{
	int incomeNumber;
	char name[50 + 1];
	char city[30 + 1];
	char job[50 + 1];
	float income;
	struct taxPayer *left;
	struct taxPayer *right;
	int height;	
	
} TaxPayer;

// height of a node
int height (TaxPayer *node)
{
	return (node == NULL) ? 0 : node->height;
}

//max of 2 numbers
int max(int a, int b)
{
	return (a > b) ? a : b;
}

int treeBalance(TaxPayer *node)
{
	return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

//show tax payers in 
void showTaxPayers_preOrder(TaxPayer *node)
{
	// stop recursive call
	if (node == NULL)
		return;
	
	// show data and recursive call
	printf("\nIncome Number: %d | Name: %s | City: %s | Job: %s | Total Income %.2f \n", 
		node->incomeNumber, node->name, node->city, node->job, node->income);
	showTaxPayers_preOrder(node->left);
	showTaxPayers_preOrder(node->right);
	
}

// 	compute tree weight
int treeWeight(TaxPayer *node)
{
	// variables
 	int leftTreeWeight, rightTreeWeight, maxWeight;
 	
 	// stop recursive call
 	if (node == NULL)
 		return;
 		
 	// compute weights
 	leftTreeWeight = treeWeight(node->left);
 	rightTreeWeight = treeWeight(node->right);
 	
 	// save the computed maxWeight
 	if (leftTreeWeight > rightTreeWeight)
 		maxWeight = leftTreeWeight;
 	else
 		maxWeight = rightTreeWeight;
 		
 	// return maxWeight added by 1
 	return (maxWeight + 1);
}

// true if tree is balanced
bool treeIsBalanced(TaxPayer *node)
{
	// variables
	int leftTreeWeight, rightTreeWeight;
	
	// stop recursive call
	if (node == NULL)
		return 1;
		
	// compute weights
	leftTreeWeight = treeWeight(node->left);
	rightTreeWeight = treeWeight(node->right);
	
	// if weights is <= 1 then the tree is balanced
	if (abs(leftTreeWeight - rightTreeWeight) <= 1 && treeIsBalanced(node->left) && treeIsBalanced(node->right))
		return 1; // tree is balanced
	else
		return 0;
}

// left rotation
TaxPayer *leftRotation(TaxPayer *z)
{
	//variation
	TaxPayer *y, *T2;
	
	//initialization
	y = z->right;
	T2 = y->left;
	
	
	// perform the rotation
	y->left = z;
	z->right = T2;
	
	//update heights
	z->height = max(height(z->left), height(z->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;
	
	//return node 
	return y;	
}

// Right Rotation
TaxPayer *rightRotation(TaxPayer *z)
{
	// variables
	TaxPayer *y, *T3;
	
	// initialization
	y = z->left;
	T3 = y->right;
	
	// perform the rotation
	y->right = z;
	z->left = T3;
	
	//update heights
	z->height = max(height(z->left), height(z->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;
	
	//return node
	return y;
	
}

// find the node with the smallest key value
TaxPayer *minValueNode(TaxPayer *node)
{
	// variables
	TaxPayer *actual;
	
	// initializing
	actual = node;
	
	// find the leaf on the most left possible (minimum)
	while (actual->left != NULL)
		actual = actual->left;
		
	//return the leaf
	return actual;
}

// new node
TaxPayer *newTaxPayer(TaxPayer *node, int taxpayerIncomeNumber, char *taxpayerName, char *taxpayerCity, char *taxpayerJob, float taxpayerIncome)
{
	// variables
	TaxPayer *taxPayer;
	int balance;
	
	// node information
	if (node == NULL)
	{
		//build a new node
		taxPayer= (TaxPayer*) malloc(sizeof(TaxPayer));
		taxPayer->income = taxpayerIncome;
		strcpy(taxPayer->city, taxpayerCity);
		strcpy(taxPayer->name, taxpayerName);
		strcpy(taxPayer->job, taxpayerJob);
		taxPayer->incomeNumber = taxpayerIncomeNumber;
		taxPayer->height = 1;
		taxPayer->left = NULL;
		taxPayer->right = NULL;
		node = taxPayer;
		return node;
	}
	
	// search to place the node 
	if (taxpayerIncomeNumber < node->incomeNumber)
		node->left = newTaxPayer(node->left, taxpayerIncomeNumber, taxpayerName, taxpayerCity, taxpayerJob, taxpayerIncome);
	else if (taxpayerIncomeNumber > node->incomeNumber)
		node->right = newTaxPayer(node->right, taxpayerIncomeNumber, taxpayerName, taxpayerCity, taxpayerJob, taxpayerIncome);
	else
	{
		printf(" the key must be unique");
		getchar();
		return node;
	}
	
	// compute height of this node
	node->height = max(height(node->left), height(node->right)) + 1;
	
	// check if tree is balanced
	balance = treeBalance(node);
	
	// check if subtree is balanced and checl if values are between [-1; 0; 1]
	if (balance >= -1 && balance <= 1)
	{
		return node;
	}
	
	
	
	// when subtree is unbalanced goes trough 4 different ways to balance it
	if (balance > 1 && taxpayerIncomeNumber < node->left->incomeNumber)
	{
		// left->left case
		taxPayer = rightRotation(node);
		
		// return node after the rotation in completed
		return taxPayer;
	}
	else if (balance < -1 && taxpayerIncomeNumber > node->right->incomeNumber)
	{
		
		// right->right case
		taxPayer = leftRotation(node);
		
		// return node after rotation
		return taxPayer;
	}
	else if (balance > 1 && taxpayerIncomeNumber > node->left->incomeNumber)
	{

		// left->right case
		node->left = leftRotation(node->left);
		taxPayer = rightRotation(node);
		
		
		// return node after rotation
		return taxPayer;
	}
	else if (balance < -1 && taxpayerIncomeNumber < node->right->incomeNumber)
	{
		
		// right->left case
		node->right = rightRotation(node->right);
		taxPayer = leftRotation(node);
		
		
		// return node after rotation
		return taxPayer;
	}
	else
	{
		// in case there's no valid cases
		printf("\n unknown case...");
		getchar();
		return node;
	}	
}

void showTaxPayers_inOrder (TaxPayer *node, char *city)
{
	//stop recursion
	if (node == NULL)
		return;
	
	//show data and does recursive call		
	showTaxPayers_inOrder(node->left, city);
	
	// print the persons information	
	if (strcmp(node->city, city) == 0)
			printf("\n Income Number: %d | Name: %s | City: %s | Job: %s | Total Income %.2f \n", 
		node->incomeNumber, node->name, node->city, node->job, node->income);
		
	//show data and does recursive call	
	showTaxPayers_inOrder(node->right, city);
}



// remove a node from the tree
TaxPayer *deleteTaxPayer(TaxPayer *root, int incomeNumber)
{
	//variables
	TaxPayer *taxPayer;
	int balance;
	
	// in case the tree is empty
	if (root == NULL)
		return root;
		
	// recursion to find the node that wants to be deleted
	if (incomeNumber < root->incomeNumber)
		root->left = deleteTaxPayer(root->left, incomeNumber);
	else if (incomeNumber > root->incomeNumber)
		root->right = deleteTaxPayer(root->right, incomeNumber);
	else
	{
		// when node to be deleted is found
		if (root->left == NULL && root->right == NULL)
		{
			// when node is a leaf
			taxPayer = root;
			root = NULL;
			free(taxPayer);
		}
		else if (root->left == NULL || root->right == NULL)
		{
			// case when there's only one child or no childs
			taxPayer = root->left != NULL ? root->left : root->right;
			root = taxPayer;
			free(taxPayer);
		}
		else
		{
			// case when a node has both childs
			taxPayer = minValueNode(root->right);
			root->incomeNumber = taxPayer->incomeNumber;						// copy its contents to the root
			strcpy(root->city, taxPayer->city);
			strcpy(root->name, taxPayer->name);
			strcpy(root->job, taxPayer->job);
			root->income = taxPayer->income;											
			root->right = deleteTaxPayer(root->right, taxPayer->incomeNumber);	// delete successor
			
		}		
	}
	
	// case the subtree is empty
	if (root == NULL)
		return root;
		
	// compute height of certain node
	root->height = max(height(root->left), height(root->right)) + 1;
	
	//recheck the tree balance
	balance = treeBalance(root);
	
	// if the subtree is unbalanced goes through one of the 4 cases below
	if (balance > 1 && treeBalance(root->left) >= 0)
	{
		// left->left case
		taxPayer = rightRotation(root);
		
		// return the node when the rotation is finished 
		return taxPayer;
	}
	else if (balance < -1 && treeBalance(root->right) <= 0)
	{
		// right->right case
		taxPayer = leftRotation(root);
		
		// return the node when the rotation is finished 
		return taxPayer;
	}
	else if (balance > 1 && treeBalance(root->left) < 0)
	{
		//left->right case
		root->left = leftRotation(root->left);
		taxPayer = rightRotation(root);
		
		// return the node when the rotation is finished 
		return taxPayer;
	}
	else if (balance < -1 && treeBalance(root->right) > 0)
	{
		// right->left case
		root->right = rightRotation(root->right);
		taxPayer = leftRotation(root);
		
		// return the node when the rotation is finished 
		return taxPayer;
	}
	else
	{
		// incase if tree is already balanced
		printf("\n ----- Tree is balanced ---- \n");
		return root;
	}
	
	
}

// Update the tax payer information
TaxPayer *updateTaxPayer(TaxPayer *root, int incomeNumber, char *city, char *job, float income)
{	
	// in case the tree is empty
	if (root == NULL)
		return root;
		
	// recursion to find the node that wants to be deleted
	if (incomeNumber < root->incomeNumber)
		root->left = updateTaxPayer(root->left, incomeNumber, city, job, income);
	else if (incomeNumber > root->incomeNumber)
		root->right = updateTaxPayer(root->right, incomeNumber, city, job, income);
	else if(incomeNumber == root->incomeNumber)
	{
		// update the tax payer city, job and income
		strcpy(root->city, city);
		strcpy(root->job, job);
		root->income = income;
	}	
	else
	{
		// incase tax payer number is invalid
		printf("\n unkown tax payer number...");
		return root;
	}
	
	//return updated root
	return root;
}

// calculate the total of persons that have that job inicially asked
int countJob(TaxPayer *node, char *job)
{
	// variables
	int count = 0;
	
	//search person(s) with the job asked
	if(node == NULL)
		return 0;
	else
	{
		if(strcmp(node->job, job) == 0)
			count++;
	} 
	
	//return to total of jobs
	return count + countJob(node->left, job) + countJob(node->right, job);
}

// calculate the total income from all the person with the job asked
float totalIncome(TaxPayer *node, char *job)
{
	// variables
	float total = 0;
	
	//search person(s) with the job asked
	if(node == NULL)
		return 0;
	else
	{
		if(strcmp(node->job, job) == 0)
			total += node->income;
	} 
	
	//return total income
	return total + totalIncome(node->left, job) + totalIncome(node->right, job);
}

// calculate average
float average (float total, int numberElements)
{
	return (float) total / (float) numberElements;
}



int main(int argc, char *argv[]) {

	// variables
	TaxPayer *taxPayers;
	
	taxPayers = NULL;
	
	// add some tax payers to the tree
	printf("Adding tax payers to the tree\n");
	taxPayers = newTaxPayer (taxPayers, 1030,"Joaquim Manuel", "Porto", "Professor", 3583.4);
	taxPayers = newTaxPayer (taxPayers, 1486,"Manel Fernandes", "Braga", "Engenheiro", 2143.2);
	taxPayers = newTaxPayer (taxPayers, 1058,"Alberto Rodrigues", "Faro", "Engenheiro", 1538.7);
	taxPayers = newTaxPayer (taxPayers, 1148,"Maria Freitas", "Beja", "Agricultora", 1816.3);
	taxPayers = newTaxPayer (taxPayers, 1793,"Manuela Castro", "Madeira", "Professora", 2581.5);
	taxPayers = newTaxPayer (taxPayers, 2105,"Jose Goncalves", "Acores", "Motorista", 947.2);
	taxPayers = newTaxPayer (taxPayers, 3496,"Jacinto Godofredo", "Castelo branco", "Engenheiro", 2037.1);
	taxPayers = newTaxPayer (taxPayers, 2049, "Catarina Pinhas", "Braga", "Agricultora", 2758.9);
	
	showTaxPayers_preOrder(taxPayers);
	
	printf("\n---------------------------------\n\n");
	printf(" Show List of people from Braga from ascending order \n");
	puts(" ");
	
	showTaxPayers_inOrder(taxPayers, "Braga");

	printf("\n---------------------------------\n\n");
	printf(" Delete a tax payer from the tree");
	puts(" ");
	
	taxPayers = deleteTaxPayer(taxPayers, 1148);
	
	printf("\n---------------------------------\n\n");
	printf(" show all the tax payers tree with without the deleted ");
	puts(" ");

	showTaxPayers_preOrder(taxPayers);
	
	printf("\n---------------------------------\n\n");
	printf(" Update the person with the Income Number of 2105 \n");
	puts(" ");
	
	updateTaxPayer(taxPayers, 2105, "Lisboa", "Carpinteiro", 1684.3);
	
	printf("\n---------------------------------\n\n");
	printf(" Show all tax Payers list with updated person \n");
	puts(" ");

	showTaxPayers_preOrder(taxPayers);
	
	printf("\n---------------------------------\n\n");
	printf(" Calculate the average of all people with the Engenheiro job \n");
	puts(" ");
	
	printf(" Average income is %.2f euros for the profession: %s\n\n", average (totalIncome(taxPayers, "Engenheiro"), countJob(taxPayers, "Engenheiro")), "Engenheiro");
	
	
	return 0;
}
