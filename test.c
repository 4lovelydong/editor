#include <stdio.h>
int main(int argc, char *argv[])
{
	fputs("\033[24;75H", stdout);
	fputs("\033[24;75HHello\n",stdout);

	return 0;
}
