#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
  char s[5];
  char v5[2];
  char v6[129];

  void *auth;
  void *service;

  while (1)
  {
    printf("%p, %p \n", auth, (const void *)service);
    if ( !fgets(s, 128, STDIN_FILENO) )
      break;
    if ( !memcmp(s, "auth ", 5u) )
    {
      auth = (char *)malloc(4u);
      *(__uint32_t *)auth = 0;
      if ( strlen(v5) <= 30 )
        strcpy(auth, v5);
    }
    if ( !memcmp(s, "reset", 5u) )
      free(auth);
    if ( !memcmp(s, "service", 6u) )
      service = (int)strdup(v6);
    if ( !memcmp(s, "login", 5u) )
    {
      if ((auth + 32) != 0)
        system("/bin/sh");
      else
        fwrite("Password:\n", 1, 10, STDOUT_FILENO);
    }
  }
  return 0;
}
