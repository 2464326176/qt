#include "mymysql.h"
#include <QDebug>
#include <string.h>
//操作数据库类


mymysql::mymysql()
{

    mysql_init(&mysql);  // 初始化一个MYSQL 连接的实例对象
    connection = NULL;  //初始状态未连接 指针置空
    memset(buf,0,sizeof(buf));//buf置0

}

int mymysql::sql_connect(const char *Hostname, const char *User,
                         const char *Password, const char *DBName){


    connection = mysql_real_connect(&mysql,Hostname,User,Password,DBName,0,0,0);
    if(connection == NULL){
         memset(buf,0,sizeof(buf));
         strcpy(buf,mysql_error(&mysql));
         return -1; //连接为失败

    }else{
        mysql_query(connection, "set names utf8");
        return 0;

    }

}
const char *mymysql::geterror(){
    return buf;
}

//关闭连接
void mymysql::sql_disconnect()
{
    if (connection)
        {
            mysql_close(connection);
            connection = NULL;
        }

}

int mymysql::sql_exec(const char *SQL)
{
    if (mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }
    return 0;
}

int mymysql::sql_open(const char *SQL, QStandardItemModel **p)
{

    
    if (mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(connection);

    if (result == NULL)//没有查询结果
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    int rowcount = mysql_affected_rows(connection);//这个函数返回SQL语句执行后又多少行
    int fieldcount = mysql_field_count(connection);//这个函数返回SQL语句执行后有多少列


    
    *p = new QStandardItemModel(rowcount, fieldcount);//根据SQL语句返回的行列总数，动态的建一个modul出来.
    MYSQL_FIELD *field;

    int i = 0;
    int j = 0;
    for(i = 0;i<fieldcount;i++)
    {
        //表头
        field = mysql_fetch_field(result);

        (*p)->setHeaderData(i, Qt::Horizontal, field->name);
    }


    for(i = 0;i<rowcount;i++)//循环遍历每一行
    {
        //数据
        MYSQL_ROW row = mysql_fetch_row(result);
        for(j = 0;j<fieldcount;j++)//循环遍历一行当中所有的列
        {
            (*p)->setData((*p)->index(i, j, QModelIndex()), row[j]);
        }
    }

    mysql_free_result(result);//释放通过mysql_store_result函数分配的内存空间

    return 0;
}











