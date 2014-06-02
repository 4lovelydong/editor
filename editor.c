
/** Dong Hee Kim
	Copy right is with 4lovelydong@naver.com

	Do not modify this comment. The source code comes with AS-IS.
	I allow re-distribution of this source code only under the education pur	pose.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/* simple line editor */
/* when it begins, starts with command mode	At command mode, we can move cursor.
	initial cursor position is 1,0	cursor moves left, up, down, right by 'h','j','k','l' key input*/

enum MODES 
{	
COMMAND_MODE,INSERT_MODE
};


int main(int argc, char * argv[])
{
	char buff[255];	
	enum MODES mode = COMMAND_MODE;
	
	int cur_line = 1;	
	int cur_col = 1;	
	const int min_line = 1;	
	const int min_col = 1;	
	const int max_line = 25;	
	const int max_col = 80;	

	int key;
	int ret;
	char data[25][80];	/* some initialization */
	
	fputs("\033[2J", stdout);	
	fputs("\033[1;1H", stdout);	

	struct termios oldt,curt, newt;	

	tcgetattr ( STDIN_FILENO, &oldt );	
	newt = oldt;	
	newt.c_lflag &= ~( ICANON | ECHO ); 	
	tcsetattr ( STDIN_FILENO, TCSANOW, &newt );  	

	/* now, we get input */	
	while (1) 
	{

	key = getchar();

	ret = feof(stdin);
	if(ret != 0)
	{
		printf("EOF\n");
		return 0;
	}
	ret = ferror(stdin);
	if(ret != 0)
	{
		printf("error getchar() by %d\n", ret);
		return 0;
	}

	
	if (mode == COMMAND_MODE) 
	{	
	switch (key) 
	{	/* make movement with the following keys */	
	case 'i':
		mode=INSERT_MODE;
		tcgetattr(STDIN_FILENO, &curt);
		newt=curt;
		newt.c_lflag |= (ECHO);
		tcsetattr(STDIN_FILENO,TCSANOW,&newt);
		break;
	case 'h':		
		cur_col--;		
		if (cur_col < min_col) 
			cur_col = min_col;		
			break;	
	case 'j':		
		cur_line++;		
		if (cur_line > max_line) 
			cur_line = max_line;		
			break;	
	case 'k':		
		cur_line--;		
		if (cur_line < min_line) 
		cur_line = min_line;		
		break;	
	case 'l':		
		cur_col++;		
		if (cur_col > max_col) 
		cur_col = max_col;		
		break;
	}	
		// end switch	
	sprintf(buff, "\033[%d;%dH%2d:%2d\t %s:%s", 0, 60, cur_line, cur_col,"command","mode");	
	fputs(buff, stdout);	
	sprintf(buff, "\033[%d;%dH", cur_line, cur_col);	
	fputs(buff, stdout);	
	} 

	else if(mode == INSERT_MODE) //insert mode
	{
		switch(key)
		{
		case 27:
			mode=COMMAND_MODE;
			tcgetattr(STDIN_FILENO, &curt);
			newt=curt;
			newt.c_lflag &= ~(ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &newt);
			break;
	/*	case 'h':
			cur_col--;
			if(cur_col < min_col)
				cur_col = min_col;
				break;
		case 'j':
			cur_line++;
			if(cur_line > max_line)
				cur_line = max_line;
				break;
		case 'k':
			cur_line--;
			if(cur_line < min_line)
			cur_line=min_line;
			break;
		case 'l':
			cur_col++;
			if(cur_col > max_col)
			cur_col=max_col;
			break;*/
	default:
			mode = INSERT_MODE;
			cur_col++;
			data[cur_col-1][cur_line-1]=key;
			if(cur_col>max_col)
			{
			cur_col=1;
			cur_line++;
			if(cur_line > max_line)
				cur_line=max_line;
			}
			break;
		}//Switch end
	  sprintf(buff, "\033[%d;%dH%2d:%2d\t %s:%s", 0, 60, cur_line, cur_col,"insert","mode");
      fputs(buff, stdout);
      sprintf(buff, "\033[%d;%dH", cur_line, cur_col);
      fputs(buff, stdout);
	}//else end
	}	
	// end while 	
	/* return termio back */ 	
	tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );	
	return 0;
	}
