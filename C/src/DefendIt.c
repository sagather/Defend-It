//#include <termios.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
//#include <regex.h>
//#include <crypt.h>

char name[50];
char pass[10];

void readName(char * name)
{
   printf("Please enter your name: \n");
   fgets(name, 50, stdin);
   //strip(name);
}

void readpass(char * pass)
{
   printf("Please enter a password of length 10 and includes at least one upper case character, one lower case character, one digit, one punctuation mark: \n");
   fgets(pass, 10, stdin);
   //strip(pass);
}

int testPass(char * pass)
{
   //re_t pattern = re_compile("^((?=.\d)(?=.[a-z])(?=.[A-Z])(?=.[!@#$%]).{10})$");
   //return re_matchp(pattern, pass);
}

int verify(char * pass)
{
  char *result;
  int ok;

  //result = crypt(pass, encrypt(pass));
  ok = strcmp (result, pass) == 0;

  puts(ok ? "Access granted." : "Access denied.");
  return ok ? 0 : 1;
}

int encrypt(char * pass)
{
  unsigned long seed[2];
  char salt[] = "$1$........";
  const char const alpha =
    "./0123456789ABCDEFGHIJKLMNOPQRST"
    "UVWXYZabcdefghijklmnopqrstuvwxyz";
  int i;

  // Generate a (not very) random seed? Rand() was not suggested?
  //Tom has CryptGenRandom on Penguin?
  seed[0] = time(NULL);
  seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

  for (i = 0; i < 10; i++)
//    salt[3+i] = alpha[(seed[i/5] >> (i%5)6) & 0x3f];


  //pass = crypt(pass, salt);
  puts(pass);
  return 0;
}

int getInt(){

    printf("Please enter an integer between ");

}

int main(int argc, const char* argv[]){






}