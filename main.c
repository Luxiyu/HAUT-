#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // 调用isdigit函数检查注册账号是否为存数字账号
#define N 20   //宏定义
#define M 10000 //宏定义，用于用户信息结构体数组
#define DATA_FILE "./data.dat"
#define MAX_SIZE 100
#define DORM_MAX_NUM 6 // 一个宿舍最多住多少人
//定义住宿信息结构体。
typedef struct dormitory
{
    char dormID[10];
    char studentID[15];
    char sex[10]; // 0:男生，1：女生
    char Name[10];
    char phonenumber[13];
    char vip[3];
} dormitory;
//定义结构体存储系统用户信息
typedef struct userinfo
{
    char Accont[N]; //账号
    char Password[N];//密码
    char ID[N]; //学号
    char People_name[N];//用户姓名
}USER;
//定义住宿信息链表结点，包括 dormitory 型的数据域和指向下一个结点的指针域。
typedef struct node_dormitory
{
    dormitory data;
    struct node_dormitory *next;
} node_dormitory, *p_node_dormitory;

//定义住宿信息链表的头指针为全局变量。
p_node_dormitory headDormitory;

//定义 3 个指针数组，分别按照关键字学号、姓名和宿舍号顺序存储住宿信息的结点地址。
dormitory *arrayDormID[MAX_SIZE], *arrayStudentID[MAX_SIZE], *arrayName[MAX_SIZE];
int countRecord;

void PrintTitle();
void PrintDormitory(dormitory p);
void sortWithDormID();
void sortWithStudentID();
void sortWithName();
void searchWithName(char *key);
void searchWithStudentID(char *key);
void searchWithDormID(char *key);
void writeDataToFile();
int jiaoyanDormNumAndSex(char *key, char *sex);
void add();
void delete();
void edit();
void readFile();
void view();
void find(int type);
void Register_system(); // 用户登录注册菜单函数
void User_reg();// 新用户注册函数
void User_enter();//系统用户登录函数
void Menu_admist();// 管理员菜单
void Menu_user(); // 普通用户菜单
void Menu1();//用户登录注册菜单界面
void Menu2();//系统用户注册界面
void Menu3();//系统用户登录界面
int sign = 0;// 区分用户标识符， 1： 系统管理员，  2：普通用户
char ACCOUNT[N]; // 用于保存普通用户登录后的账号
void Load(USER user[],  int *nptr); //把文本文件中用户信息加载到内存中
void Save(USER user[],  int n); //把用户信息数据保存到文本文件中
void Creat_account(USER user[],  int *nptr);  // 批量创建用户
void De_account(USER user[],  int *nptr); //批量删除用户
void Edit_accont(USER user[],  int *nptr); // 重置账号密码或查看账户密码
void Print_account(USER *sptr); // 输出sptr所指的内容记录
void PrAllAccount(USER user[],  int *nptr); // 查看当前馆内所有用户信息
int FindByNum(USER user[],  int n, char *str);  // 定位待删除账号的位置


int main(void)
{
    int n;
    USER user[M]; //定义结构体数组保存用户信息


    Register_system(); // 登录注册功能  系统管理员登录后sign = 1， 普通用户登录后 sign = 2，
    Load(user, &n); // 把用户信息加载到内存中
    readFile(); //
    if(sign == 1) // 进入系统管理员菜单
    {
        int select;
        FILE *fp;
        while(1)
        {
            Menu_admist();
            printf("请输入菜单选项：\n");
            scanf("%d", &select);
            switch(select)
            {
                case 1: // 批量创建用户
                    Creat_account(user, &n);break;
                case 2: // 批量删除用户
                    De_account(user, &n);break;
                case 3: //修改账号密码
                    Edit_accont(user, &n);break;
                case 4: //重置账号密码
                    PrAllAccount(user, &n);break;
                case 5://显示全部住宿信息 排序
                    view(); break;
                case 6://添加一条住宿信息 查 增
                    add(); break;
                case 7://修改一条住宿信息
                    edit(); break;
                case 8://删除一条住宿信息
                    delete(); break;
                case 9://根据姓名查找
                    find(1); break;
                case 10://根据学号查找
                    find(2); break;
                case 11://根据住宿号查找
                    find(3); break;
                case 0:
                    printf("成功退出系统，期待你的下次使用O(∩_ ∩)O, 祝你生活愉快！\n");
                    writeDataToFile();
                    exit(0);
                default:
                    printf("\n输入错误！无该菜单，输入任意键返回 *系统管理员功能菜单界面* ...\n");
                    scanf("%*c%*c");
                    break;
            }
        }

    }
    if(sign == 2) // 进入系统普通用户菜单
    {
        int select;
        FILE *fp;
        while(1)
        {
            Menu_user();
            printf("请输入菜单选项：\n");
            scanf("%d", &select);
            switch(select)
            {
                case 1:
                    Edit_accont(user, &n); //修改账号密码
                    break;
                case 2:
                    find(1); break;
                case 3:
                    find(2); break;
                case 4:
                    find(3); break;
                case 0:
                    printf("成功退出系统，期待你的下次使用O(∩_ ∩)O, 祝你生活愉快！\n");
                    writeDataToFile();
                    exit(0);
                default:
                    printf("\n输入错误！无该菜单，输入任意键返回 *系统普通用户功能菜单界面* ...\n");
                    scanf("%*c%*c");
                    break;
            }
        }
    }
}
//用户登录注册菜单函数
void Register_system()
{
    int select;
    Menu1();
    scanf("%d", &select);
    switch(select)
    {
        case 0:  //退出系统
            printf("\n成功退出系统！期待你的下次使用O(∩_ ∩)O ，祝你生活愉快！\n");
            exit(0);
        case 1: //登录账户
            User_enter();
            return;
        case 2://注册账号
            User_reg();
            return;
        default: //重新输入选项
            printf("\n输入错误，输入任意键返回 *系统用户登录注册界面* ...\n");
            scanf("%*c%*c");
            system("clear");
            Register_system();
            return;
    }
}


