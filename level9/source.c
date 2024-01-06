void N::setAnnotation(N *this,char *argv)
{
  size_t len;
  int i = 0x18;
  len = strlen(argv);
  memcpy(this + 4,argv,len);
  return;
}

int main(int argc, char** argv, char** envp)
{
    if (argc > 1)
    {
        int32_t* eax = operator new(0x6c); // alloc 108
        N::N(eax, 5); // constructor
        int32_t* eax_1 = operator new(0x6c);
        N::N(eax_1, 6);
        N::setAnnotation(eax, argv[1]);
        return **eax_1(eax_1, eax);
    }
    exit(1);
}


