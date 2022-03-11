#include <errno.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include "schedule.h"

void fifo(proc list[], int size){
	
	
    for(int i=0; i < size; i++) {
		if(i == 0){
        list[i].time_turnaround = list[i].length;
		}
		else{	
        // when 1 or more
        list[i].time_turnaround = list[i-1].time_turnaround + list[i].length;
        list[i].time_wait = list[i-1].time_turnaround;
        list[i].time_response = list[i-1].time_turnaround;
		}

    }
	
	
	float avgTurnaroundTime = 0;
	float avgResponseTime = 0;
	float avgWaitTime = 0;
	
	for(int x = 0; x < size; x++){
		avgTurnaroundTime += (float)list[x].time_turnaround;
		avgResponseTime += (float)list[x].time_response;
		avgWaitTime += (float)list[x].time_wait;
	}
	avgTurnaroundTime = avgTurnaroundTime / size;
	avgResponseTime = avgResponseTime / size;
	avgWaitTime = avgWaitTime / size;
	
	printf("\n");
	printf("Fifo: \n");
	printf("avg turnaround time: %.1f ms\n", avgTurnaroundTime);
	printf("avg response time: %.1f ms \n", avgResponseTime);
	printf("avg waits time: %.1f ms \n", avgWaitTime);
	printf("\n");
	
	
}

void sjf(proc list[], int size){
	int temp;
	for(int a = 0; a < size; a++){ 
		list[a].time_wait = 0;
		list[a].time_response = 0;
		list[a].time_turnaround = 0;
	
	}
	for(int i = 0; i < size - 1; i++){
		for(int x = 0; x < size - i -1 ; x++){
			if(list[x].length > list[x+1].length){
				temp = list[x].length;
				list[x].length = list[x+1].length;
				list[x+1].length = temp;
		}
		}
	}

	   for(int i = 0; i < size; i++) {
		if(i == 0){
        list[i].time_turnaround = list[i].length;
		}
		else if(i >= 1){	
        // when 1 or more
        list[i].time_turnaround = list[i-1].time_turnaround + list[i].length;
        list[i].time_wait = list[i-1].time_turnaround;
        list[i].time_response = list[i-1].time_turnaround;
		}

    }
	
	//calculate avgs and print them
	float avgTurnaroundTime = 0;
	float avgResponseTime = 0;
	float avgWaitTime = 0;
	
	for(int x = 0; x < size; x++){
		avgTurnaroundTime += (float)list[x].time_turnaround;
		avgResponseTime += (float)list[x].time_response;
		avgWaitTime += (float)list[x].time_wait;
	}
	avgTurnaroundTime = avgTurnaroundTime / size;
	avgResponseTime = avgResponseTime / size;
	avgWaitTime = avgWaitTime / size;
	
	printf("SJF: \n");
	printf("avg turnaround time: %.1f ms\n", avgTurnaroundTime);
	printf("avg response time: %.1f ms \n", avgResponseTime);
	printf("avg waits time: %.1f ms \n", avgWaitTime);
	printf("\n");
}


void roundRobin(proc list[], int size){
	
	int totalTime = 0;
	int timeQuantum = 10;
	int worked = 0;
	
	for(int a = 0; a < size; a++){ 
			list[a].time_wait = 0;
			list[a].time_response = 0;
			list[a].time_turnaround = 0;
	
	}
	for( int b = 0; b < size; b++){
		totalTime += list[b].length;
	}

	
	//it runs as soon as it starts
	list[0].time_response = 0;
	
	//calculate rest of response time
	for(int i = 0; i < size; i++){
		
		for(int x = i+1; x <size; x++){
			if(list[i].length < 10){
			list[x].time_response += list[i].length;
			//printf("at 1 \n");
			}
			else{
				list[x].time_response += timeQuantum;
				
			}
			
		}
		
	}
	
	//************
	int tmp = 0;
	while(worked < totalTime){
		
		for(int i = 0; i < size; i++){
			
			if(list[i].length == 0){
				//do nothing, the process is done

			}
			
			//finishing the last bit of a process: burst < 10
			else if(list[i].length <= timeQuantum ){
				
				worked += list[i].length;
				tmp = list[i].length;
				list[i].length = 0;
				//process is finished
				
				//update wait time of all other incomplete procs
				for(int x = 0; x < size; x++){
					if(x == i){
						//do nothing to wait time; the current process is running
						list[x].time_turnaround += tmp;
						
					}else if(list[x].length > 0){
						
						list[x].time_turnaround += tmp;
						list[x].time_wait += tmp;
						
						//printf("time: %d %d \n", x, list[x].time_wait);
					}
					
				}
				
					
			}
			else{
				
				worked += timeQuantum;
				
				list[i].length -= timeQuantum;
				
				//udpate wait times
				for(int x = 0; x < size; x++){
					if(x == i){
						//do nothing to wait time; the current process is running
						list[x].time_turnaround += timeQuantum;
					}else if(list[x].length > 0){
						list[x].time_turnaround += timeQuantum;
						list[x].time_wait += timeQuantum;
						
					}
				}
				
				
				
			}
			
		}
	}
	
	
	//Caluclate and print avgs
	float avgTurnaroundTime = 0;
	float avgResponseTime = 0;
	float avgWaitTime = 0;
	
	for(int x = 0; x < size; x++){
		avgTurnaroundTime += (float)list[x].time_turnaround;
		avgResponseTime += (float)list[x].time_response;
		avgWaitTime += (float)list[x].time_wait;
		
	}
	avgTurnaroundTime = avgTurnaroundTime / size;
	avgResponseTime = avgResponseTime / size;
	avgWaitTime = avgWaitTime / size;
	
	
	printf("Round Robin: \n");
	printf("avg tt: %.1f \n", avgTurnaroundTime);
	printf("avg rt: %.1f \n", avgResponseTime);
	printf("avg wt: %.1f \n", avgWaitTime);
	printf("\n");

}