//新用户注册函数
void User_reg()
{
    int select;
    char Accont[N], Password1[N], Password2[N], ID[N], People_name[N];
    FILE *fp;
    Menu2(); // 新用户注册功能菜单
    if((fp = fopen("userinfo.dat", "a")) == NULL) //创建 userinfo.dat 文本文件
    {
        printf("can not open this file\n");
        exit(0);
    }
    fclose(fp);
    scanf("%d", &select);
    switch(select)
    {
        case 1:	    // 注册账号密码 ， 并检验注册账号是否符合规定，是否被注册 。
            while(1)
            {
                FILE *fp;
                int i;
                char Accont1[N], password1[N], ID[N], People_name[N];
                int sign1 = 0; // 检查账号是否为数字账号，0：是  1：否
                system("clear");
                Menu2();
                printf("1\n");
                printf("请输入注册账号（九位数数字）:\n");
                scanf("%s", Accont);
                if((fp = fopen("userinfo.dat", "r")) == NULL)
                {
                    printf("can not open this file\n");
                    exit(0);
                }
                for(i = 0; Accont[i] != '\0'; i++) // 检查是否为九位账号
                {
                    if(isdigit(Accont[i]) == 0)  // 检查账号是否为数字账号
                    {
                        sign1 = 1; break;
                    }
                }
                if(i >= 10 || sign1 == 1 || i < 9)
                {
                    sign1 = 1;
                    printf("未按要求注册账号，请重新注册！ \n");
                    printf("\n输入任意键返回 *系统用户注册界面* ...\n");
                    scanf("%*c%*c");
                    User_reg();
                    return;
                }
                while(!feof(fp)) // 检查账号是否被注册
                {
                    fscanf(fp, "%s %s %s %s", Accont1, password1, ID,  People_name);
                    if(strcmp(Accont, Accont1) == 0)
                    {
                        fclose(fp);
                        printf("该账号已被注册，请重新注册!\n");
                        printf("\n输入任意键返回 *系统用户注册界面* ...\n");
                        scanf("%*c%*c");
                        sign1 = 1;
                        User_reg();
                        return;
                    }
                }
                if(i == 9 && sign1 == 0)
                {
                    printf("注册账号符合要求。");
                    break;
                }

            }
            if((fp = fopen("userinfo.dat", "a")) == NULL)
            {
                printf("can not open this file\n");
                exit(0);
            }
            printf("请输入你的身份证号:\n");
            scanf("%s", ID);
            printf("请输入你的姓名：\n");
            scanf("%s", People_name);
            printf("请输入账号密码：\n");
            scanf("%s", Password1);
            printf("请再次输入密码：\n");
            scanf("%s", Password2);
            if(strcmp(Password1, Password2) == 0)
            {
                fprintf(fp, "\n%s  %s  %s  %s\n", Accont, Password1, ID, People_name);
                fclose(fp);
                printf("****新用户创建成功(￣▽￣)**** \n");
                printf("\n输入任意键返回 *系统用户登录注册界面* ...\n");
                scanf("%*c%*c");
                system("clear");
                Register_system();
                return;
            }else
            {
                printf("\n****两次输入密码不一致!!!! 请重新注册****\n");
                printf("\n输入任意键返回 *系统用户注册界面* ...\n");
                scanf("%*c%*c");
                User_reg();
                return;
            }
        case 2: //返回上一界面
            printf("\n输入任意键返回 *系统用户登录注册界面* ...\n");
            scanf("%*c%*c");
            system("clear");
            Register_system();
            return;
        case 0:  //退出系统
            printf("\n成功退出系统！期待你的下次使用O(∩_ ∩)O，祝你生活愉快！\n");
            exit(0);
        default:  //重新输入
            printf("\n输入错误，无该菜单，输入任意键返回 *系统用户注册界面* ...\n");
            scanf("%*c%*c");
            system("clear");
            User_reg();
            return;
    }
}

