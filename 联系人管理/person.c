#include <stdio.h>
#include<stdlib.h>
#include<windows.h>
#pragma warning(disable:4996)
void Add();//增加联系人
void Del();//删除联系人
void Alter();//修改联系人
void Que();//查找联系人
void Count();//统计联系人
void Resource();//联系人资源分布
void FragMent();//碎片整理
void Output();//输出联系人
void Show();//功能选择主界面
void WriteIn();//读入内存
void Clean();//清理g_szBuf
#define SIZE 1000
char g_szBuf[SIZE] = { 0 };
//定义一个全局数据，接收用户输入的数据，然后写进文件
int main()
{
    Show();
    //调用主界面
    return 0;
}
void Clean()//清空数组。以便于接收新的数据
{
    for (int i = 0; i < SIZE; i++)
        //遍历整个数组
    {
        g_szBuf[i] = '\0';
        //把每个值的数据清空
    }
}
void WriteIn()//把数组里面的数据依次写入文件
{
    FILE* fp = NULL;
    //文件指针
    int nLen = 0;
    //联系人姓名长度
    int nLen2 = 0;
    //联系方式长度
    int nLen3 = 0;
    //总长度
    int i = 0;
    //局部变量i
    nLen = strlen(g_szBuf) + 1;
    //计算联系人姓名长度
    nLen2 = 1 + strlen(&g_szBuf[strlen(g_szBuf) + 1]);
    //计算联系方式长度
    nLen3 = nLen2 + nLen;
    //计算总长度
    for (i = 0; i < nLen3; i++)
    {
        fp = fopen("MyData2.bin", "ab+");
        //以追加的方式打开文件
        if (fp == NULL)
            //文件打开失败判定
        {
            printf("文件打开失败\r\n");
            //文件打开失败提示
            exit(1);
            //退出程序
        }
        fwrite(&g_szBuf[i], sizeof(char), 1, fp);
        //写入数据
        fclose(fp);
        //关闭文件指针
    }
}

