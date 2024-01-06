char *p(char *dest, char *s)
{
  char buf[4104];

  puts(s);
  read(0, buf, 4096);
  *(strchr(buf, '\n')) = 0;
  return strncpy(dest, buf, 20);
}

char *pp(char *dest)
{
  char line1[20];
  char line2[20];

  p(line1, " - ");
  p(line2, " - ");
  strcpy(dest, line1);
  dest[strlen(dest)] = " ";
  return strcat(dest, line2);
}

int main(void)
{
  char str[54];
  
  pp(str);
  puts(str);
  return 0;
}
