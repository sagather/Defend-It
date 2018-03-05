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
#define BASE (256)
#include <fcntl.h>


//Command for compile in terminal
//gcc -pedantic -Wall -Wextra -Werror DefendIt.c

//TODO:  Make sure we flush the buffer just in case we have inputs longer than our char arrays
//TODO:  Make sure output file type is restricted.  ex: output_txt does not work

char input[51];
char output[51];

char fname[51];
char lname[51];

char pass[11];

static long long passedInt1, passedInt2;

void clearBuf()
{
        char c;
        while((c = (char) getchar()) != '\n' && c != EOF);
}

//File verification code modified from https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
static FILE* openFileRead(char* fileName)
{

    return fopen(fileName, "r");
}

FILE * passwordFile(char * name)
{

    FILE* file = openFileRead(name);

    return file;

}

char * getPass(FILE* file)
{

    fgets(pass, 11, file);
    return pass;
}

bool checkName(char * name)
{
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^([a-zA-Z]+){50}$", REG_EXTENDED|REG_NOSUB) != 0)
    {
        printf("\nregcomp() failed, returning nonzero\n");
        return false;
    }
    ret = regexec(&regex, name, 1, pmatch, 0);
    if(ret == 0) //match found
    {
        return true;
    }
    else
    {

        return false;
    }

}

char * readName(char * n)
{
    int len = 0;
    fgets(n, 51, stdin);
    clearBuf();
    len = (int) strlen(n);
    if(n[len-1] == '\n' )
        n[len-1] = 0;
    return n;

}



bool checkPass()
{
    //passwordFile();
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^([a-zA-Z0-9]+){10}$", REG_EXTENDED|REG_NOSUB) != 0)
    {
        printf("\nregcomp() failed, returning nonzero\n");
        return false;
    }
    ret = regexec(&regex, input, 1, pmatch, 0);
    if(ret == 0) //match found
    {
        return true;
    }
    else
    {
        printf("\nPassword does not meet requirements\n");
        return false;
    }

}

static FILE* openFileWrite(char* fileName)
{

    return fopen(fileName, "w");
}

void readpass(char* fileName)
{
    char pass[11];
    int len = 0;

    FILE *file = passwordFile(fileName);

    printf("Please enter a password of only length 10 that includes only upper case and lower case characters, and digits: \n");
    fgets((char *) pass, (int) "%s", stdin);

    len = (int) strlen((const char *) pass);
    if (pass[len - 1] == '\n')
    {
        pass[len - 1] = 0;
        fprintf(file, "%s", pass);
        fclose(file);
    }
    while (!checkPass())
    {
        readpass(fileName);
    }

}

