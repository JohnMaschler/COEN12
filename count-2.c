//Johnathon Maschler COEN 12 Professor Amer Lab 1 Thurs 9:15

/*The code accepts a text file as an argument from the user. The program then
reads the file and increments a counter for each word in the file. The program
then outputs the final number of words.
 */
#include <stdio.h>

#define MAXWORDLENGTH 30

int main(int argc, char *argv[])//allows user to pass text file as an argument
{
    FILE *fp;
    int wordCount=0;//
    char word[MAXWORDLENGTH];
    if (argc!=2)//confirm that a file was passed
    {
        printf("error\n");
        return 0;
    }
    char *filename = argv[1];
    fp = fopen(filename, "r");//open and read the file
    if (fp==NULL)//confirm the file is not empty
    {
        printf("error\n");
        return 0;
    }

    while (fscanf(fp, "%s", word)==1)//increment counter for each string in the file
        wordCount++;

    printf("%d total words\n", wordCount);
    fclose(fp);//close the file
    return 0;
}