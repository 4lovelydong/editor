#include <stdio.h>
int main(int argc, char *argv[])
{
	fputs("\033[23;75H", stdout);
	fputs("\033[23;75HHello\n",stdout);

	return 0;
}
