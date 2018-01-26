#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define N 4
int mx[4] = {1,0,-1,0};
int my[4] = {0,1,0,-1};
// C++ program to check if a given instance of N*N-1
// puzzle is solvable or not
#include <stdio.h>
int getInvCount(int arr[])
{
	int inv_count = 0,i,j;
	for (i = 0; i < N * N - 1; i++)
	{
		for (j = i + 1; j < N * N; j++)
		{
			// count pairs(i, j) such that i appears
			// before j, but i > j.
			if (arr[j] && arr[i] && arr[i] > arr[j])
				inv_count++;
		}
	}
	return inv_count;
}

// find Position of blank from bottom
int findXPosition(int puzzle[N][N])
{	int i,j;
	// start from bottom-right corner of matrix
	for (i = N - 1; i >= 0; i--)
		for (j = N - 1; j >= 0; j--)
			if (puzzle[i][j] == 0)
				return N - i;
}

// This function returns true if given
// instance of N*N - 1 puzzle is solvable
int isSolvable(int puzzle[N][N])
{
	// Count inversions in given puzzle
	int invCount = getInvCount((int*)puzzle);

	// If grid is odd, return true if inversion
	// count is even.
	if (N & 1)
		return !(invCount & 1);

	else	 // grid is even
	{
		int pos = findXPosition(puzzle);
		if (pos & 1)
			return !(invCount & 1);
		else
			return invCount & 1;
	}
}
typedef struct Node{
	int matrix[N][N];
	struct Node* next;
	struct Node* parent;
	int cost;
	int level;
	int x,y,par_cost;
}node;
node* front = NULL;
node* front_open = NULL;
void push(node* elem){
	if(front == NULL)front = elem;
	else{
		node* temp = front;
		while(temp->next!=NULL && temp->next->cost < elem->cost)
			temp = temp->next;
		elem->next = temp->next;
		temp->next = elem;
	}
}

void push_open(node *elem){
	//printf("IM out");
	if(front_open == NULL)front_open = elem;
	else{
		//printf("IM in");
		node* temp = front_open;
		while(temp->next!=NULL && temp->next->cost < elem->cost)
			temp = temp->next;
		elem->next = temp->next;
		temp->next = elem;
	}
}

node* getmin(){
	node* temp = front;
	front = front->next;
	return temp;
}
int caldif(int A[N][N],int B[N][N]){
	int cost = 0;
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			if(A[i][j] != B[i][j])cost++;
	return cost;
}
int get_openlist(node *ptr){
	node* temp = front_open;
	while(temp != NULL){
		if(caldif(temp->matrix,ptr->matrix) == 0)
			return 1;
		temp = temp->next; 
	}
	return 0;
}
int calman(int A[N][N],int B[N][N]){
	int p[N*N][2],q[N*N][2];
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++){
			p[A[i][j]][0] = i;
			p[A[i][j]][1] = j;
		} 
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++){
			q[B[i][j]][0] = i;
			q[B[i][j]][1] = j;
		} 
	int man = 0;
	for(int i=0;i<N*N;i++){
		man += abs(p[i][0]-q[i][0]) + abs(p[i][1]-q[i][1]);
	}
	return man;
}
node* init(int par[N][N],int fin[N][N],int x,int y,int nx,int ny,int lev,node* parent){
	node* temp = (node*)malloc(sizeof(node));
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			temp->matrix[i][j] = par[i][j];
	int tp = temp->matrix[x][y];
	temp->matrix[x][y] = temp->matrix[nx][ny];
	temp->matrix[nx][ny] = tp;
	temp->level = lev;
	temp->x = nx;
	temp->y  = ny;
	temp->cost = 0;
	temp->parent = parent;
	temp->par_cost = 0;
	temp->cost = caldif(temp->matrix,fin) + calman(temp->matrix,fin);
	return temp;
}
int check(int x,int y){
	if(x >= 0 && x < N && y >=0 && y < N)return 1;
	return 0;
}
void printmat(int mat[N][N]){
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			printf("%d ",mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}	
void ppath(node* head){
	if(head == NULL)return;
	ppath(head->parent);
	printmat(head->matrix);
}
void play(int initial[N][N],int fin[N][N],int x,int y){
	front = init(initial,fin,x,y,x,y,0,NULL);
	front_open = init(initial,fin,x,y,x,y,0,NULL);
	int cst=0;
	while(front != NULL){
		node* top = getmin();
		if(caldif(top->matrix,fin) == 0){
			ppath(top);
			return;
		}

		for(int  i  = 0;i < 4;i++){
			if(check(top->x+mx[i],top->y+my[i])){
				
				//printf("%d running",i);
				node* chd = init(top->matrix,fin,top->x,top->y,top->x+mx[i],top->y+my[i],top->level+1,top);
				//push_open(chd);
				//if(top->parent != NULL && caldif(top->parent->matrix,chd->matrix) == 0)continue;
				if(top->parent != NULL && get_openlist(chd) == 1) {
				//	printf("INs");
					continue;}
				
				chd->cost=chd->cost+chd->parent->par_cost+1;
				chd->par_cost = chd->parent->par_cost+1;
				push(chd);
				//printmat(chd->matrix);
			}
			
		}
		//printf("%d \n",cst);
		push_open(top);
	}
}
int main(){

	int initial[N][N]={{1,2,7,3},
			   {5,6,0,4},
			   {9,10,11,8},
			   {13,14,15,12}};
	int fin[N][N]={{1,2,3,4},
			   {5,6,7,8},
			   {9,10,11,12},
			   {13,14,15,0}};
/*int initial[N][N] = {{8,6,7},
{2,5,4},
{3,0,1}};
int fin[N][N] = {{1,2,3},
{4,5,6},
{7,8,0}};*/
	int x_c,y_c,i,j;
	for(i=0;i<N;i++)
	    for(j=0;j<N;j++)
		if(initial[i][j] == 0)
		   {x_c =i;
		    y_c =j;}
        if(isSolvable(initial)){
	   printf("THE PUZZLE IS SOLVABLE\n");
	   printf("THE BLANK IS AT %d %d\n",x_c,y_c);
 	   play(initial,fin,x_c,y_c);
	}
	else
	   printf("THE PUZZLE IS NOT SOLVABLE\n");

return 0;
	
}

/*{1,2,4},
			   {3,0,8},
			   {7,5,6}
{1,2,3},
			   {4,7,8},
			   {0,5,6}*/

