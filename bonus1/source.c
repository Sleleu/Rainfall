int main(int argc, char **argv)

{
  int ret_value;
  char buff [40];
  int x;
  
  x = atoi(*(char **)(argv[1]));
  if (x < 10) {
    memcpy(buff,*(void **)(argv[2]), x * 4);
    if (x == 0x574f4c46) {
      execl("/bin/sh","sh",0);
    }
    ret_value = 0;
  }
  else {
    ret_value = 1;
  }
  return ret_value;
}