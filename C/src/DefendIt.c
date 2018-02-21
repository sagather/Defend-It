#include <termios.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <regex.h>
//#include <DefendIt.h>
//#include <crypt.h>


char name[50];
char pass[10];

void readName(char * name)
{
   printf("Please enter your name: \n");
   fgets(name, 50, stdin);
   //strip(name);
}

int testName(char * name)
{
    if(strlen(name) > 50)
    {
        return 1;
    }
    return 0;
}

void readpass(char * pass)
{
   printf("Please enter a password of length 10 and includes at least one upper case character, one lower case character, one digit, one punctuation mark: \n");
   fgets(pass, 10, stdin);
   //strip(pass);
}

int testPass(char * pass)
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

int verify(char * pass)
{
  char *result;
  int ok;

  result = crypt(pass, encryption(pass));
  ok = strcmp (result, pass) == 0;

  puts(ok ? "Access granted." : "Access denied.");
  return ok ? 0 : 1;
}

int encryption(char * pass)
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

long long getInt(){

    char* input[100];
    printf("Enter an integer between -2147483648 and 2147483647\n");
    fgets(input, 100, stdin);

    return verifyInt(input);

}

//Source code modified from https://wiki.sei.cmu.edu/confluence/display/c/ERR34-C.+Detect+errors+when+converting+a+string+to+a+number
long long verifyInt(const char *buff) {
    int matches;
    long long pInt;

    if (buff) {
        matches = sscanf(buff, "%lld", &pInt);
        if (matches != 1) {
            printf("Not a valid integer type... did you have any letters or special symbols in there?");
        }
    } else {
        printf("Not a valid integer type... did you have any letters or special symbols in there?");
    }
}

//File verification code modified from https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
FILE* openFileRead(const char* fileName){

    if( access( fileName, F_OK ) != -1 ) {
        return fopen(fileName, 'r');
    } else {
        printf("File does not exist.");
    }

}

FILE* openFileWrite(const char* fileName){

    if( access( fileName, F_OK ) != -1 ) {
        printf("Cannot overwrite existing file.");
    } else {
        return fopen(fileName, "w");
    }

}

int main(int argc, const char argv[]){

    getInt();



}