//系统用户登录函数
void User_enter()
{
    int select;
    char Accont[N], Password[N];
    char Accont1[N], Password1[N], ID1[N], People_name1[N];
    FILE *fp;
    if((fp = fopen("userinfo.dat", "a")) == NULL) //创建 userinfo.dat 文本文件
    {
        printf("can not open this file\n");
        exit(0);
    }
    fclose(fp);
    Menu3(); // 系统用户登录界面
    scanf("%d", &select);
    switch(select)
    {
        case 1: //登录账号密码
            if((fp = fopen("userinfo.dat", "r")) == NULL)
            {
                printf("can not open this file\n");
                exit(0);
            }
            printf("请输入账号：\n");
            scanf("%s", Accont);
            printf("请输入密码：\n");
            scanf("%s", Password);
            while(!feof(fp))
            {
                fscanf(fp, "%s %s", Accont1, Password1, ID1,  People_name1);
                if(strcmp(Accont, "123456") == 0 && strcmp(Password, "000000") == 0 )  // 图书管理系统管理员账户及密码识别
                {
                    sign = 1;   //系统管理员标识符
                    fclose(fp);
                    printf("账号登录成功！！尊敬的管理员欢迎你使用\n *河 工 大 宿 舍 管 理 员 处 理 系 统* \n");
                    printf("\n输入任意键前往 *系统管理员功能菜单界面* ...\n");
                    scanf("%*c%*c");
                    return;
                }
                if(strcmp(Accont, Accont1) == 0 && strcmp(Password, Password1) == 0)  // 图书管理系统普通用户账号及密码识别
                {
                    strcpy(ACCOUNT, Accont);
                    sign = 2;  //普通用户标识符
                    fclose(fp);
                    printf("账号登录成功！！尊敬的用户欢迎你使用 *河 工 大 宿 舍 用 户 管 理 系 统*\n");
                    printf("\n输入任意键前往 *系统用户功能菜单界面* ...\n");
                    scanf("%*c%*c");
                    return;
                }
            }
            if(sign == 0)
            {
                printf("账号密码错误！！ 请重新输入......\n");
                printf("如若忘记账号密码可携带本人身份证到寝室一楼值班室找回账号密码。\n");
                printf("\n输入任意键返回 *系统用户登录界面* ...\n");
                scanf("%*c%*c");
                User_enter();
                return;
            }
        case 2: //返回上界面
            printf("\n输入任意键返回 *系统用户登录注册界面* ...\n");
            scanf("%*c%*c");
            system("clear");
            Register_system();
            return ;
        case 0: //退出系统
            printf("\n成功退出系统！\n");
            exit(0);
        default: //重新输入
            printf("\n输入错误！无该菜单，输入任意键返回 *系统用户登录界面* ...\n");
            scanf("%*c%*c");
            User_enter();
            return;
    }
}

//把文本文件中用户信息加载到内存中
void Load(USER user[],  int *nptr)
{

    FILE *fp;
    int i = 0;

    if((fp = fopen("userinfo.dat", "r")) == NULL)
    {
        *nptr = 0;
        return;
    }
    while(fscanf(fp,  "%s %s %s %s",  user[i].Accont, user[i].Password, user[i].ID, user[i].People_name) != EOF)
    {
        i++;
    }
    *nptr = i ; //  *nPtr记录文件中当前用户人数
    fclose(fp);
}

// 批量创建用户
void Creat_account(USER user[],  int *nptr)
{
    int i = *nptr;
    printf("请输入待创建用户信息， 输入 0 表示结束添加！\n");
    printf("九位数数字账号 ---- 密码 ---- 学号 ---- 用户姓名：\n");
    while(1)
    {
        scanf("%s", &user[i].Accont);
        if(strcmp(user[i].Accont, "0") == 0)
        {
            break;
        }
        scanf("%s", &user[i].Password);
        scanf("%s", &user[i].ID);
        scanf("%s", &user[i].People_name);
        i++;
    }
    *nptr = i;
    Save(user, *nptr); // //把数据保存到文本文件中
    printf("\n创建成功！！输入任意键返回 *系统管理员功能菜单界面*....... \n");
    scanf("%*c%*c");
}

//把数据保存到文本文件中
void Save(USER user[],  int n)
{
    int i;
    FILE *fp;
    if((fp = fopen("userinfo.dat", "w")) == NULL)
    {
        printf("can not open this file!!");
        exit(0);
    }
    for(i = 0; i < n; i++)
    {
        fprintf(fp, "%s %s %s %s\n",  user[i].Accont, user[i].Password, user[i].ID, user[i].People_name);
    }
    fclose(fp);
}

