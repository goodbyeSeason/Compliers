#include<stdio.h>
#include<string.h>

// use '|' to replace '↑'
// use 'int' to replace 'integer'

int stack[50];
char input[50];
char str[50];
int ip, top, p;              //ip->input     top->stack   p->str
int t;                       //newtable
int fstart, fend;
char tblptr[10][10];
int offset[10];
char stype[10];
int swidth;

struct table1
{
    int state;
    char str[10];
    char output[3];
};      //分析表

struct table1 table[50] =
{
    {0,"id","r2"},
    {0,"proc","r2"},
    {0,"M","1"},
    {0,"P","2"},
    {1,"id","s4"},
    {1,"proc","s5"},
    {1,"D","3"},
    {2,"$","acc"},
    {3,";","s6"},
    {3,"$","r1"},
    {4,":","s7"},
    {5,"id","s8"},
    {6,"id","s4"},
    {6,"proc","s5"},
    {6,"D","9"},
    {7,"int","s11"},
    {7,"real","s12"},
    {7,"|","s13" },
    {7,"T","10"},
    {8,";","s14"},
    {9,";","r3"},
    {9,"$","r3"},
    {10,";","r5"},
    {10,"$","r5"},
    {11,";","r7"},
    {11,"$","r7"},
    {12,";","r8"},
    {12,"$","r8"},
    {13,"int","s11"},
    {13,"real","s12"},
    {13,"|","s13"},
    {13,"T","15"},
    {14,"id","r6"},
    {14,"proc","r6"},
    {14,"N","16"},
    {15,";","r9"},
    {15,"$","r9"},
    {16,"id","s4"},
    {16,"proc","s5"},
    {16,"D","17"},
    {17,";","s18"},
    {18,"id","s4"},
    {18,"proc","s5"},
    {18,"s","s19"},
    {18,"D","9"},
    {19,";","r4"},
    {19,"$","r4"}
};

struct regular1
{
    char start;
    char production[15];
    int len;
};    //文法产生式

struct regular1 regular[10] =
{
    {'S',"P",1},
    {'P',"MD",2},
    {'M',"e",0},
    {'D',"D:D",3},
    {'D',"procid;ND;s",7},
    {'D',"id:T",3},
    {'N',"e",0},
    {'T',"int",1},
    {'T',"real",1},
    {'T',"|T",2}
};

struct newtable
{
    char name[10];
    char type[5];
    int width;
};    //翻译出来的表项
struct newtable newtb[100];

void Init_stack();       //初始化栈
void Init_queue();       //初始化队列
void compare();
int find(int num, int sstart,int send);
int gotofind(int num, char s);
void choose();
void translate(int num);

int main()
{
    printf("input: ");
    scanf("%s", input);
    p = 0;
    Init_queue();
    Init_stack();
    compare();
    int i;
    for (i = 0; i < t; i++)
    {
        printf("%s\t", newtb[i].name);
        printf("%s\t", newtb[i].type);
        printf("%d\n", newtb[i].width);
    }
}

void Init_queue()
{
    int len = strlen(input);
    input[len] = '$';
    input[len + 1] = '\0';
    ip = 0;
}

void Init_stack()
{
    top = 0;
    stack[top] = 0;
}

void compare()
{
    int i;
    while (1)
    {
        choose();                            //分词
        i = find(stack[top], fstart,fend);
        if (table[i].output[0] == 's')
        {
            if (table[i].output[2] == '\0')
            {
                top++;
                stack[top] = table[i].output[1] - '0';
            }
            else
            {
                top++;
                stack[top] = (table[i].output[1] - '0') * 10 + (table[i].output[2] - '0');
            }
            for(i = fstart; i<fend; i++)
                str[p++] = input[i];
            if (input[fend] >= '0' &&input[fend] <= '9')
                ip = fend + 1;
            else
                ip = fend;
            str[p] = '\0';
       //     printf("移入:");
        //    puts(str);
        }
        else if (table[i].output[0] == 'r')
        {
            int num = 0;
            if (table[i].output[2] == '\0')
            {
                num += table[i].output[1] - '0';
            }
            else
            {
                num += (table[i].output[2] - '0') * 10 + table[i].output[1] - '0';
            }
            translate(num);
      //      printf("根据%c->%s归约  ", regular[num].start, regular[num].production);
            if (strcmp(regular[num].production, "e") == 0)
                p -= 0;
            else
                p -= strlen(regular[num].production);
            str[p++] = regular[num].start;
            str[p] = '\0';
            //puts(str);
            top -= regular[num].len;
            int temp = gotofind(stack[top], str[p - 1]);
            if (table[temp].output[1] == '\0')
                stack[top + 1] = table[temp].output[0] - '0';
            else
                stack[top + 1] = (table[temp].output[0] - '0') * 10 + (table[temp].output[1] - '0');
            top++;
        }
        else if(strcmp(table[i].output,"acc") == 0)
        {
          //  printf("接受\n");
            break;
        }
    }
}

int find(int num, int sstart,int send)
{
    char s[100];
    int i, j;
    for (i = sstart,j = 0; i < send; i++,j++)
    {
        s[j] = input[i];
    }
    s[j] = '\0';
    for (i = 0; i < 48; i++)
    {
        if (table[i].state == num&&strcmp(table[i].str,s) == 0)
            break;
    }
    return i;
}

int gotofind(int num, char s)
{
    int i;
    for (i = 0; i < 48; i++)
    {
        if (table[i].state == num&&table[i].str[0] == s)
            break;
    }
    return i;
}

void choose()
{
    if (input[ip] == 'i'&&input[ip + 1] == 'd')
    {
        int i, j, k;
        for (i = ip + 2;; i++)
        {
            if (input[i] < '0'||input[i] > '9')
            {
                for (j = 0, k = ip; k < i; j++, k++)
                {
                    tblptr[t][j] = input[k];
                }
                tblptr[t][j] = '\0';
                break;
            }
        }
    }
    if (input[ip] >= 'a'&&input[ip] <= 'z')
    {
        int i;
        for (i = ip; i < strlen(input); i++)
        {
            if (input[i] == ';' || input[i] == '|' || input[i] == ':' || input[i] == '$' || (input[i]>='0'&&input[i]<='9'))
            {
                fstart = ip;
                fend = i;
                break;
            }
        }
    }
    else
    {
        fstart = ip;
        fend = ip + 1;
    }
}

void translate(int num)
{
    switch (num)
    {
    case 1:
    {
        break;
    }
    case 2:
    {
        offset[t] = 0;
        break;
    }
    case 4:
    {
        break;
    }
    case 5:
    {
        newtb[t].width = offset[t];
        strcpy(newtb[t].name, tblptr[t]);
        strcpy(newtb[t++].type, stype);
        offset[t] += offset[t - 1] + swidth;
        break;
    }
    case 6:
    {
        break;
    }
    case 7:
    {
        strcpy(stype, "int");
        swidth = 4;
        break;
    }
    case 8:
    {
        strcpy(stype, "real");
        swidth = 8;
        break;
    }
    case 9:
    {
        swidth = 4;
        break;
    }
    }
}
