void p(void)
{
	unsigned int retaddr;
	char buffer[76];

	fflush(stdout);
	gets(buffer);
	if ((retaddr & 0xb0000000) == 0xb0000000)
	{
		printf("(%p)\n",retaddr);
		_exit(1);
  	}
	puts(buffer);
	strdup(buffer);
	return;
}

int main(void)
{
	p();
	return;
}