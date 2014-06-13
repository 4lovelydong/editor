
 /*not modify this comment. The source code comes with AS-IS.
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

typedef struct line_node
{
	char text;
	int key;
	struct line_node *prev;
	struct line_node *next;
	struct line_node *head;
	struct line_node *tail;
};

typedef struct line_node node;


struct position
{
	int row;
	int col;
};

node *start_display_node;
node *current_node;
node *head=NULL;
node *tail=NULL;

node *create_node(char);
void insert_node(char,int);
void file_open();
int getch(void);
void file_save();
FILE *fp1;
FILE *fp2;

int key;
char data[25][80];
int i,j;

void file_open()
{
	char fn[20],c;
	printf("\n\tEnter the file name:\n ");
	gets(fn);
	fp1=fopen(fn,"rt");
	if(fp1==NULL)
		printf("\n\tFile not found!\n");


	else
	{
		while(!feof(fp1))
		{
			c=getc(fp1);
			printf("%c",c);
		}

		fclose(fp1);
	}

}
/*
void file_save(char data[],FILE* fp2)
{
	char file_name[20];
	int n;
	int i, j;
	puts("Insert:Filename\t");
	gets(file_name);
	fp2=fopen(file_name,"wt");
		for(j=0;j<80;j++)
		{
				if(data[j] != NULL)
					fprintf(fp2, "%c", data[j]);
				else
					fprintf(fp2, "%c", ' ');
		}
	
}
*/
void file_save(node* current_node,FILE* fp2)
{
	char file_name[20];
	int n;
	puts("Insert:File name\t");
	gets(file_name);
	fp2=fopen(file_name,"wt");
	while(current_node!= NULL)
	{	
			if(current_node->text!= '\0')
				{
				fprintf(fp2,"%c",current_node->text);
			//	printf("null no ok");
				}
			else
				{
				fprintf(fp2,"%c",' ');
			//	printf("else ok");
				}
		current_node=current_node->next;
		}	
		fclose(fp2);
}


	
void insert_node(char text,int line_num)
{
	node *new_node, *ptr;
	char data;
	new_node=create_node(text);
	if(head==NULL)
	{
		head=new_node;
		tail=new_node;
		current_node=tail;
	}
	else
	{
			if(current_node==tail)
			{
				new_node->prev=tail;
				tail->next=new_node;
				tail=new_node;
				current_node=new_node;
			}
			else
			{
				new_node->next=current_node->next;
				(current_node->next) -> prev = new_node;
				new_node->prev = current_node;
				current_node->next=new_node;
				current_node=new_node;
			}
	}
		data=current_node->text;
		printf("%c",data);
}

node *create_node(char text)
{
	node *new_node = (node *)malloc(sizeof(node));
	if(new_node != NULL)
	{
		new_node->text=text;
		new_node->next=NULL;
		new_node->prev=NULL;
	}
	return new_node;
}

//bool delete_node(node *target

/*
int getch(void)
{
	struct termios oldt,newt;
	int ch;
	tcgetattr( STDIN_FILENO , &oldt);
	newt=oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch=getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
}*/

int getch(void)
{
	int ch;
	struct termios buf;
	struct termios save;
	
	tcgetattr(0,&save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN]=1;
	buf.c_cc[VTIME]=0;
	tcsetattr(0,TCSAFLUSH, &buf);
	ch=getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

int main(int argc, char * argv[])
{
	char buff[255];	
	enum MODES mode = COMMAND_MODE;
	
	node* temp=create_node(key);

	int cur_line = 1;	
	int cur_col = 1;	
	const int min_line = 1;	
	const int min_col = 1;	
	const int max_line = 25;	
	const int max_col = 80;	
	int a;
	int b=0;
	int key;
	int ret;
//	char data[25][80];	/* some initialization */
	
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

	key = getch();
//	node* temp=create_node(key);
//	node* temp2;
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
	case 'd':
		{
			cur_col = 1;

			for(a=1;a<=80;a++)
			{
			data[cur_line][a]=' ';
			printf("%c",data[cur_line][a]);
			}
		}
		//	delete_current_line();
		break;
	case 'x':
		data[cur_line][cur_col]=' ';
		printf("%c",data[cur_line][cur_col]);
		break;
	case 'o':
		file_open();
		break;
	case 10:
		cur_col=1;
		cur_line++;
		break;
	case 32:
		cur_col++;
		break;
	case 's':
	//	node* temp=create_node(key);
		file_save(temp,fp2);
		break;
	}	
		// end switch	
	sprintf(buff, "\033[%d;%dH%2d:%2d\t %s:%s", 27, 60, cur_line, cur_col,"command","mode");	
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
		case 10:
			cur_col=1;
			cur_line++;
			break;
		case 9:
			printf(" ");
			cur_col--;
			break;
		case 32:
			cur_col++;
			break;	
		default:
			mode = INSERT_MODE;
			if(cur_col<=max_col)
			{
	    		insert_node(key,cur_line);
				data[cur_line][cur_col]=key;
				cur_col++;
			}	
			else
			{
				cur_col=1;
				cur_line++;
				insert_node(key,cur_line);
				data[cur_line][cur_col]=key;
				cur_col++;
				if(cur_line > max_line)
					cur_line=max_line;
			}
			//printf("\n%c",data[cur_line][cur_col]);
			break;
		//	data[25][80]=key;
			//cur_col++;
			/*node *temp = head;
			while(temp != NULL)
				temp=temp->next;*/
		}//Switch end
	  sprintf(buff, "\033[%d;%dH%2d:%2d\t %s:%s", 27, 60, cur_line, cur_col,"insert","mode");
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



