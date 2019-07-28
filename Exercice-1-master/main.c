/*
 *
 * Author : SOSSOU A.Daniel
 * Email : daniel1sossou@gmail.com
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
    pid_t pid1, pid2, pid3;

    pid1 = fork();

    if(pid1<0){
        printf("Error the first son haven't created\n");
    } else{
        if(pid1==0){
                printf("I'm the first son; pid : %d \n",getpid());
        } else{
            pid2 = fork();
            if(pid2<0){
                printf("Error the second son haven't created\n");
            } else{
                if(pid2==0){
                    printf("I'm the second son; pid : %d \n",getpid());
                } else{
                    pid3 = fork();
                    if(pid3<0){
                        printf("Error the third son haven't created\n");
                    } else{
                        if(pid3==0){
                            printf("I'm the third son; pid : %d\n",getpid());
                        }
                    }
                }
            }
        }
    }


    return 0;
}