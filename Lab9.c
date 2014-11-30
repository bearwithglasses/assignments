/* Renee Prescilla
	Lab #9
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define ARRAYSIZE 20
#define ADD 1
#define SEARCH 2
#define DISPLAY 3
#define QUIT 4
#define READB 5

#define NODE struct node
#define LIST struct list

//Initializes the structure and the next node
struct node{
	char name[ARRAYSIZE];
	int number;
    NODE *next;
};

//created another structure for the head and tail node
struct list{
	NODE *head[4];
	NODE *tail[4];
};

pthread_mutex_t mutex;

//Prototypes
int main(int argc, char *argv[ ]);
void insert(LIST *waitlist, char *name, int number);
void search(LIST *waitlist);
void display(LIST *waitlist);
void *autosaver(void *waitlist);
void readfromfile(char *filename, LIST *waitlist);
void writetofile(char *filename, LIST *WAITLIST);
void displayb();

int main(int argc, char *argv[ ])
{
	//input from scanf()
	LIST *waitlist = (LIST*)malloc(sizeof(LIST));
	int i;
	for(i=0;i<4;i++){
		waitlist->head[i] = NULL;
		waitlist->tail[i] = NULL;
		}

	pthread_t thr;
	pthread_mutex_init(&mutex,NULL); //initialize

	int command;
    char name[ARRAYSIZE];
	int number;
    
    //add names and numbers from the text file
    //if the file doesnt exist, return error
    if (argc == 1){
        printf("The name of the file is missing!\n");
        return 0;
    }
    else{
    	printf("Test 1\n");
        readfromfile(argv[1],waitlist); //reads from the txt file + adds to list
    	pthread_create(&thr, NULL, autosaver, (void*)waitlist);
    	printf("Autosave Function worked!\n");
    }

	while(1)
	{
		printf("\n\nDinosaur Diner Waitlist\n\n");
		printf("Here are the options that you can do:\n\n");
		printf("\t1: Add Person and Info\n");
		printf("\t2: Delete Person\n");
		printf("\t3: Display List\n");
		printf("\t4: Quit\n");
		printf("\t5:  Display Autosave");
		printf("\n");

		printf("Type 1, 2, 3, 4, or 5: ");
		scanf("%d",&command);

		switch(command)
		{

			case ADD:
                //gets user input
                printf("\nType the person's info in this format: Name Tablenumber\n");
                printf("Enter information here: ");
                scanf("%s %d",name,&number);
                
                pthread_mutex_lock(&mutex);
                insert(waitlist,name,number);
                pthread_mutex_unlock(&mutex);
                break;

			case SEARCH:
				pthread_mutex_lock(&mutex);
				search(waitlist);
				pthread_mutex_unlock(&mutex);
				break;

			case DISPLAY:
				pthread_mutex_lock(&mutex);
				display(waitlist);
				pthread_mutex_unlock(&mutex);
				break;

			case QUIT:
				pthread_cancel(thr);
                writetofile(argv[1],waitlist);
				return 0;
				break;

			case READB:
				displayb();
				break;

			default:
				break;

		}

	}
	return 0;

}

void *autosaver(void *waitlist){

	printf("Test 2\n");
	LIST *list = (LIST *)waitlist;
	int i;

	while(1){

		printf("Test 3\n");

		pthread_mutex_lock(&mutex);
		FILE *bp = fopen("binary.bin","wb");
		if (bp == NULL){
			printf("Cannot open binary.bin\n");
		}

		//FILE *bp = fopen("binary.bin","wb");

		printf("Test 4\n");

		for(i=0;i<4;i++){
			printf("Test 5\n");
			fflush(stdout);
			while(list->head[i]!=NULL){
				fwrite(list->head[i],sizeof(NODE),1,bp);
				list->head[i] = list->head[i]->next;
				}
			printf("Test 6\n");
		}

		fclose(bp);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}

}

void displayb(char *filename){

	NODE temp;
	pthread_mutex_lock(&mutex);
	FILE *bp = fopen(filename,"rb");

	while(fread(&temp,sizeof(NODE),1,bp) == 1){

		printf("name: %s tablenumber: %d",temp.name,temp.number);
	}

	fclose(bp);
	pthread_mutex_unlock(&mutex);

}

void readfromfile(char *filename, LIST *waitlist){
    
    char name[ARRAYSIZE];
    int size;
    FILE * fp=fopen(filename,"r");
    while ( fscanf(fp,"%s %d",name,&size)!=EOF ) {
        insert(waitlist,name,size);
    }
    
    fclose(fp);
 
}

void writetofile(char *filename, LIST *waitlist){
    int i;
    
    FILE * fp=fopen(filename,"w");
    
    for(i=0;i<4;i++){
        
	    NODE *curr = waitlist->head[i];
	    while(curr != NULL){
	        fprintf(fp,"%s %d\n",curr->name,curr->number);
	        curr = curr->next;
	    }
	}
    
    fclose(fp);
    
}

void insert(LIST *waitlist,char *name, int number){
    
    int list;
	NODE *new =(NODE*)malloc(sizeof(NODE));

    //set the temp node's variables
    strcpy(new->name,name);
    new->number = number;
	new->next = NULL;

	if (new->number == 1 || new->number == 2){
		list = 0;
	}
	else if (new->number == 3 || new->number == 4){
		list = 1;
	}
	else if (new->number == 5 || new->number == 6){
		list = 2;
	}
	else if (new->number >= 7){
		list = 3;
	}

    NODE *curr = waitlist->head[list];

    //checks if head is null [list is empty]
    if(waitlist->head[list] == NULL){
        waitlist->head[list] = new;
		waitlist->tail[list] = new;
		//printf("\n\nThe person has been added!\n\n------------------\n\n");
        return;
    }
    //else, assign to tail and more tail pointer
    else{
		//checkes for duplicates
    	while(curr != NULL){
     	   if (!strcmp(name,curr->name)){
     	       printf("\nThere is a duplicate! You cannot add this person.\n");
     	       return;
     	   }
     	   else{
     	       curr = curr -> next;
      	  }
   		}

        waitlist->tail[list]->next = new;
        waitlist->tail[list] = new;
    }

	//printf("\n\nThe person has been added!\n\n------------------\n\n");
}


void search(LIST *waitlist){
	int deletetable;
    int list;

	printf("\nEnter the table size that you would like to remove: ");
	scanf("%d",&deletetable);

    if (deletetable == 1 || deletetable == 2){
		list = 0;
	}
	else if (deletetable == 3 || deletetable == 4){
		list = 1;
	}
	else if (deletetable== 5 || deletetable == 6){
		list = 2;
	}
	else if (deletetable >= 7){
		list = 3;
	}

	//if the current list is still NULL (it is empty),
	//then it will go to the previous lists until it finds an unempty one
	//and will delete from that list
	while(waitlist->head[list]==NULL){
		list=list-1;
	}

	NODE *prev = waitlist->head[list];
    NODE *curr = waitlist->head[list]->next;

    //checks if the number to be removed is the same as head
    //then just removes the head
    if (waitlist->head[list]->number <= deletetable){

    	printf("\n\t%s's table of %d will be seated now.\n",prev->name,prev->number);

		//if there is only one element in the list,
		//then the head and tail of the list of the same
		//the tail must point to NULL, and the head can be freed
		if(waitlist->head[list] == waitlist->tail[list]){
					waitlist->tail[list] = NULL;
				}

		free(waitlist->head[list]);
        waitlist->head[list] = curr;

    }

    //otherwise, if the current isnt null [end of the list]
    else{

        while (curr != NULL){

			//if the current number is less than or equal to the requested table number
			if (curr->number <= deletetable){

				printf("%s's table of %d will be seated now.\n",curr->name,curr->number);

                prev->next = curr->next; //sets the prev->next to the current->next
                                        //so prev->next no longer goes to current

				if(curr == waitlist->tail[list]){
					waitlist->tail[list] = prev;
				}

                free(curr); //frees current and doesnt alter list [nothing links to it]
				return;
                }
            }

        }
    printf("\n\n------------------\n\n");
}

void display(LIST *waitlist){

	int i;

	printf("\n------------------\n");
	printf("\nCurrent Dinosaur Dino Waitlist\n\n");

	for(i=0;i<4;i++){

	    NODE *curr = waitlist->head[i];
	    printf("List %d \n",i+1);
	    if (curr == NULL){
	    	printf("\n\tThis list is currently empty.\n");
	    }
	    while(curr != NULL){
	        printf("\n\tName: %s\n",curr->name);
	        printf("\tTable Size: %d\n\n",curr->number);
	        curr = curr->next;
	    }
	}

	printf("\n\n------------------\n");
}