unsigned long Hasher(const char *s )
{
    unsigned long hash = 5381;
    int c = 0;

    while (c <= *s++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

char * conLong(char * temp, long s)
{
    sprintf(temp, "%ld", s);
    return temp;
}

unsigned long generatePass(char * pass, long s)
{
    //FILE* file = passwordFile(fileName);

    //fgets(pass, "%s", file);

    char temp[30];
    char * you = conLong(temp, s);
    char * newPass = {crypt(pass, you)};
    unsigned long newP = Hasher(newPass);
    return newP;
}


long salt(char * pass)
{
    ssize_t result = 0;
    char temp[10];
    strncpy(temp, pass, 10);
    int randomData = open("/dev/random", O_RDONLY);
    if (randomData < 0)
    {
        printf("something went wrong");
    }
    else
    {
        result = read(randomData, temp, 10) ;
        if (result < 0)
        {
            printf("something went wrong");
        }
    }

    return (long) result ;
}

void verifyPass(unsigned long newPass, unsigned long securePass)
{

    if(newPass == securePass)
    {
        printf("Password has been authenticated: ");
        printf("true\n");
    }
    else
    {
        printf("Password has been authenticated: ");
        printf("false\n");
    }
}

//Source code modified from https://wiki.sei.cmu.edu/confluence/display/c/ERR34-C.+Detect+errors+when+converting+a+string+to+a+number
//and from https://www.techonthenet.com/c_language/standard_library_functions/stdlib_h/strtoll.php
static long long verifyIntType(const char *buff)
{
    long long pInt = (long long) 2000000000000;
    char *ptr = NULL;

    if (buff) {
        pInt = strtoll(buff, &ptr, 10);
        if (pInt == 0)
        {
            /* If a conversion error occurred, display a message and exit */
            if (errno == EINVAL)
            {
                printf("Invalid integer type, were there some weird characters or something?\n");
            }
        }
    }
    else
    {
        printf("Not a valid integer type... did you have any letters or special symbols in there?");
    }

    return pInt;
}

static bool intMatch(char* toMatch)
{

    int len;
    len = (int) strlen(toMatch);
    if(toMatch[len-1] == '\n' )
        toMatch[len-1] = 0;
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^-?([0-9]+)", REG_EXTENDED|REG_NOSUB) != 0)
    {
        printf("\nregcomp() failed, returning nonzero\n");
        return false;
    }
    ret = regexec(&regex, toMatch, 1, pmatch, 0);
    if(ret == 0) //match found
    {
        return true;
    }
    else
    {
        printf("Please enter valid data\n");
        return false;
    }

}

static long long getInt()
{

    char* input[100];
    printf("Enter an integer between -2147483648 and 2147483647\n");
    fgets((char *) input, 100, stdin);

    while(intMatch((char *) input) == false)
    {
        fgets((char *) input, 100, stdin);
    }

    return verifyIntType((const char *) input);
//
}

static bool checkInt(long long input)
{

    if(input >= -2147483648 && input <= 2147483647)
    {
        return true;
    }
    return false;
}

static void readInput(char * input)
{
    int len;
    printf("\nPlease enter the name of an Input File (Must be .txt, must already exist, no special characters or spaces allowed, no more than 50 chars long, and must be in the current directory):\n");
    fgets(input, 51, stdin);
    len = (int) strlen(input);
    if(input[len-1] == '\n' )
        input[len-1] = 0;
}

static bool checkInput(char * input)
{
    struct stat buffer;
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^([a-zA-Z]+)(.txt)$", REG_EXTENDED|REG_NOSUB) != 0)
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
    printf("Please enter the name of an Output File (Must be .txt, must not already exist, no special characters or spaces allowed, no more than 50 chars long, and must be directed to the current directory): \n");
    fgets(output, 51, stdin);
    len = (int) strlen(output);
    if(output[len-1] == '\n' )
        output[len-1] = 0;
}

static int checkOutput(char * output)
{
    struct stat buffer;
    regex_t regex;
    regmatch_t pmatch[2];
    int ret;
    if(regcomp(&regex, "^([a-zA-Z]+)(.txt)$", REG_EXTENDED|REG_NOSUB) != 0)
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

int main()
{
    printf("Please enter your first name: \n");

    readName(fname);
    bool fnamecheck = checkName(fname);
    while(fnamecheck == false)
    {
        readName(fname);
        fnamecheck = checkName(fname);
    }

    printf("Please enter your last name: \n");
    readName(lname);
    bool lnamecheck = checkName(lname);
    while(lnamecheck == false)
    {
        readName(lname);
        lnamecheck = checkName(lname);
    }

    passedInt1 = getInt();
    while(!checkInt(passedInt1))
    {
        passedInt1 = getInt();
    }

    passedInt2 = getInt();
    while(!checkInt(passedInt2))
    {
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
    bool outputcheck = (bool) checkOutput(output);
    while(outputcheck == false)
    {
        printf("\nOutput file name is invalid. Please try again.\n");
        memset(output, 0, sizeof output);
        readOutput(output);
        outputcheck = (bool) checkOutput(output);
    }


    verifyPass((generatePass(getPass(passwordFile("check.pass")), (salt(getPass(passwordFile("check.pass")))))), (generatePass(getPass(passwordFile("check2.pass")), (salt(getPass(passwordFile("check2.pass")))))));


    FILE * inputFile = openFileRead(input);
    FILE* outputFile = openFileWrite(output);

    fprintf(outputFile, "%s %s\n%lld\n%lld\n", fname, lname, (passedInt1 + passedInt2), (passedInt1 * passedInt2));

    int c = 0;
    while((c = getc(inputFile)) != EOF)
    {
        fputc(c, outputFile);
    }
    fclose(outputFile);

    fclose(inputFile);
    fclose(inputFile);
    return 0;
}
