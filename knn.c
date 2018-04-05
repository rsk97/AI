#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
	struct Node* next;
	int k_value,index,class;
	float cost;
}node;

node* front = NULL;
float class_count[10];
int trainsize = 6668;
int testsize = 3332;
int count = 193;
int label[2215];
float **dist,**dist_test;

node* init(float cost,int k,int c){

	node* temp = (node*)malloc(sizeof(node));
	temp->cost = cost;
	temp->k_value = k;
	temp->class = c;
	temp->next = NULL;
	return temp;
}

void push(node* elem){
	int no=1;
	if(front == NULL)front = elem;
	else{
		if(front->cost > elem ->cost){
			elem->next = front;
			front = elem;
		}
		else{
		node* temp = front;
		while(temp->next!=NULL && temp->next->cost < elem->cost){
			temp = temp->next;
		}
		elem->next = temp->next;
		temp->next = elem;
		}
	}
}

float ** readFromCSV(char filename[],int totalLines){
	float **ar = (float**)malloc(totalLines*sizeof(float*));
	FILE * F = fopen(filename,"r");
	int i = 0,m=0;
	char buf[1000];
	char *str;
	fgets(buf, sizeof(buf), F);
	while(fgets(buf, sizeof(buf), F) != NULL){
		str = strtok(buf,",");
		ar[i]=(float *)malloc(count*sizeof(float));	
		int j = 0;
		while(str!=NULL){	
			ar[i][j++] = atof(str);
			str = strtok(NULL,",");
		}
		i++;
	}
	fclose(F);
	return ar;
}

int vote_class(){
	int i,max = 0,index,j=0;
	for (i = 0; i < 10; ++i){
		class_count[i]=0;
	}
	node *t = front;
	int ind = 0;
	while(ind < t->k_value){
		//printf("index %d class %d cost %f\n",ind,t->class,t->cost );
		//class_count[t->class]+=1/t->cost;
		class_count[t->class]++;
		t = t->next;
		ind++;
	}
	for (i = 0; i < 10; ++i){
		if(max < class_count[i]){
			max = class_count[i];
			index = i;
		}
	}
	return index;
}

void kNN(int k,float **test,float **train){
	int i,j,l,acc=0,output,cost;
	for(i=0;i<testsize;i++){front =NULL;
		for(j=0;j<trainsize;j++){
			cost = 0;
			if(dist_test[i][j] == 0){
				for (l = 0; l< count-1; ++l){
					cost += pow(test[i][l]-train[j][l],2);
				}
				dist_test[i][j] = sqrt(cost);
				
			}
		cost = sqrt(cost);
		node *f = init(cost,k,train[j][192]);
		push(f);	
		}
		output = vote_class();
		//printf(" pred :%d true_class: %f \n",output,test[i][192]);
		if(output == test[i][192])
			acc++;
	}
	float acc2 = (float) ((acc*100)/testsize) ;
	printf("The Accuracy for k = %d on Testset is %.4f\n", k,acc2);
}

float kNN_check(int k,float **train,int low,int high){
	int i,j,l,acc=0,output;
	float cost;
	int val_size = high - low;
	for(i=low;i< high;i++){front = NULL;
		for(j=0;j<trainsize;j++){
			cost = 0;		
			if(j == low) j = high;
			if(j == 6668) break;
			//printf("i = %d j = %d class i %.0f class j %.0f",i,j,train[i][192],train[j][192] );
			if(dist[i][j] == 0){
				for (l = 0; l< count-1; ++l){
					cost += pow(train[i][l]-train[j][l],2);
				}
				dist[i][j] = sqrt(cost);
				dist[j][i] = sqrt(cost);
			}
		cost = dist[i][j];
		node *f = init(cost,k,train[j][192]);
		push(f);
		//printf(" cost %f \n", cost);
		
		//free(front);;
		}
		//printf("example is %f",train[i][192]);
		output = vote_class();
		//printf(" pred %d\n", output);
		if(output == train[i][192]){
			acc++;
		}
	}
	float acc2 = (float) (acc*100)/val_size ;
	return acc2;
}

float max_val(float a,float b,float c){
	 if(a>b && a>c) return a;
	 if(b>a && b>c) return b;
	 if(c>a && c>b) return c;
	 return a;
}

int fold_validation(float **train){
	printf("Cross-Validation is on Progress\n");
	int k,low=0,high,kfold;
	float a,b,c,store_prev = 0 ,store_curr;
	high = 2223;
	for(k=1;k<26;k++){
		printf("k = %d\n",k );
		a=kNN_check(k,train,low,high);
		printf("VAL 1 OVER acc:%f\n",a );
		b=kNN_check(k,train,high,2*high);
		printf("VAL 2 OVER acc:%f\n",b );
		c=kNN_check(k,train,2*high,trainsize);
		printf("VAL 3 OVER acc:%f\n",c );
		//store_curr = max_val(a,b,c);
        store_curr = (a+b+c)/3;
		if(store_curr > store_prev){
			store_prev = store_curr;
			kfold = k;
		}
		printf("acc :%f for k = %d\n",store_curr,k);
	}
	return kfold;
}

int main(){
	float **train,k,**test;
	int i,j;
	train = readFromCSV("shuffled.csv",6670);
	test = readFromCSV("test.csv",3334);
	dist = (float**)malloc(6670*sizeof(float*));
	dist_test = (float**)malloc(3334*sizeof(float*));
	for(i = 0;i < 6670;i++){
		dist[i]=(float *)malloc(6670*sizeof(float));
		for (j = 0; j < 6670; ++j){
			dist[i][j] = 0;
		}
	}
	for(i = 0;i < 3334;i++){
		dist_test[i]=(float *)malloc(6670*sizeof(float));
		for (j = 0; j < 6670; ++j){
			dist_test[i][j] = 0;
		}
	}
	//uncomment the below line to perform cross validation
	//k = fold_validation(train);

	k =5;
	kNN(k,test,train);
	return 0;
}