//批量删除用户
void De_account(USER user[],  int *nptr)
{
    int i, index;
    char account1[N];
    while(printf("\n输入待删除的账号, 注意：输入 0 表示结束输入\n"), scanf("%s", account1), strcmp(account1, "0") != 0)
    {
        index = FindByNum(user, *nptr,  account1);  //定位待删除用户信息在数组的下标
        if(index < 0)
        {
            printf("\n系统内%s账号不存在！！请重新输入\n" , account1);
        }else
        {

            Print_account(&user[index]);
            for(i = index; i < *nptr - 1; i++)
            {
                user[i] = user[i+1];
            }
            (*nptr)--;
            Save(user, *nptr);
            printf("\n删除成功 o(*￣▽￣*)o\n ");
        }
    }
    printf("\n输入任意键返回 *系统管理员功能菜单界面*....... \n");
    scanf("%*c%*c");
}

// 定位待删除账号的位置
int FindByNum(USER user[],  int n, char *str)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(strcmp(user[i].Accont, str) == 0)
        {
            return i;	//返回待删除用户信息在数组的下标
        }
    }
    return -1;
}

// 管理员和用户修改账号密码
void Edit_accont(USER user[],  int *nptr)
{
    int i, index;
    char account1[N], password1[N], password2[N];
    if(sign == 1) //管理员修改密码
    {
        printf("请输入要重置密码或要查看密码的账号:\n");
        scanf("%s", account1);
    }else
    {
        if(sign == 2); // 用户修改密码
        strcpy(account1, ACCOUNT); // 将普通用户登录后的账号存入account1中
    }
    index =  FindByNum(user, *nptr, account1);
    if(index == -1)
    {
        printf("\n系统内无该账号信息！！\n");
        Edit_accont(user, nptr);
        return;
    }
    Print_account(&user[index]);
    while(1)
    {
        printf("\n请输入要更换的密码：\n");
        scanf("%s", password1);
        printf("请再次输入密码：\n");
        scanf("%s", password2);
        if(strcmp(password1, password2) == 0)
        {
            printf("\n密码修改成功 ^_~ \n");
            strcpy(user[index].Password, password2);
            break;
        }else
        {
            printf("\n输入的两次密码不同，请重新修改。＞﹏＜\n");
        }
    }
    Save(user, *nptr);
    printf("\n输入任意键返回 *系统管理员功能菜单界面* ...\n");
    scanf("%*c%*c");
}

//查看当前馆内所有用户信息
void PrAllAccount(USER user[],  int *nptr)
{
    int i;
    printf("\t\t  *所有用户信息*\n");
    printf("%-15s%-15s%-18s%-15s\n","用户账号:", "用户密码:", "用户身份证号:", "用户姓名:");
    for(i = 0; i < *nptr; i++)
    {
        printf("\n%-15s%-15s%-18s%-15s\n", user[i].Accont, user[i].Password, user[i].ID, user[i].People_name);
    }
    printf("\n请输入任意键返回 *系统管理员功能菜单界面* .....\n");
    scanf("%*c%*c");
}

//输出sptr指针所指记录的内容
void Print_account(USER *sptr)
{
    printf("\n%s账号信息为：\n", sptr->Accont);
    printf("%-15s%-15s%-18s%-15s\n","用户账号:", "用户密码:", "用户身份证号:", "用户姓名:");
    printf("%-15s%-15s%-18s%-15s\n", sptr->Accont, sptr->Password, sptr->ID, sptr->People_name);
}
//系统用户登录注册界面
void Menu1()
{
    system("clear");
    printf("===============================================\n");
    printf("                                            \n");
    printf("            欢 迎 使 用 宿 舍 系 统             \n");
    printf("      该界面为：*系统用户登录注册界面*            \n");
    printf("     1: 登录  2：新用户注册  0: 退出系统         \n");
    printf("                                             \n");
    printf("===============================================\n");
    printf("请输入菜单项：\n");
}

//系统用户注册界面
void Menu2()
{
    system("clear");
    printf("====================================\n");
    printf("                                 \n");
    printf("   该界面为：*系统用户注册界面*       \n");
    printf("     1: 注册账号                   \n");
    printf("     2：返回 *用户登录注册界面*      \n");
    printf("     0： 退出系统                  \n");
    printf("                                 \n");
    printf("====================================\n");
    printf("请输入选项：\n");
}

