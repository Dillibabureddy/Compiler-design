#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 20

int main()
{
    char pro[SIZE], alpha[SIZE], beta[SIZE];
    int nont_terminal,i,j, index=3;

    printf("Enter the Production as :");
    scanf("%s", pro);

    nont_terminal=pro[0];
    if(nont_terminal==pro[index]) //Checking if the Grammar is LEFT RECURSIVE
    {
        //Getting Alpha
        for(i=++index,j=0;pro[i]!='|';i++,j++){
            alpha[j]=pro[i];
            //Checking if there is NO Vertical Bar (|)
            if(pro[i+1]==0){
                printf("This Grammar CAN'T BE REDUCED.\n");
                exit(0); //Exit the Program
            }
        }
        alpha[j]='\0'; //String Ending NULL Character

        if(pro[++i]!=0) //Checking if there is Character after Vertical Bar (|)
        {
            //Getting Beta
            for(j=i,i=0;pro[j]!='\0';i++,j++){
                beta[i]=pro[j];
            }
            beta[i]='\0'; //String Ending NULL character

            //Showing Output without LEFT RECURSION
            printf("\nGrammar Without Left Recursion: \n\n");
            printf(" %c->%s%c'\n", nont_terminal,beta,nont_terminal);
            printf(" %c'->%s%c'|#\n", nont_terminal,alpha,nont_terminal);
        }
        else
            printf("This Grammar CAN'T be REDUCED.\n");
    }
    else
        printf("\n This Grammar is not LEFT RECURSIVE.\n");
}