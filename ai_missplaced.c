/*
ARSHAD G ------------ 201601025
ROHAN S  ------------ 201601073
AI ASSIGNMENT - I ----------- 15 PUZZLE SOLVER USING  DISPLACED TILES HEURISTIC

*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define N 4

int mx[4] = {1,0,-1,0};
int my[4] = {0,1,0,-1};

int node_count = 0,node_count_cl = 0,path_count = 0;

int getInvCount(int arr[]){
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
int findXPosition(int puzzle[N][N]){
	int i,j;
	// start from bottom-right corner of matrix
	for (i = N - 1; i >= 0; i--)
		for (j = N - 1; j >= 0; j--)
			if (puzzle[i][j] == 0)
				return N - i;
}

// This function returns true if given
// instance of N*N - 1 puzzle is solvable
int isSolvable(int puzzle[N][N]){
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
	node_count++;
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
	//node_count_cl++;
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

void count_open_and_closed(){
	int c1=0,c2=0;
	if(front == NULL)printf("0 NODES REMAIN IN OPEN LIST\n");
	if(front_open == NULL)printf("0 NODES REMAIN IN CLOSED LIST\n");
	else{
		node* temp1 = front;
		node* temp2 = front_open;
		while(temp1->next!=NULL){c1++;
			temp1 = temp1->next;}
		while(temp2->next!=NULL){c2++;
			temp2 = temp2->next;}
		printf("%d NODES REMAIN IN OPEN LIST \n%d NODES REMAIN IN CLOSED LIST\n",c1,c2);
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
int get_closedlist(node *ptr){
	node* temp = front;
	while(temp != NULL){
		if(caldif(temp->matrix,ptr->matrix) == 0){
			//temp->next = temp->next->next;
			return 1;}
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

int displacedtile(int A[N][N]){
	int cost = 0;
	int B[N][N] ={{1,2,3,4},
			   {5,6,7,8},
			   {9,10,11,12},
			   {13,14,15,0}};
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			if(A[i][j] != B[i][j]&& A[i][j])cost++;
	//cost += calman(A,B);
	return cost;
}

node* init(int par[N][N],int fin[N][N],int x,int y,int nx,int ny,int lev,node* parent,int heu_no){
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
	if(heu_no == 1)
		temp->cost =   calman(temp->matrix,fin);
	else
		temp->cost = displacedtile(temp->matrix);
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
	path_count++;
}

void play(int initial[N][N],int fin[N][N],int x,int y,int h){
	front = init(initial,fin,x,y,x,y,0,NULL,h);
	front_open = init(initial,fin,x,y,x,y,0,NULL,h);

	while(front != NULL){
		node* top = getmin();
		node_count_cl++;
		if(caldif(top->matrix,fin) == 0){
			ppath(top);
			return;
		}

		for(int  i  = 0;i < 4;i++){
			if(check(top->x+mx[i],top->y+my[i])){
				
				//printf("%d running",i);
				node* chd = init(top->matrix,fin,top->x,top->y,top->x+mx[i],top->y+my[i],top->level+1,top,h);
				//push_open(chd);
				if(top->parent != NULL && caldif(top->parent->matrix,chd->matrix) == 0)continue;
				if(top->parent != NULL && (get_openlist(chd) == 1 || get_closedlist(chd)==1 )) {
				//	printf("INs");
					continue;}
				
				//chd->cost=chd->cost+0.3*(chd->parent->par_cost)+1;
				chd->cost=chd->cost + chd->parent->par_cost + 1;				
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

// 0) 25 - 35 sec 1) 1-3 sec 
	//int initial[N][N]={{1,2,3,4}, {5,6,7,8}, {13,10,11,12},{9,15,14,0}};

// 0) & 1) IMMEDIATELY
	int initial[N][N]={{5,1,7,3}, {9,2,11,4}, {13,6,15,8},{0,10,14,12}};

//NOT SOLVABLE
//	int initial[N][N]={{0,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,1}};
//	int initial[N][N]={{3,9,1,15},{14,11,4,6},{13,0,10,12},{2,7,8,5}};

//	int initial[N][N]={{1,2,7,3},{5,6,0,4},{9,10,11,8},{13,14,15,12}};


	int fin[N][N]={{1,2,3,4},{5,6,7,8}, {9,10,11,12}, {13,14,15,0}};
/*
//takes 30+ steps to solve 
int initial[N][N] = {{8,6,7},
{2,5,4},
{3,0,1}};
int fin[N][N] = {{1,2,3},
{4,5,6},
{7,8,0}};*/
	int x_c,y_c,i,j,heu;
	for(i=0;i<N;i++)
	    for(j=0;j<N;j++)
		if(initial[i][j] == 0)
		   {x_c =i;
		    y_c =j;
		    break;}
        if(isSolvable(initial)){
	   printf("THE PUZZLE IS SOLVABLE\n");
	   printf("THE BLANK IS AT %d %d\n\n",x_c,y_c);
	   //scanf("%d",&heu);
	   //printf("THIS MAY TAKE SOME TIME PLEASE WAIT\n");
 	   play(initial,fin,x_c,y_c,0);
	   printf("THE NO OF NODES ADDED TO OPEN LIST - %d CLOSED LIST - %d\n\n",node_count,node_count_cl);
	   printf("NO OF STEPS TAKEN TO CONVERGE %d\n\n",path_count-1);
	   count_open_and_closed();
	}
	else
	   printf("THE PUZZLE IS NOT SOLVABLE \n");

return 0;
	
}

/*
//takes 15+ steps to solve
{5,1,7,3}, 
{9,2,11,4}, 
{13,6,15,8},
{0,10,14,12}

//takes 20+ steps to solve
{1,2,3,4}, 
{5,6,7,8}, 
{13,10,11,12},
{9,15,14,0}


//takes 50+ steps
{1,10,15,4},{13,6,3,8},{2,9,12,7},{14,5,0,11}

//takes 60+ steps
{2,5,13,12},
			   {1,0,3,15},
			   {9,7,14,6},
			   {10,11,8,4}

//
{6,2,8,4},
{12,14,1,10}, 
{13,15,3,9}, 
{11,0,5,7}
//
{5,8,7,11}, 
{1,6,12,2}, 
{9,0,13,10},
{14,3,4,15}
{1,2,4},
			   {3,0,8},
			   {7,5,6}
{1,2,3},
			   {4,7,8},
			   {0,5,6}*/