//用户登录注册界面
void Menu3()
{
    system("clear");
    printf("====================================\n");
    printf("                                  \n");
    printf("   该界面为：*系统用户登录界面*       \n");
    printf("      1：登录                      \n");
    printf("      2：返回 *用户登录注册界面*      \n");
    printf("      0: 退出系统                   \n");
    printf("                                   \n");
    printf("====================================\n");
    printf("请输入选项：\n");
}
// 管理员菜单界面
void Menu_admist()
{
    system("clear");
    printf("==========================================\n");
    printf("                                        \n");
    printf("   欢迎来到 *系统管理员功能菜单界面*         \n");
    printf("     1: 批量创建普通用户账号               \n");
    printf("     2: 批量删除普通用户账号                \n");
    printf("     3: 修改单个用户密码或查看用户密码       \n");
    printf("     4：重制单个用户密码                   \n");
    printf("     5：显示全部住宿信息，排序              \n");
    printf("     6：添加一条住宿信息                   \n");
    printf("     7：修改一条住宿信息                   \n");
    printf("     8：删除一条住宿信息                   \n");
    printf("     9：根据姓名查找                     \n");
    printf("     10：根据学号查找                     \n");
    printf("     11：根据宿舍号查找                   \n");
    printf("     0: 退出系统                         \n");
    printf("                                   \n");
    printf("==========================================\n");
}

// 普通用户菜单界面
void Menu_user()
{
    system("clear");
    printf("============================================\n");
    printf("                                          \n");
    printf("   欢迎来到 *系统普通用户功能菜单界面*         \n");
    printf("     1: 重置密码                           \n");
    printf("     2: 根据姓名查找                        \n");
    printf("     3：根据学号查找                        \n");
    printf("     4：根据宿舍号查找                      \n");
    printf("     0: 退出系统                           \n");
    printf("                                          \n");
    printf("============================================\n");
}
//下列函数是输出住宿信息时要调用的函数，为了输出格式便于查看，在输出住宿信息之前输
void PrintTitle()
{
    printf("--------------------------------------------------------------------------------\n");
    printf("      宿舍号 |        学号 |        名字 |       性别|      电话号码｜    寝室长\n");
}
//下列函数是输出结点 p 所代表的住宿信息的内容（宿舍号、学号、姓名）。
void PrintDormitory(dormitory p)
{
    printf(" %10s ", p.dormID);
    printf("| %10s  ", p.studentID);
    printf(" | %10s", p.Name);
    if (strcmp(p.sex, "0") == 0) {
        char *s = "男";
        printf(" | %10s", s);
    } else {
        char *s = "女";
        printf(" | %10s", s);
    }
    printf(" | %10s    ", p.phonenumber);
    printf(" | %8s", p.vip);
    printf("\n");
    printf("\n");
    
}

//下列函数是排序函数，功能是根据宿舍号进行排序，排序时住宿信息链表并不改变，只是把
//宿舍指针数组中的内容根据指针所指的结点的宿舍号大小进行排序。C
void sortWithDormID()
{
    int i, j, k;
    dormitory *tmp;
    for(i = 0; i < countRecord - 1; i++)
    {
        k = i;
        for(j = i + 1; j < countRecord; j++)
            if(strcmp(arrayDormID[j]->dormID, arrayDormID[k]->dormID) < 0)
                k = j;
        if(k != i)
        {
            tmp = arrayDormID[i];
            arrayDormID[i] = arrayDormID[k];
            arrayDormID[k] = tmp;
        }
    }
}
//下列函数是排序函数，功能是根据学号进行排序，排序时住宿信息链表并不改变，只是把学
//号指针数组中的内容根据指针所指的结点的学号大小进行排序 C
void sortWithStudentID()
{
    int i, j, k;
    dormitory *tmp;
    for(i = 0; i < countRecord - 1; i++)
    {
        k = i;
        for(j = i + 1; j < countRecord; j++)
            if(strcmp(arrayStudentID[j]->studentID, arrayStudentID[k]->studentID) < 0)
                k = j;
        if(k != i)
        {
            tmp = arrayStudentID[i];
            arrayStudentID[i] = arrayStudentID[k];
            arrayStudentID[k] = tmp;
        }
    }
}
//下列函数是排序函数，功能是根据姓名进行排序，排序时住宿信息链表并不改变，只是把姓
//名指针数组中的内容根据指针所指的结点的姓名大小进行排序。C
void sortWithName()
{
    int i, j, k;
    dormitory *tmp;
    for(i = 0; i < countRecord - 1; i++)
    {
        k = i;
        for(j = i + 1; j < countRecord; j++)
            if(strcmp(arrayName[j]->Name, arrayName[k]->Name) < 0)
                k = j;
        if(k != i)
        {
            tmp = arrayName[i];
            arrayName[i] = arrayName[k];
            arrayName[k] = tmp;
        }
    }
}