void lottery(proc list[], int size){
	
	int totalTime = 0;
	int timeQuantum = 10;
	int worked = 0;
	int randomNum = 0;
	int runningProc = 0;
	
	//assign time to 0 for all times
	for(int a = 0; a < size; a++){ 
			list[a].time_wait = 0;
			list[a].time_response = 0;
			list[a].time_turnaround = 0;
	}
	
	//calculate total times
	for( int b = 0; b < size; b++){
		totalTime += list[b].length;
	}

	

	
	//Loop for doing procs
	
	while(worked < totalTime){
		
		randomNum = rand() % totalTime;
		
		int back = 0;
		int front = 0;
		
		for(int i = 0; i < size; i++){
			
			if(list[i].length > 0){  //if the process is finished don't include it in the lottery
			front += list[i].tickets - 1;
			if(randomNum > back && randomNum < front ){
				runningProc = i;
				if(list[i].complete == 0){
					list[i].complete = 1;
					list[i].time_response = worked;
				}
				
				
			}
			back += list[i].tickets - 1;
			}
		}
		
		int tmp = 0;
		
		if(list[runningProc].length == 0){
				//do nothing, the process is done
				
			}
			
			//finishing the last bit of a process: burst < 10
			else if(list[runningProc].length <= timeQuantum ){
				
				worked += list[runningProc].length;
				tmp = list[runningProc].length;
				list[runningProc].length = 0;
				
				//process is finished
				
				//update wait time of all other incomplete procs
				for(int x = 0; x < size; x++){
					if(x == runningProc){
						//do nothing to wait time; the current process is running
						list[x].time_turnaround += tmp;
						
					}else if(list[x].length > 0){
						
						list[x].time_turnaround += tmp;
						list[x].time_wait += tmp;

					}
					
				}
				
					
			}
			else{
				
				worked += timeQuantum;
				
				list[runningProc].length -= timeQuantum;
				
				//udpate wait times
				for(int x = 0; x < size; x++){
					if(x == runningProc){
						//do nothing to wait time; the current process is running
						list[x].time_turnaround += timeQuantum;
					}else if(list[x].length > 0){
						list[x].time_turnaround += timeQuantum;
						list[x].time_wait += timeQuantum;
						
					}
				}
				
			}

		
	}
	
	//calculat and print avgs
	float avgTurnaroundTime = 0;
	float avgResponseTime = 0;
	float avgWaitTime = 0;
	
	for(int x = 0; x < size; x++){
		avgTurnaroundTime += (float)list[x].time_turnaround;
		avgResponseTime += (float)list[x].time_response;
		avgWaitTime += (float)list[x].time_wait;
		
		
	}
	avgTurnaroundTime = avgTurnaroundTime / size;
	avgResponseTime = avgResponseTime / size;
	avgWaitTime = avgWaitTime / size;
	
	printf("Lottery: \n");
	printf("avg tt: %.1f \n", avgTurnaroundTime);
	printf("avg rt: %.1f \n", avgResponseTime);
	printf("avg wt: %.1f \n", avgWaitTime);
	printf("\n");
	
	
	
}