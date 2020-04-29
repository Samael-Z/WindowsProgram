#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#pragma warning(disable:4996)//4996不报错
void show_1();//判断跳转到show函数
void show();//展示程序功能主界面
void addchar();//添加字符串
void del();//删除字符串
void prin();//打印字符串
void alter();//修改字符串
void seach();//查找字符串
void census();//统计字母出现比例
void infor();//内存资源分布
void sort();//碎片整理
char g_szBuf[10000] = { 0 };//字符串所存的数组
char g_szBuffer[10000] = { 0 };//用来整理有效数据的字符串
int main()//主函数
{
    show();//展示功能选择
    return 0;//退出程序
}
/*
约定
1. 给数据一个标头，-1代表有效数据
2. 给数据一个表头，-2代表无效数据

碎片整理： int a = 5；
            定义一个新数组
            遍历所有-1吧-1装进新数组
            清空旧数组，拷贝返回

*/

//添加字符串
void addchar()
{
    int i = 0;
    char add[5000] = { 0 };
    //保存增加字符串的数组
    printf("请输入你想要添加的字符串：");
    scanf_s("%s", add,sizeof(add));
    //接收增加的字符串
    for (i = 0; i < 10000; i++)
    {
        //在数组的第一个位置插入字符串的判断
        if (g_szBuf[i] == '\0' && g_szBuf[i + 1] == '\0' && i == 0)
        {
            g_szBuf[i] = -1;//有效数据标头-1
            strcpy(&g_szBuf[i + 1], add);//把有效数据插入数组
            break;
        }
        //在数组的第二次插入字符串的判断
        else if (g_szBuf[i] == '\0' && g_szBuf[i + 1] == '\0')
        {
            g_szBuf[i + 1] = -1;//有效数据标头-1
            strcpy(&g_szBuf[i + 2], add); break;//增加字符串
        }
        //重复利用碎片
        else if (g_szBuf[i] == -2 && strlen(&g_szBuf[i + 1]) == strlen(add))
        {
            g_szBuf[i] = -1;//如果匹配到与增加字符串同大小的字符串把原标头
            //置为-1变成有效数据
            strcpy(&g_szBuf[i + 1], add); break;//增加字符串到同等大小碎片

        }
    }
}
//判断是否返回窗口
void show_1()
{

    //接收用户输入的数字，如果匹配调用show函数
    printf("回车键进入主界面：");

         getchar();//接收用户字符
         getchar();
        show();
}
//删除字符串
void del()
{

    int i = 0;
    int n = 0;
    int x = 0;
    //打印数组，供用户选择删除的字符串标号
    for (i = 0; i < 10000; i++)
        if (g_szBuf[i] == -1)
        {
            x++;
            printf("%d:%s\r\n", x, &g_szBuf[i + 1]);//打印有效数据
        }

    scanf("%d", &n);//接收用户删除的字段
    x = 0;//原x值清0
    //遍历数组，如果用户删除的字符串标号与存在的标号一致，则算作删除
    for (i = 0; i < 10000; i++)
    {
        if (g_szBuf[i] == -1)
        {
            x++;
            if (n == x)
            {
                g_szBuf[i] = -2;//把有效数据的标头置为-2
                break;
            }
        }
    }
}
//修改字符串
void alter()
{

    int i = 0;
    int n = 0;
    int x = 0;
    int u = 0;
    int j = 0;
    //打印数组，供用户选择删除的字符串标号
    for (i = 0; i < 10000; i++)
        if (g_szBuf[i] == -1)
        {
            u++;
            printf("%d:%s\r\n", u, &g_szBuf[i + 1]);
            //打印有效数据供用户选择，并记录标头位置
        }

    printf("###########修改字符串#############\r\n");
    printf("请输入你想要添加的字符串：");
    i = 0;
    u = 0;
    n = 0;
    char add[5000] = { 0 };
    scanf_s("%s", add,sizeof(add));
    printf("请输入你想要添加到的编号：");
    scanf("%d", &n);
    //用户输入添加的字符串内容和编号时候，遍历数组给出合适位置
    for (i = 0; i < 10000; i++)
        if (g_szBuf[i] == -1)
        {
            u++;
            //如果源字符串的长度小于增加字符串的长度，将新字符串加到数组最后面
            if ((n == u) && strlen(&g_szBuf[i + 1]) < strlen(add))
            {
                g_szBuf[i] = -2;
                //如果匹配到要修改的字符串把原标头置为-2
                for (j = 0; j < 10000; j++)
                {
                    if (g_szBuf[j] == '\0' && g_szBuf[j + 1] == '\0')
                    {
                        g_szBuf[j + 1] = -1;
                        //将修改的字符串放进去
                        strcpy(&g_szBuf[j + 2], add); break;
                    }
                }
            }
            //如果源字符串的长度等于增加字符串的长度，将新字符串换到源字符串的位置
            else if ((n == u) && strlen(&g_szBuf[i + 1]) == strlen(add))
            {
                strcpy(&g_szBuf[i + 1], add); break;
                //把要修改的字符串拷贝进数组
            }
            //如果源字符串的长度大于增加字符串的长度，将新字符串加到数组最后面
            else if ((n == u) && strlen(&g_szBuf[i + 1]) > strlen(add))
            {
                g_szBuf[i] = -2;
                //把有效数据标头改成无效数据
                for (j = 0; j < 10000; j++)
                {//循环遍历找到位置
                    if (g_szBuf[j] == '\0' && g_szBuf[j + 1] == '\0')
                    {
                        g_szBuf[j + 1] = -1;
                        //把原标头置为-1
                        strcpy(&g_szBuf[j + 2], add); break;
                    }
                }
            }

        }
}