//下列函数是根据姓名查找函数，功能是采用二分查找法在姓名指针数组中查找结点。A
void searchWithName(char *key)
{
    int low, high, mid, i, matchCount; //matchCount 为匹配记录数目
    low = 0;
    high = countRecord - 1;
    matchCount = 0;
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(strcmp(arrayName[mid]->Name, key) == 0)
        {
            PrintTitle();
            i = mid - 1;
            while(i >= low)
            {
                if(strcmp(arrayName[i]->Name, key) == 0)
                    i--;
                else
                    break;
            }
            low = i + 1; //low 此时为匹配的第一条记录下标
            i = mid;
            while(i <= high)
            {
                if(strcmp(arrayName[i]->Name, key) == 0)
                    i++;
                else
                    break;
            }
            high = i - 1; //high 此时为匹配的最后一条记录下标
            matchCount = high - low + 1;
            for(i = low; i <=high; i++)
                PrintDormitory(*arrayName[i]);
            printf("\n共找到%d条数据\n", matchCount);
            getchar();
            return;
        }
        else if(strcmp(arrayName[mid]->Name, key) < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    printf("\n404 没有找到");
    getchar();
}
//下列函数是根据学号查找函数，功能是采用二分查找法在学号指针数组中查找结点。A
void searchWithStudentID(char *key)
{
    int low, high, mid;
    low = 0;
    high = countRecord - 1;
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(strcmp(arrayStudentID[mid]->studentID, key) == 0)
        {
            PrintTitle();
            PrintDormitory(*arrayStudentID[mid]);
            printf("\n找到1条数据\n");
            getchar();
            return;
        }
        else if(strcmp(arrayStudentID[mid]->studentID, key) < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    printf("\n404 没有找到");
    getchar();
}
//下列函数是根据宿舍号查找函数，功能是采用二分查找法在宿舍指针数组中查找结点。A
void searchWithDormID(char *key)
{
    int low, high, mid, i, matchCount; //matchCount 为匹配记录数目
    low = 0;
    high = countRecord - 1;
    matchCount = 0;
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(strcmp(arrayDormID[mid]->dormID, key) == 0)
        {
            PrintTitle();
            i = mid - 1;
            while(i >= low)
            {
                if(strcmp(arrayDormID[i]->dormID, key) == 0)
                    i--;
                else
                    break;
            }
            low = i + 1; //low 此时为匹配的第一条记录下标
            i = mid;
            while(i <= high)
            {
                if(strcmp(arrayDormID[i]->dormID, key) == 0)
                    i++;
                else
                    break;
            }
            high = i - 1; //high 此时为匹配的最后一条记录下标
            matchCount = high - low + 1;
            for(i = low; i <=high; i++)
                PrintDormitory(*arrayDormID[i]);
            printf("\n共找到%d条数据!\n", matchCount);
            getchar();
            return;
        }
        else if(strcmp(arrayDormID[mid]->dormID, key) < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    printf("\n404 没有找到");
    getchar();
}
//下列函数是写入文件，功能是把住宿信息链表中的内容保存到数据文件中。添加、修改、删
//除链表的函数完成操作后，只是链表的内容改变了，并没有把修改后的内容写入到数据文件中，
//只有调用下列函数，修改后的内容才写入数据文件中。D
void writeDataToFile()
{
    FILE *fp;
    p_node_dormitory pNodeDormitory;
    int count = 0;
    if((fp = fopen(DATA_FILE, "wb+")) == NULL)
    {
        printf("文件已损坏!\n");
        return;
    }
    pNodeDormitory = headDormitory;
    while(pNodeDormitory != NULL)
    {
        if(fwrite(&pNodeDormitory->data, sizeof(dormitory), 1, fp) != 1)
            printf("文件写入失败!\n");
        pNodeDormitory = pNodeDormitory->next;
        count++;
    }
    fclose(fp);
    printf("成功写入%d条数据!\n", count);
    getchar();
}
// 通过宿舍号查询到对应的信息  -1代表失败  0 代表添加成功 A
int jiaoyanDormNumAndSex(char *key, char *sex)
{
    // 男女不能混住
    int low, high, mid, index, matchCount; //matchCount 为匹配记录数目
    low = 0;
    high = countRecord - 1;
    matchCount = 0;
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(strcmp(arrayDormID[mid]->dormID, key) == 0)
        {
            index = mid - 1;
            while(index >= low)
            {
                if(strcmp(arrayDormID[index]->dormID, key) == 0)
                    index--;
                else
                    break;
            }
            low = index + 1; //low 此时为匹配的第一条记录下标
            index = mid;
            while(index <= high)
            {
                if(strcmp(arrayDormID[index]->dormID, key) == 0)
                    index++;
                else
                    break;
            }
            high = index - 1; //high 此时为匹配的最后一条记录下标
            matchCount = high - low + 1;
            if (matchCount == DORM_MAX_NUM) {
                printf("\n添加失败😭\n");
                printf("\n%s号宿舍只能住%d个人，目前已经住满！\n", key, DORM_MAX_NUM);
                getchar();
                return -1;
            }
            for(index = low; index <=high; index++) {
                if (strcmp(arrayDormID[index]->sex, sex) != 0) {
                    printf("\n添加失败😭\n");
                    if (strcmp(arrayDormID[index]->sex, "0") == 0) {
                        printf("\n%s号宿舍是男宿舍不能住女生\n", key);
                    } else {
                        printf("\n%s号宿舍是女宿舍不能住男生\n", key);
                    }

                    getchar();
                    return -1;
                } else {
                    low = 1;
                    high = 0;
                }
            }
        }
        else if(strcmp(arrayDormID[mid]->dormID, key) < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;

}

//下列函数是添加住宿信息的函数，功能是在住宿信息链表的表头插入一个新的结点，并把该
//结点的地址分别添加到宿舍号指针数组、姓名指针数组和学号指针数组的末尾，然后把宿舍号指
//针数组、姓名指针数组和学号指针数组分别排序。B
void add()
{
    char dormID[10];
    char studentID[15];
    char name[10];
    char sex[10];
    char phonenumber[15];
    char vip[10];
    printf("\n 宿舍号： ");
    scanf("%s", dormID);
    printf("\n 学号： ");
    scanf("%s", studentID);
    printf("\n 名字： ");
    scanf("%s", name);
    printf("\n 性别（0：男，1：女）： ");
    scanf("%s", sex);
    printf("\n 电话号码： ");
    scanf("%s", phonenumber);
    printf("\n 寝室长： ");
    scanf("%s", vip);

    // 校验宿舍是否住满，男女不能混住
    if (jiaoyanDormNumAndSex(dormID, sex) == -1) return;



    int i, j, k;
    p_node_dormitory pNodeDormitory;
    pNodeDormitory = (p_node_dormitory)malloc(sizeof(node_dormitory));


    strcpy(pNodeDormitory->data.dormID, dormID);
    strcpy(pNodeDormitory->data.studentID, studentID);
    strcpy(pNodeDormitory->data.Name, name);
    strcpy(pNodeDormitory->data.sex, sex);
    strcpy(pNodeDormitory->data.phonenumber, phonenumber);
    strcpy(pNodeDormitory->data.vip, vip);

    if(headDormitory == NULL)
    {
        headDormitory = pNodeDormitory;
        pNodeDormitory->next = NULL;
    }
    else
    {
        pNodeDormitory->next = headDormitory;
        headDormitory = pNodeDormitory;
    }
    //添加一条记录后重新排序住宿号指针数组
    for(i = 0; i < countRecord; i++)
    {
        if(strcmp(arrayDormID[i]->dormID, &headDormitory->data.dormID) > 0)
            break;
    }
    for(j = countRecord; j > i; j--)
        arrayDormID[j] = arrayDormID[j - 1];
    arrayDormID[i] = &headDormitory->data;
    //添加一条记录后重新排序学号指针数组
    for(i = 0; i < countRecord; i++)
    {
        if(strcmp(arrayStudentID[i]->studentID, &headDormitory->data.studentID) > 0)
            break;
    }
    for(j = countRecord; j > i; j--)
        arrayStudentID[j] = arrayStudentID[j - 1];
    arrayStudentID[i] = &headDormitory->data;
    //添加一条记录后重新排序姓名指针数组
    for(i = 0; i < countRecord; i++)
    {
        if(strcmp(arrayName[i]->Name, &headDormitory->data.Name) > 0)
            break;
    }
    for(j = countRecord; j > i; j--)
        arrayName[j] = arrayName[j - 1];
    arrayName[i] = &headDormitory->data;
    countRecord++;
    PrintTitle();
    PrintDormitory(pNodeDormitory->data);
    getchar();
}
//下列函数是删除住宿信息的函数，功能是根据学号查找到要删除的结点，在信息链表中删除
//该结点，同时把宿舍号指针数组、姓名指针数组和学号指针数组中与该结点相关的指针域置为空。B
void delete()
{
    p_node_dormitory pNodeDormitory, p1;
    char sID[10];
    int i, j;
    p1 = headDormitory;
    printf("\n请输入要删除的学号：");
    scanf("%s", sID);
    if(strcmp(p1->data.studentID, sID) == 0)
    {
        headDormitory = p1->next;
        pNodeDormitory = p1;
    }
    else
    {
        pNodeDormitory = p1->next;
        while(pNodeDormitory != NULL)
        {
            if(strcmp(pNodeDormitory->data.studentID, sID) == 0)
            {
                p1->next = pNodeDormitory->next;
                break;
            }
            p1 = pNodeDormitory;
            pNodeDormitory = pNodeDormitory->next;
        }
    }
    if(pNodeDormitory)
    {
        //删除一条记录后重新排序住宿号指针数组
        for(i = 0; i < countRecord; i++)
            if(arrayDormID[i] == &pNodeDormitory->data)
                break;
        for(j = i; j < countRecord - 1; j++)
            arrayDormID[j] = arrayDormID[j + 1];
        //删除一条记录后重新排序学号指针数组
        for(i = 0; i < countRecord; i++)
            if(arrayStudentID[i] == &pNodeDormitory->data)
                break;
        for(j = i; j < countRecord - 1; j++)
            arrayStudentID[j] = arrayStudentID[j + 1];
        //删除一条记录后重新排序姓名指针数组
        for(i = 0; i < countRecord; i++)
            if(arrayName[i] == &pNodeDormitory->data)
                break;
        for(j = i; j < countRecord - 1; j++)
            arrayName[j] = arrayName[j + 1];
        free(pNodeDormitory);
        countRecord--;
        printf("\n学号%s已删除", sID);
    }
    else
        printf("\n404 没有该学生");
    getchar();
}
//下列函数是修改住宿信息函数，首先根据输入的学号找到要修改的住宿信息结点，然后修改
//该结点的数据域（宿舍号、学号、姓名）。B
void edit()
{
    p_node_dormitory pNodeDormitory;
    char sID[10];
    pNodeDormitory = headDormitory;
    printf("\n请输入你想编辑的学号");
    printf("\n学号： ");
    scanf("%s", sID);
    while(pNodeDormitory != NULL)
    {
        if(strcmp(pNodeDormitory->data.studentID, sID) == 0)
        {
            PrintTitle();
            PrintDormitory(pNodeDormitory->data);
            printf("\n 编辑学号： ");
            scanf("%s", pNodeDormitory->data.dormID);
            printf("\n 编辑宿舍号： ");
            scanf("%s", pNodeDormitory->data.studentID);
            printf("\n 编辑名字： ");
            scanf("%s", pNodeDormitory->data.Name);
            printf("\n 编辑性别（0：男，1：女）： ");
            scanf("%s", pNodeDormitory->data.Name);
            printf("\n\n\n");
            PrintTitle();
            PrintDormitory(pNodeDormitory->data);
            printf("\n学号%s已编辑成功!", pNodeDormitory->data.studentID);
            getchar();
            return;
        }
        pNodeDormitory = pNodeDormitory->next;
    }
    printf("\n404 找不到对应的学生");
    getchar();
}
//下列函数是读入文件的函数，把事先保存在数据文件中的住宿信息读入到内存中，创建住宿
//信息组成的单链表，同时把链表结点的地址保存到住宿号指针数组、学号指针数组和姓名指针数
//组中。D
void readFile()
{
    FILE *fp;
    p_node_dormitory pNodeDormitory;
    int size;
    countRecord = 0;
    fp = fopen(DATA_FILE, "rb");
    fseek(fp, 0, 2); //得到文件大小
    size = ftell(fp);
    rewind(fp);
    while(size != 0)
    {
        size -= sizeof(struct dormitory);
        pNodeDormitory = (p_node_dormitory)malloc(sizeof(node_dormitory));
        if(fread(&pNodeDormitory->data, sizeof(struct dormitory), 1, fp) != 1)
            exit(0);
        pNodeDormitory->next = headDormitory;
        headDormitory = pNodeDormitory;
        arrayDormID[countRecord] = &headDormitory->data;
        arrayStudentID[countRecord] = &headDormitory->data;
        arrayName[countRecord] = &headDormitory->data;
        countRecord++;
    }
    fclose(fp);
    sortWithDormID();
    sortWithStudentID();
    sortWithName();
    printf("获取到%d条数据!\n", countRecord);
}
//下列函数是显示住宿信息的函数，功能是把目前住宿信息链表中的所有结点内容显示出来。
//首先按照链表顺序显示链表内容（宿舍号、学号、姓名），然后分别按照姓名、学号、宿舍号为关
//键字排序后显示住宿信息（宿舍号、学号、姓名）。C
void view()
{
    int count = 0;
    p_node_dormitory pNodeDormitory;
    pNodeDormitory = headDormitory;
    PrintTitle();
    while(pNodeDormitory != NULL)
    {
        PrintDormitory(pNodeDormitory->data);
        pNodeDormitory = pNodeDormitory->next;
        count++;
    }
    printf("共有%d条数据\n", count);
    printf("\n\n通过名字排序\n");
    PrintTitle();
    for(count = 0; count < countRecord; count++)
    {
        PrintDormitory(*arrayName[count]);
    }
    printf("\n\n通过学号排序\n");
    PrintTitle();
    for(count = 0; count < countRecord; count++)
    {
        PrintDormitory(*arrayStudentID[count]);
    }
    printf("\n\n通过宿舍号排序\n");
    PrintTitle();
    for(count = 0; count < countRecord; count++)
    {
        PrintDormitory(*arrayDormID[count]);
    }
    getchar();
}
//下列函数是查找函数，进入查找函数后再次进行选择查找的方式。A
void find(int type)
{
    char *typeName = NULL;
    switch(type)
    {
        case 1:
            typeName="学生名字"; break;
        case 2:
            typeName="学号"; break;
        case 3:
            typeName="宿舍号"; break;
    }
    char key[10];
    printf("\n请输入要查找的%s：", typeName);
    scanf("%s", key);
    switch(type)
    {
        case 1:
            searchWithName(key); break;
        case 2:
            searchWithStudentID(key); break;
        case 3:
            searchWithDormID(key); break;
    }
}



















