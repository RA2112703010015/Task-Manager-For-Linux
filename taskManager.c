#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <string.h>
#include <signal.h>

//Structure to store process details
struct ProcessInfo{
	pid_t PID;
	char pname[10];
	int priority;
	char *ArrivalTime;
	struct ProcessInfo* next;
}*head,*temp,*tp;

int pid;

//Insert data into list
void insertData(){
	if(head==NULL){
		head=temp;
	}
	else{
		tp=head;
		while(tp->next!=NULL){
			tp=tp->next;
		}
		tp->next=temp;
		
	}

}

//Fetches time of process creation
void getCurrentTime(){
    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    temp->ArrivalTime=time_str;
}

//Gets Process Name
void getName(char *s){
   const char c[2] = " ";
   char *token;
   /* walk through other tokens */
   while( token = strtok_r(s, c,&s)) {
      if(token[0]!='s'){
      	token[strlen(token)-1]='\0';
      	strcpy(temp->pname,token);
      	break;
      }
   }
}

// To create a task
void createTask(char *c){
	pid=fork();/*Creates a new process*/
	if(pid>0){
	 	temp=(struct ProcessInfo*)malloc(sizeof(struct ProcessInfo));
	 	temp->next=NULL;
	 	getName(c);
		temp->PID=pid;
		temp->priority=getpriority(getpid(),PRIO_PROCESS);
		getCurrentTime();
		insertData();
		printf("[+] %d\n",temp->PID);
	}
	else{
		wait(NULL);
		exit(0);
		
	}
}

//To display process info
void displayProcess(){
	tp=head;
	printf("\nProcessName\t\tPID\t\t\tArrival Time\t\t\tPriority\n");
	while(tp->next!=NULL){
		if(tp->PID!=0)
	   printf("%s\t\t\t%d\t\t\t%s\t%d\n",tp->pname,tp->PID,tp->ArrivalTime,tp->priority);
	   tp=tp->next;
	}
	printf("%s\t\t\t%d\t\t\t%s\t%d\n\n",tp->pname,tp->PID,tp->ArrivalTime,tp->priority);
}

//Kills a process
void killProcess(pid_t n){
	if(!pid) exit(0);
	kill(n,SIGKILL);
	tp=head;
//Removing the process from link
	while(tp->PID!=n){
		temp=tp;
		tp=tp->next;
	}
	temp->next=tp->next;
	free(tp);
}

//Prints the manual 
void printMAN(){
	printf("\nstart <process_name> : to start a process\n");
	printf("kill : to terminate a process\n");
	printf("disp : displays details of current processes\n");
	printf("man : shows list of all available commands\n");
	printf("CTRL + c : to exit out of the terminal\n\n");
}

/*Main Function*/
int main(){
	int n;
	head=(struct ProcessInfo*)malloc(sizeof(struct ProcessInfo));
	head->next=NULL;
	pid=1;
	char ch[100];
	
//Command Line Loop
	do{
		if(pid>0){
			printf(">>> ");
			fgets(ch,sizeof(ch),stdin);
				if(ch[0]=='s'){
					createTask(ch);
				}
				else if(ch[0]=='d'){
					displayProcess();
				}
				else if(ch[0]=='m'){
					printMAN();
				}
				else if(ch[0]=='k'){
					int id;
					printf("Enter PID : ");
					scanf("%d",&id);
					killProcess(id);
				}
				else if(ch[0]=='0'){
					exit(0);
				}
				else{
					printf("\nUse 'man' to find proper commands\n");
				}
			}
		else{
			wait(NULL);
			exit(0);
		}
	}while(1);
}