void Add()//增加联系人功能
{
    char ChAdd[33] = {0};
    //联系人姓名接收数组
    char nTel[12] = { 0 };
    //手机号接收数组
    char c;
    //缓冲区清空变量
    Clean();
    //清空原数组
    printf("----------------添加联系人-----------------\r\n");
    printf("------姓名最大允许输入16个汉字\r\n");
    printf("------手机号码最长允许11位\r\n");
    printf("$你想要添加的联系人姓名：");
    //UI界面
    scanf_s("%32s", ChAdd,sizeof(ChAdd));
    //接收联系人姓名，并且只接收16位汉字
    while ((c = getchar()) != '\n' && c != EOF);
    //缓冲区清空
    printf("$你想要添加的联系人手机号码：");
    scanf_s("%11s", nTel,sizeof(nTel));
    //接收手机号码，并且限制11位手机号码
    while ((c = getchar()) != '\n' && c != EOF);
    //缓冲区清空
    Clean();
    //清空原数组
    for (int i = 0; i < SIZE; i++)
    {
        //在数组的第一个位置插入字符串的判断
         if (g_szBuf[i] == '\0' && g_szBuf[i + 1] == '\0' && i == 0)
         {
                g_szBuf[i] = -1;
                //联系人标头-1
                g_szBuf[ i+2 + strlen(ChAdd)] = -2;
                //手机号标头-2
                strcpy(&g_szBuf[i+1], ChAdd);
                //把联系人插入数组
                strcpy(&g_szBuf[i+ 3 + strlen(ChAdd)], nTel);
                //联系方式插入数组
                break;
                //跳出循环
         }  
    }

    WriteIn();
    //把整理好的数据写入文件
    printf("写入成功\r\n");
    //提示语句
    Clean();
    //清空原数组
}
void Del()//删除联系人功能
{
    printf("--------------------删除联系人--------------------\r\n");
    Clean();
    Output();
    //打印已有通讯录
    int nStrlen = 0;
    FILE* fp = NULL;
    //文件指针
    int DeleteIn = 0;
    //接收删除编号
    int nFileSize = 0;
    //通讯录长度
    int i = 0;
    int nTally = 1;
    //局部变量
    int nCount = 0;
    printf("$请输入你想要删除的联系人序号：");
    scanf("%d", &DeleteIn);
    //接收被删除的序号
    fp = fopen("MyData2.bin", "rb");
    //读数据
    fseek(fp, 0, SEEK_END);
    //把文件指针移动到文件末尾
    nFileSize = ftell(fp);
    //计算文件数据长度
    if (fp == NULL)
     //文件打开失败判断条件
    {
        printf("文件打开失败\r\n");
        //文件打开失败提示
        exit(1);
        //退出程序
    }

    fseek(fp, 0, SEEK_SET);
    //文件指针移动到当前
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;
            //如果文件读取失败
            //跳转到EXIT_PROC
            //关闭指针
        }
    }
    for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
    {
        if (g_szBuf[i] == -1)
            //联系人判断条件
        {
            nCount++;
            //记录联系人标号
            if (DeleteIn == nCount)
                //联系人标号与被删除标号判断
            {
                g_szBuf[i] = -3;
                //联系人置为删除
                g_szBuf[i + strlen(&g_szBuf[i]) + 1] = -3;
                //联系方式置为删除
            }
           
        }
    }
    fclose(fp);
    //关闭指针
    //写回文件
    for (i = 0; i < SIZE; i++)
    {
        if ((g_szBuf[i] == -1 && i==0) || (g_szBuf[i] == -2 && i == 0) || (g_szBuf[i] == -3 && i == 0))
            //写入数据判断条件
        {
            if (nTally == 1)
            {
                fp = fopen("MyData2.bin", "wb");
                //第一次写入数据用wb形式从头开始写
                nTally++;
            }
            if (fp == NULL)
                //文件打开失败判定条件
            {
                printf("文件打开失败\r\n");
                //文件打开提示
                exit(1);
                //退出程序
            }
            fwrite(&g_szBuf[i], sizeof(char), strlen(&g_szBuf[i]) + 1, fp);
            fclose(fp);
            
        }
        else if ((g_szBuf[i] == -1 && g_szBuf[i- 1]  == '\0') ||
                    (g_szBuf[i] == -2 && g_szBuf[i- 1] == '\0') || 
                    (g_szBuf[i] == -3 && g_szBuf[i -1] == '\0')  )
        {
            if (nTally == 1)
            {
                fp = fopen("MyData2.bin", "wb");
                nTally++;
            }
            else
            {
                fp = fopen("MyData2.bin", "ab+");
            }
            if (fp == NULL)
            {
                printf("文件打开失败\r\n");
                exit(1);
            }
            fwrite(&g_szBuf[i], sizeof(char), strlen(&g_szBuf[i]) + 1, fp);
            fclose(fp);
        }
    }   
   //备份结束位置

EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    Clean();
}
void Alter()//修改联系人功能
{
    Clean();
    int AlterIn = 0;
    char AlterName[32] = { 0 };
    char c;
    int nChange = 0;
    FILE* fp = NULL;
    int nFileSize = 0;
    int i = 0;
    int nCount = 0;
    int nTally = 1;
    fp = fopen("MyData2.bin", "rb");
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    if (fp == NULL)
    {
        printf("文件打开失败\r\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;//如果失败，关闭文件指针
        }
    }
    fclose(fp);//关闭指针
    printf("------------------修改联系人-------------------\r\n");
    printf("------------1：姓名---------\r\n");
    printf("------------2：手机号码-------\r\n");
    printf("请选择：");
    scanf("%d" ,&AlterIn);
    if (AlterIn == 1)//只修改姓名
    {
        Output();
        while ((c = getchar()) != '\n' && c != EOF);
        printf("$请输入新姓名：");
        scanf("%32s", AlterName);
        while ((c = getchar()) != '\n' && c != EOF);
        printf("$请选择替换的位置:");
        scanf("%d", &nChange);


        for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
        {
            if (g_szBuf[i] == -1)//找到有效数据-联系人
            {
                nCount++;//下标+1
                if (nCount== nChange)//找到修改位置
                {
                    if (strlen(AlterName) == strlen(&g_szBuf[i + 1]))
                        //如果修改的名字和原本字符串大小一致，在原地修改
                        //标头和原来一样，可以不用改变
                    {
                            strcpy(&g_szBuf[i + 1], AlterName);
                            break;
                    }

                    else  if (strlen(AlterName) < strlen(&g_szBuf[i + 1]))
                        //如果修改的名字小于原本字符串，在原地修改
                        //标头和原来一样，可以不用改变
                    {
                            memset(&g_szBuf[i + 1], 0, strlen(&g_szBuf[i + 1]));
                            //清空原本字符串
                            strcpy(&g_szBuf[i + 1], AlterName);
                            break;
                    }
                    else  if (strlen(AlterName) > strlen(&g_szBuf[i + 1]))
                            //如果修改的名字大于原本字符串，在原地修改
                            //将原来的联系人标头置为-3
                    {
                             //把现标头置为-1
                               g_szBuf[nFileSize] = -1;
                              //把原标头置为-3
                               g_szBuf[i] = -3;
                               //在字符串末尾添加新的联系人
                               strcpy(&g_szBuf[nFileSize+1], AlterName);
                               //添加原对应的联系人电话号码
                               strcpy(&g_szBuf[nFileSize + 2+strlen(AlterName)], 
                                   &g_szBuf[i + 1 + strlen(&g_szBuf[i])]);
                               //并且原来联系人的手机号码也置为-3
                               g_szBuf[i + 1 + strlen(&g_szBuf[i])] = -3;
                               break;
                    } 
                }
            }
        }
    }
    else if (AlterIn == 2)//只修改手机号
    {
        Output();
        while ((c = getchar()) != '\n' && c != EOF);
        printf("$请输入新的联系方式：");
        scanf("%11s", AlterName);
        while ((c = getchar()) != '\n' && c != EOF);
        printf("$请选择替换的位置");
        scanf("%d", &nChange);

        for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
        {
            if (g_szBuf[i] == -1)//找到有效数据-联系人
            {
                nCount++;//下标+1
                if (nCount == nChange)//找到修改位置
                {
                    if (strlen(AlterName) == strlen(/*原手机号码长度*/
                        &g_szBuf[  /*当前地址*/i+  2+/*偏移地址*/strlen(&g_szBuf[i])]))
                        //如果修改的手机号和原本手机号长度一致，在原地修改
                        //标头和原来一样，可以不用改变
                    {
                        strcpy(&g_szBuf[i + 2 +
                            /*偏移地址*/strlen(&g_szBuf[i])], AlterName);
                        break;
                    }

                    else  if (strlen(AlterName) <  strlen(/*原手机号码长度*/
                        &g_szBuf[  /*当前地址*/i + 2 +/*偏移地址*/strlen(&g_szBuf[i])]))
                        //如果修改的手机号长度小于原本手机号长度，在原地修改
                        //标头和原来一样，可以不用改变
                    {
                        memset(&g_szBuf[i + 2+strlen(&g_szBuf[i])],
                            0, strlen(&g_szBuf[i +2 +strlen(&g_szBuf[i])]));
                        //清空原本字符串
                        strcpy(&g_szBuf[i + 2 + strlen(&g_szBuf[i])], AlterName);
                        break;
                    }
                    else  if (strlen(AlterName) > strlen(/*原手机号码长度*/
                        &g_szBuf[  /*当前地址*/i + 2 +/*偏移地址*/strlen(&g_szBuf[i])]))
                        //如果修改的手机号长度大于原本手机号的长度，
                        //在数组末尾修改
                        //将原来的联系人标头置为-3
                    {
                        //在字符串末尾添加新的联系人
                        strcpy(&g_szBuf[nFileSize], &g_szBuf[i]);
                        //添加原对应的联系人电话号码
                        strcpy(&g_szBuf[nFileSize + 2 + strlen(&g_szBuf[i])],
                            AlterName);
                        g_szBuf[nFileSize + 1 + strlen(&g_szBuf[i])] = -2;
                        //把原标头置为-3
                        g_szBuf[i] = -3;
                        g_szBuf[i + 1 + strlen(&g_szBuf[i])] = -3;
                        break;
                    }
                }
            }
        }
    }
    for (i = 0; i < SIZE; i++)
    {
        if ((g_szBuf[i] == -1&& i== 0) || (g_szBuf[i] == -2 && i == 0) || (g_szBuf[i] == -3 && i == 0))
        {
            if (nTally == 1)
            {
                fp = fopen("MyData2.bin", "wb");
                nTally++;
            }
            if (fp == NULL)
            {
                printf("文件打开失败\r\n");
                exit(1);
            }
            fwrite(&g_szBuf[i], sizeof(char), strlen(&g_szBuf[i]) + 1, fp);
            fclose(fp);
        }

        else if ((g_szBuf[i] == -1 && g_szBuf[i-1] == '\0') ||
            (g_szBuf[i] == -2 && g_szBuf[i - 1] == '\0') || 
            (g_szBuf[i] == -3 && g_szBuf[i - 1] == '\0'))
        {
            if (nTally == 1)
            {
                fp = fopen("MyData2.bin", "wb");
                nTally++;
            }
            else
            {
                fp = fopen("MyData2.bin", "ab+");
            }
            if (fp == NULL)
            {
                printf("文件打开失败\r\n");
                exit(1);
            }
            fwrite(&g_szBuf[i], sizeof(char), strlen(&g_szBuf[i]) + 1, fp);
            fclose(fp);
        }
    }
    
EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    Clean();//清空原数组
}
void Que()//查找联系人功能
{
    Clean();
    Output();
    int nStrlen = 0;
    FILE* fp = NULL;
    char QueIn[32] = { 0 };
    char c;
    int nFileSize = 0;
    int i = 0;
    int nCount = 0;
    int nSele = 0;
    printf("---------------------联系人查找---------------------\r\n");
    printf("-------------1：姓名查找\r\n");
    printf("-------------2：手机号查找\r\n");
    printf("-------------3模糊查找\r\n");
    scanf("%d", &nSele);
    fp = fopen("MyData2.bin", "rb");
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    if (fp == NULL)
    {
        printf("文件打开失败\r\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;
        }
    }
    fclose(fp);//关闭指针
    //指定姓名查找
    if (nSele == 1)
    {
        while ((c = getchar()) != '\n' && c != EOF);
        printf("$输入被查找的姓名：");
        scanf("%32s", QueIn);
        while ((c = getchar()) != '\n' && c != EOF);
        for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
        {
            if (g_szBuf[i] == -1)
            {

                if (strcmp(&g_szBuf[i+1], QueIn) == 0)//算法正确
                {
                    nCount++;
                    //输出查找到的姓名，手机号
                    printf("查找成功！\r\n");
                    printf("%d~联系人：%s--手机号：%s", nCount,
                        &g_szBuf[i + 1], &g_szBuf[i + strlen(&g_szBuf[i]) + 2]);
                }
            }
        }
    }
    //指定姓名查找结束
    else if (nSele == 2)//指定号码查找
    {
        while ((c = getchar()) != '\n' && c != EOF);
        printf("输入被查找的手机号：");
        scanf("%11s", QueIn);
        while ((c = getchar()) != '\n' && c != EOF);
        for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
        {
            if (g_szBuf[i] == -1)
            {

                if (strcmp(&g_szBuf[i + strlen(&g_szBuf[i]) + 2], QueIn) == 0)//算法正确
                {
                    nCount++;
                    //输出查找到的姓名，手机号
                    printf("查找成功！\r\n");
                    printf("%d~联系人：%s---手机号：%s",nCount, 
                    &g_szBuf[i + 1],&g_szBuf[i+strlen(&g_szBuf[i])+2]); 
                }
            }
        }
    }//指定号码结束；
   //模糊查找
    else if (nSele == 3)
    {
        while ((c = getchar()) != '\n' && c != EOF);
        printf("输入被查找的模糊姓名或者手机号：");
        scanf("%32s", QueIn);
        while ((c = getchar()) != '\n' && c != EOF);
        for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
        {
            if (g_szBuf[i] == -1)
            {

                if ((strstr(&g_szBuf[i+1], QueIn) != NULL)||
                    (strstr(&g_szBuf[i + strlen(&g_szBuf[i]) + 2], QueIn) != NULL))//算法正确
                {
                    nCount++;
                    //输出查找到的姓名，手机号
                    printf("查找成功！\r\n");
                    printf("%d~联系人：%s---手机号：%s\r\n", nCount,
                        &g_szBuf[i + 1], &g_szBuf[i + strlen(&g_szBuf[i]) + 2]);
                }
            }
        }
    }
    else
    {
        printf("输入有误！");
    }

EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    Clean();
}
void Count()//统计联系人功能
{
    //5.可统计每个联系人信息的出现次数和比例。

    Clean();
    FILE* fp = NULL;
    int nCount = 0;
    int nFileSize = 0;
    char ChCount[32] = { 0 };
    char c;
    int i = 0;
    float fNumerator = 0;
    float fDenominator = 0;
    float fConsult = 0;
    fp = fopen("MyData2.bin", "rb");
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    if (fp == NULL)
    {
        printf("文件打开失败\r\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;
        }
    }
    fclose(fp);
    printf("--------------------统计联系人--------------------\r\n");
    printf("--------------------请输入被统计联系人姓名:");
    //while ((c = getchar()) != '\n' && c != EOF);
    scanf("%32s", ChCount);
    //while ((c = getchar()) != '\n' && c != EOF);
    for (i = 0; i < nFileSize; i++)
    {
        if (g_szBuf[i] == -1)
        {
            fDenominator++;//分母
            if (strcmp(&g_szBuf[i + 1], ChCount)==0)
            {
                fNumerator++;//分子
            }
        }
    }
    fConsult = (fNumerator / fDenominator)*100;
    printf("----$%s$出现了%.0f次----出现的比例是:%.2f%%", ChCount, fNumerator, fConsult);
EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    Clean();

}
void Resource()//联系人资源分布功能
{
    Clean();
    FILE* fp = NULL;
    int nCount = 0;
    int nFileSize = 0;
    int i = 0;
    fp = fopen("MyData2.bin", "rb");
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    if (fp == NULL)
    {
        printf("文件打开失败\r\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;
        }
    }
    fclose(fp);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        //g_szBuf[i+strlen(&g_szBuf[i])] =='\0'   g_szBuf[i] == -3 && g_szBuf[i-1]=='\0'
        if (g_szBuf[i] == -3 && i ==0 )
        {
            for (int j = 0; j < strlen(&g_szBuf[i]); j++)
            {
                printf("F");
            }
        }
        else if(g_szBuf[i] == -3 && g_szBuf[i - 1] == '\0')
        {
            for (int j = 0; j < strlen(&g_szBuf[i]); j++)
            {
                printf("F");
            }
        }
        else if((g_szBuf[i] == -1 && i == 0)|| (g_szBuf[i] == -2 && i == 0))
        {
            for (int j = 0; j < strlen(&g_szBuf[i]); j++)
            {
                printf("U");
            }
        }
        else if ((g_szBuf[i] == -1 && g_szBuf[i-1] == '\0') || (g_szBuf[i] == -2 && g_szBuf[i - 1] == '\0'))
        {
            for (int j = 0; j < strlen(&g_szBuf[i]); j++)
            {
                printf("U");
            }
        }
    }
EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    Clean();
}
void FragMent()//联系人碎片整理功能
{
    Clean();
    FILE* fp = NULL;
    int nCount = 0;
    int nFileSize = 0;
    int i = 0;
    int nTally = 1;
    fp = fopen("MyData2.bin", "rb");
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    if (fp == NULL)
    {
        printf("文件打开失败\r\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;
        }
    }
    fclose(fp);

    for (i = 0; i < nFileSize; i++)
    {
        if ((i==0 && g_szBuf[i] == -1) || (i == 0 && g_szBuf[i] == -2))
            //第一次写入数据判断条件
        {
             if (nTally == 1)
             {
                fp = fopen("MyData2.bin", "wb");
                //用wb从原文件开始写入可以起到清空源文件的作用
                 nTally++;
             }
            //else
          //  {
            //    fp = fopen("MyData2.bin", "ab+");
            //}
            if (fp == NULL)//文件打开失败判断条件
            {
                printf("文件打开失败\r\n");
                //提示文件打开失败
                exit(1);
                //退出程序
            }
            fwrite(&g_szBuf[i], sizeof(char), strlen(&g_szBuf[i]) + 1, fp);
            //向文件写入第一条数据
            fclose(fp);
            //关闭文件指针
        }
        else if((g_szBuf[i] == -1&&g_szBuf[i-1] == '\0') || ( g_szBuf[i] == -2 && g_szBuf[i - 1] == '\0'))
            //写入剩下有效数据判断条件
        {
            if (nTally == 1)
            {
                fp = fopen("MyData2.bin", "wb");
                //用wb从原文件开始写入可以起到清空源文件的作用
                nTally++;
            }
            else
            {
                 fp = fopen("MyData2.bin", "ab+");
                   //追加写入文件
                  if (fp == NULL)
                     //文件打开失败判断条件
                  {
                      printf("文件打开失败\r\n");
                        //提示文件打开失败
                      exit(1);
                        //退出程序
                  }
                  fwrite(&g_szBuf[i], sizeof(char), strlen(&g_szBuf[i]) + 1, fp);
                   //写入剩下的有效数据
                  fclose(fp);
                   //关闭文件指针
            }
        }
    }


EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    Clean();

}
void Output()//输出联系人功能
{
    printf("--------------------通讯录--------------------\r\n");
    FILE* fp = NULL;
    int nCount = 0;
    int nFileSize = 0;
    int i = 0;
    fp = fopen("MyData2.bin", "rb");
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    if (fp == NULL)
    {
        printf("文件打开失败\r\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < nFileSize; i++)//把信息从二进制文件里读取到数组
    {
        if (fread(&g_szBuf[i], sizeof(char), 1, fp) != 1)
        {
            goto EXIT_PROC;
        }
    }
    fclose(fp);
    for (i = 0; i < nFileSize; i++)//遍历数组找到有效值
    {
        if (g_szBuf[i] == -1)
        {
            nCount++;
            printf("%d:%s:%s\r\n", 
                nCount, &g_szBuf[i + 1], &g_szBuf[i + 2 + strlen(&g_szBuf[i])]);
        }
    }
EXIT_PROC:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
}
void Show()//功能选择主界面
{
    int i = 0;//接收用户的功能选择
    //system("color 4e");//给界面换个颜色
    //int j = 1;
    while (1)
    {
        system("cls");//清除上一个界面
        printf("*****************联系人增删改查*****************\r\n");
        printf("------------------1：增加字符串------------------\r\n");
        printf("------------------2：删除字符串------------------\r\n");
        printf("------------------3：修改字符串------------------\r\n");
        printf("------------------4：查询字符串------------------\r\n");
        printf("------------------5：统计字符串------------------\r\n");
        printf("------------------6：存储资源分布----------------\r\n");
        printf("------------------7：碎片整理--------------------\r\n");
        printf("------------------8：打印字符串------------------\r\n");
        printf("------------------0：退出程序------------------\r\n");
        printf("------------------请选择功能模块：");
        scanf_s("%d", &i);//接收用户的功能选择
        switch (i)//用switch-case实现用户对功能的选择
        {
        case 0:
            exit(1);
        case 1:
            printf("###################################################\r\n");
            Add();
            printf("###################################################\r\n");
            system("pause");
            break;
        case 2:
            printf("###################################################\r\n");
            Del();
            printf("###################################################\r\n");
            system("pause");
            break;
        case 3:
            printf("###################################################\r\n");
            Alter();
            printf("###################################################\r\n");
            system("pause");
            break;
        case 4:
            printf("###################################################\r\n");
            Que();
            printf("###################################################\r\n");
            system("pause");
            break;
        case 5:
            printf("###################################################\r\n");
            Count();
            printf("\r\n");
            printf("###################################################\r\n");
            system("pause");
            break;
        case 6:
            printf("###############资源分布#########################\r\n");
            Resource();
            printf("\r\n");
            //printf("###################################################\r\n");
            system("pause");
            break;
        case 7:
            FragMent();
            printf("碎片整理完毕......");
            system("pause");
            break;
        case 8:
            printf("###################################################\r\n");
            Output();
            printf("###################################################\r\n");
            system("pause");
            break;
        default:
            printf("###################################################\r\n");
            printf("输入有误请再次输入！\r\n");
            printf("###################################################\r\n");
            system("pause");
            //提示错误信息！
            break;
        }
    }
   
}


