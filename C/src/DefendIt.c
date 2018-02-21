#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <regex.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
//#include <DefendIt.h>
//#include <crypt.h>

//Command for compile in terminal
//gcc -pedantic -Wall -Wextra -Werror DefendIt.c

static char name[50];
static char pass[10];
char input[50];
char output[50];

static long long passedInt1, passedInt2;

static void readName(char * name)
{
   printf("Please enter your name: \n");
   fgets(name, 50, stdin);
   //strip(name);
}

static int testName(char * name)
{
    if(strlen(name) > 50)
    {
        return 1;
    }
    return 0;
}

static void readpass(char * pass)
{
   printf("Please enter a password of length 10 and includes at least one upper case character, one lower case character, one digit, one punctuation mark: \n");
   fgets(pass, 10, stdin);
   //strip(pass);
}

static int testPass(char * pass)
{
    regex_t regex;
    int reti;
    reti = regcomp(&regex, "^((?=.\\d)(?=.[a-z])(?=.[A-Z])(?=.[!@#$%]).{10})$", 0);
    if(reti)
    {
        -1;
    }
    reti = regexec(&regex, pass, 1, 1, 0);
    if(!reti)
    {
        return 0;
    }
    else
    {
        printf("password does not meet requirement");
        return 1;
    }
}

static int verify(char * pass)
{
  char *result;
  int ok;

  result = crypt(pass, encryption(pass));
  ok = strcmp (result, pass) == 0;

  puts(ok ? "Access granted." : "Access denied.");
  return ok ? 0 : 1;
}

static int encryption(char * pass)
{
  unsigned long seed[2];
  char salt[] = "$1$........";
  const char alpha[65] = {"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
  int i;

  // Generate a (not very) random seed? Rand() was not suggested?
  //Tom has CryptGenRandom on Penguin?
  seed[0] = time(NULL);
  seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

  for (i = 0; i < 10; i++)
    salt[3+i] = alpha[(seed[i/5] >> (i%5)*6) &0x3f];


  pass = crypt(pass, salt);
  puts(pass);
  return 0;
}

//Source code modified from https://wiki.sei.cmu.edu/confluence/display/c/ERR34-C.+Detect+errors+when+converting+a+string+to+a+number
//and from https://www.techonthenet.com/c_language/standard_library_functions/stdlib_h/strtoll.php
static long long verifyIntType(const char *buff) {
    long long pInt = (long long)NULL;
    char *ptr = NULL;

    if (buff) {
        pInt = strtoll(buff, (char **) ptr, 10);
        if (pInt == 0)
        {
            /* If a conversion error occurred, display a message and exit */
            if (errno == EINVAL) {
                printf("Invalid integer type, were there some weird characters or something?\n");
            }
        }
    } else {
        printf("Not a valid integer type... did you have any letters or special symbols in there?");
    }

    return pInt;
}

static long long getInt(){

    char* input[100];
    printf("Enter an integer between -2147483648 and 2147483647\n");
    fgets((char *) input, 100, stdin);

    return verifyIntType((const char *) input);
}

static int checkInt(long long input){

    if(input < -2147483648 || input > 2147483648){
        printf("Invalid range\n");
        return 0;
    }
    return 1;

}

//File verification code modified from https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
static FILE* openFileRead(const char* fileName){

    if( access( fileName, F_OK ) != -1 ) {
        return fopen(fileName, (const char *) 'r');
    } else {
        printf("File does not exist.");
    }
    return NULL;

}

static FILE* openFileWrite(const char* fileName){

    if( access( fileName, F_OK ) != -1 ) {
        printf("Cannot overwrite existing file.");
    } else {
        return fopen(fileName, "w");
    }
    return NULL;

}

static void readInput(char * input)
{
    int len;
    printf("\nPlease enter the name of an Input File (Must be .txt, must already exist, the only special characters allowed are underscores and dashes, and must be in the current directory):\n");
    fgets(input, 50, stdin);
    len = strlen(input);
    if(input[len-1] == '\n' )
        input[len-1] = 0;
}

static bool checkInput(char * input)
{
    struct stat buffer;
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^([a-zA-Z_ -]+)(.txt)$", REG_EXTENDED|REG_NOSUB) != 0)
    {
        printf("\nregcomp() failed, returning nonzero\n");
        return false;
    }
    ret = regexec(&regex, input, 1, pmatch, 0);
    if(ret == 0) //match found
    {
        int exists = stat(input, &buffer);
        if(exists == 0)//file must exist for input
        {
            return true;
        }
        else
        {
            printf("\nInput file does not exist\n");
            return false;
        }
    }
    else
    {
        printf("\nInput file does not meet requirements\n");
        return false;
    }
}

static void readOutput(char * output)
{
    int len;
    printf("Please enter the name of an Output File (Must be .txt, must not already exist, the only special characters allowed are underscores and dashes, and must be directed to the current directory): \n");
    fgets(output, 50, stdin);
    len = strlen(output);
    if(output[len-1] == '\n' )
        output[len-1] = 0;
}

static int checkOutput(char * output)
{
    struct stat buffer;
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^([a-zA-Z_ -]+)(.txt)$", REG_EXTENDED|REG_NOSUB) != 0)
    {
        printf("\nregcomp() failed, returning nonzero\n");
        return false;
    }
    ret = regexec(&regex, output, 1, pmatch, 0);
    if(ret == 0) //match found
    {
        int exists = stat(output, &buffer);
        if(exists != 0)//file must not exist for input
        {
            return true;
        }
        else
        {
            printf("\nOutput file does exist\n");
            return false;
        }
    }
    else
    {
        printf("\nOutput file does not meet requirements\n");
        return false;
    }
}

int main(int argc, const char argv[]){

    passedInt1 = getInt();

    while(!checkInt(passedInt1)){
        passedInt1 = getInt();
    }

    passedInt2 = getInt();

    while(!checkInt(passedInt2)){
        passedInt2 = getInt();
    }

    readInput(input);
    bool inputcheck = checkInput(input);
    while(inputcheck == false)
    {
        printf("\nInput file name is invalid. Please try again.\n");
        memset(input, 0, sizeof input);
        readInput(input);
        inputcheck = checkInput(input);
    }

    readOutput(output);
    bool outputcheck = checkOutput(output);
    while(outputcheck == false)
    {
        printf("\nOutput file name is invalid. Please try again.\n");
        memset(output, 0, sizeof output);
        readOutput(output);
        outputcheck = checkOutput(output);
    }

    FILE* inputFile = openFileRead(input);

    FILE* outputFile = openFileWrite(output);

    fputs(name, outputFile);
    fputs(pass, outputFile);
    fputs((const char *) (passedInt1 + passedInt2), outputFile);
    fputs((const char *) (passedInt1 * passedInt2), outputFile);

    int c;
    while((c = getc(inputFile)) != EOF){
        fputc(c, outputFile);
    }

    fclose(outputFile);
    fclose(inputFile);

}