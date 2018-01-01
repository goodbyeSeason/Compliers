#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isAlpha(char ch);//�ж��Ƿ�Ϊ��ĸ
bool isDigital(char ch);//�ж��Ƿ�Ϊ����
int Scanner();

int syn;//��ŵ����ӱ���
int p;
char ch;
int lineNum;
char sum[10];//������ֵ��ַ�������
int m;
char program[200], token[10];//token��ŵ��ʵ��ַ�������
char *rwtab[6] = {"begin", "if", "then", "while", "do", "end"};//�ؼ���

int main()
{
    printf("������(�� '#'����)\n");
    do
    {
        ch = getchar();
        program[p++] = ch;
    }
    while (ch != '#');
    p = 0;
    lineNum = 0;
    do
    {
        Scanner();
        switch (syn)
        {
        case -1:
            printf("��%d�зǷ�����: %s\n", lineNum + 1, sum);
            break;
        default:
            if (syn == 11)
                printf("(%d,%s)\n", syn, sum);
            else
                printf("(%d,%s)\n", syn, token);
            break;
        }
    }
    while (syn != 0);
    return 0;
}

bool isDigital(char ch)
{
    if (ch <= '9' && ch >= '0')
        return true;
    else
        return false;
}

bool isAlpha(char ch)
{
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
        return true;
    else
        return false;
}

int Scanner()
{
    int num = 0;
    for (m = 0; m < 10; m++)//��ʼ������������
    {
        sum[m] = NULL;
        token[m] = NULL;
    }
    m = 0;
    ch = program[p++];//�ȶ����һ���ַ�

    while (ch == ' ' || ch == '\n' || ch == '\t')//���Կո񡢻��з����Ʊ��
    {
        if (ch == '\n' || ch == 13)  //ASC����13Ϊ�ո�
            lineNum++;
        ch = program[p++];//���Ŷ����ַ�
    }
    if (isAlpha(ch))//�ж��Ƿ�Ϊ��ĸ
    {
        do
        {
            token[m++] = ch;
            ch = program[p++];
        }
        while (isAlpha(ch) || isDigital(ch));
        p--;
        syn = 10;
        token[m++] = '\0';
        for (int n = 0; n <= 5; n++)
        {
            if (strcmp(token, rwtab[n]) == 0)
            {
                syn = n + 1;
                break;
            }
        }
        return 0;
    }
    else if (isDigital(ch))
    {
        int m = 0;
        while (isDigital(ch))
        {
            sum[m++] = ch;
            ch = program[p++];
        }
        p--;
        syn = 11;
        if (isAlpha(ch))
        {
            p++;
            while(isAlpha(ch) || isDigital(ch))
            {
                sum[m++] = ch;
                ch = program[p++];
            }
            syn = -1;
            p--;
        }
        return 0;
    }
    else
    {
        token[0] = ch;
        switch (ch)
        {
        case '<':
            ch = program[p++];
            if (ch == '>')
            {
                syn = 21;
                token[1] = ch;
            }
            else if (ch == '=')
            {
                syn = 22;
                token[1] = ch;
            }
            else
            {
                syn = 20;
                p--;
            }
            break;
        case '>':
            ch = program[p++];
            if (ch == '=')
            {
                syn = 24;
                token[1] = ch;
            }
            else
            {
                syn = 23;
                p--;
            }
            break;
        case ':':
            ch = program[p++];
            if (ch == '=')
            {
                syn = 18;
                token[1] = ch;
            }
            else
            {
                syn = 17;
                p--;
            }
            break;
        case '+':
            syn = 13;
            break;
        case '-':
            syn = 14;
            break;
        case '*':
            syn = 15;
            break;
        case '/':
            syn = 16;
            break;
        case ';':
            syn = 26;
            break;
        case '(':
            syn = 27;
            break;
        case ')':
            syn = 28;
            break;
        case ',':
            syn = 12;
            break;
        case '#':
            syn = 0;
            break;
        default:
            syn = -1;
            break;
        }
        return 0;
    }
    return 0;
}