//打印字符串
void prin()
{
    int x = 0;
    int i = 0;
    //遍历数组，找到有效数据标头-1，打印&g_szBuf[i + 1]
    for (i = 0; i < 10000; i++)
        if (g_szBuf[i] == -1)
        {//遍历数组，找有效数据标头-1
            x++;
            printf("%d:%s\r\n", x, &g_szBuf[i + 1]);
            //打印所有有效数据，并且给给每个数据一个标号
        }
}
//查询字符串
void  seach()
{
    int i = 0;
    int n = 0;
    int x = 0;//记录数据标号
    int input = 0;//接收用户的功能选择
    char s_char[5000] = { 0 };
    printf("###########查询字符串#############\r\n");
    printf("###########1：按内容#############\r\n");
    printf("###########2：按编号#############\r\n");
    printf("###########3：模糊查找#############\r\n");
    printf("请输入输入查询的的方式:");
    scanf("%d", &input);
    //接收用户的选择
    if (input == 1)
    {
        prin();
        //打印有效数据
        printf("你要查询字符串的内容:");
        scanf("%s", s_char);
        //接收查询内容
        for (i = 0; i < 10000; i++)
        {
            if (g_szBuf[i] == -1)
            {
                //比较字符串是否一样
                if ((strcmp(&g_szBuf[i + 1], s_char)) == 0)
                {
                    //匹配字符串是否一样
                    printf("%s\r\n", &g_szBuf[i + 1]);
                    //打印字符串
                }
            }
        }

    }
    else if (input == 2)
    {
        prin();
        //展示原有字符串，方面用户选择
        printf("你要查询字符串的编号:");
        scanf("%d", &n);
        //接收用户的要查找的标号
        for (i = 0; i < 10000; i++)
        {
            //比较字符串的标号和输入的数字是否一样
            if (g_szBuf[i] == -1)
            {
                x++;
                if (n == x)//如果查找的标号和原有标号一致
                {
                    printf("%s\r\n", &g_szBuf[i + 1]);
                    //打印字符串
                    break;
                }
            }
        }

    }
    else if (input == 3)
    {
        prin();
        //展示原有字符串
        int y = 0;
        printf("你要查询字符串模糊的内容:");
        scanf("%s", s_char);
        //接收用户要查找的模糊字段
        for (i = 0; i < 10000; i++)
        {
            if (g_szBuf[i] == -1)
            {
                y++;
                //比较有效字符串的内容是否有检索的内容
                if (strstr(&g_szBuf[i + 1], s_char) != NULL)
                {
                    //调用strstr函数检索查找字符在每一个有效数据是否出现
                    printf("%d:%s\r\n", y, &g_szBuf[i + 1]);
                    //打印出现过的字符串
                }

            }
        }
    }

}
//统计字符串
void census()
{
    int i = 0;
    int ch[256] = { 0 };
    int k = 0;
    for (i = 0; i < 10000; i++)
    {//遍历整个数组，找到出现的所有字母
        if (('A' <= g_szBuf[i] && g_szBuf[i] <= 'Z') || ('a' <= g_szBuf[i] && g_szBuf[i] <= 'z'))
        {
            //将出现的英文字符强转为整型，存放在整型数组ch里面
            ch[(int)g_szBuf[i]]++;
            k++;
        }
    }
    printf("###########统计字符串#############\r\n");
    for (i = 0; i < 256; i++)
    {//遍历出现过的int型字母
        if (ch[i] != 0)//如果不等于0进入内层
        {
            //在整型数组里面的数字还原成英文字符，并计算比例
            printf("字母%c: 出现了%d 比例:%5.2f%%\r\n", (char)i, ch[i], (float)ch[i] / (float)k * 100);
        }

    }
}
//存储资源分布
void infor()
{
    int i = 0;
    int j = 0;
    printf("存储信息分布图:");
    for (i = 0; i < 10000; i++)
    {//遍历数组
        if (g_szBuf[i] == -1)
        {//找到有效数据
            //有效数据全部替换成U
            for (j = i + 1; j <= (strlen(&g_szBuf[i + 1]) + i); j++)
            {//打印与有效数据等同的字符U
                printf("U");
            }
        }
        if (g_szBuf[i] == -2)
        {//找到所有的无效数据
            //无效数据全部替换成F
            for (j = i + 1; j <= (strlen(&g_szBuf[i + 1]) + i); j++)
            {//打印与所有无效数据等同的字符F
                printf("F");
            }
        }
    }
    printf("\r\n");
}
//碎片整理
void sort()
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 10000; i++)
    {
        //找到有效数据放到另一个数组里面，再把原数组清0，在把有效数组放回去
        if (g_szBuf[i] == -1)
        {//找到有效数据
            for (j = 0; j < 10000; j++)
            {
                if (g_szBuffer[j] == '\0' && g_szBuffer[j + 1] == '\0' && j == 0)
                {
                    //把有效数据放到新数组的第一个位置
                    strcpy(&g_szBuffer[j], &g_szBuf[i]);
                    //拷贝原有效数据
                    break;
                }
                else if (g_szBuffer[j] == '\0' && g_szBuffer[j + 1] == '\0')
                {
                    //把原有效数据在新数组内依次排好
                    strcpy(&g_szBuffer[j+1], &g_szBuf[i]);
                    //赋值有效数据
                    break;
                }
            }
        }
    }
   
    for (i = 0; i < 10000; i++)
    {
        g_szBuf[i] = '\0';
      //清空原有效数据的值
    }
   
    for (i = 0; i < 10000; i++)
    {
        if (g_szBuffer[i] == -1)
        {
            strcpy(&g_szBuf[i], &g_szBuffer[i]);
        }
        //遍历数组，把排列好的有效数据
        //依次放回原数组
    }
}
//展示主界面
void show()
{
    int n = 0;//接收用户的功能选择
    system("cls");///清除上一个界面
    printf("###########字符串增删改查2.0#########\r\n");
    printf("###########1：增加字符串#############\r\n");
    printf("###########2：删除字符串#############\r\n");
    printf("###########3：修改字符串#############\r\n");
    printf("###########4：查询字符串#############\r\n");
    printf("###########5：统计字符串#############\r\n");
    printf("###########6：存储资源分布###########\r\n");
    printf("###########7：碎片整理###############\r\n");
    printf("###########8：打印字符串#############\r\n");
    printf("请选择功能模块：");
    scanf_s("%d", &n);
    //接收用户的功能选择
    switch (n)
        //用switch-case实现用户对功能的选择
    {
    case 1:
        printf("###########增加字符串#############\r\n");
        addchar();//增加字符串
        show_1();//选择是否进入功能 选择
        break;
    case 2:
        printf("###########删除字符串#############\r\n");
        printf("你要删除字符串的编号:");
        del();//删除字符串
        show_1();//选择是否进入功能 选择
        break;
    case 3:

        alter();//修改字符串
        show_1();//选择是否进入功能 选择
        break;
    case 4:
        seach();//查找字符串
        show_1();//选择是否进入功能 选择
        break;
    case 5:
        census();//统计字母
        show_1();//选择是否进入功能 选择
        break;
    case 6:
        infor();//资源分布
        show_1();//选择是否进入功能 选择
        break;
    case 7:
        sort();//碎片整理
        show_1();//选择是否进入功能 选择

        break;
    case 8:
        prin();//打印字符串
        show_1();//选择是否进入功能 选择
        break;
    default:
        printf("输入有误退出程序！\r\n");
        //提示错误信息！
        break;
    }

}