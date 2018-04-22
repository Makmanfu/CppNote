
#ifndef WDBGRID_DEF_H
#define WDBGRID_DEF_H

#include <QtCore/QtCore>

const char tb_admin[] = "create table tb_admin"
            "( id       integer primary key AUTOINCREMENT, "
            "  username varchar(255) not null,"
            "  password varchar(255) not null,"
            "  role int "
            ")";

const char tb_user[] = "create table tb_user ( id integer primary key AUTOINCREMENT, username varchar(255) not null,"
"password varchar(255) not null, role int )";

const char tb_topic[] = "create table tb_topic(id integer primary key AUTOINCREMENT, name varchar(255) not null, desc varchar(255),"
"totalTime integer not null, score int not null, creatorID integer not null, createDateTime datetime)";

const char tb_topic_focus[] = "create table tb_topic_focus ("
"id                    integer primary key AUTOINCREMENT,"
"name                  varchar(255) not null,            "
"topicId               integer not null,                 "
"seq                   integer not null,                 "
"focusTime             integer not null,                 "
"warningUpLimit        double not null,                  "
"warningDownLimit      double not null,                  "
"MaxUpLimit            double not null,                  "
"MaxDownLimit          double not null,                  "
"discountValue         int not null)";

const char tb_check_group[] = "create table tb_check_group("
"id                    integer primary key AUTOINCREMENT,"
"name                  varchar(255) not null,           "
"creatorID             integer not null,                 "
"createDateTime        datetime)";

const char tb_trainer[] = "create table tb_trainer("
"id                     integer primary key AUTOINCREMENT, "
"groupId                integer not null,                  "
"name                   varchar(255) not null,             "
"sex                    varchar(255) not null,             "
"workUnit               varchar(255) not null,             "
"Station                varchar(255) not null)";

const char tb_exam_topic[] = "create table tb_exam_topic("
"id                    integer primary key AUTOINCREMENT, "
"userId                integer not null,                  "
"topicId               integer not null,                  "
"groupId               integer not null,                  "
"score                 int not null,                      "
"answerDateTime    	   datetime )";

const char tb_exam_focus[] = "create table tb_exam_focus("
"id                    integer primary key AUTOINCREMENT,"
"userId                integer not null,                 "
"examtopicId           integer not null,                 "
"focusId               integer not null,                 "
"groupId               integer not null,                 "
"score                 int not null,                     "
"answerDateTime    	   datetime)";

const char tb_exam_focus_status[] = "create table tb_exam_focus_status(       "
"userId                integer not null,  "
"examtopicId           integer not null,  "
"examfocusId           integer not null,  "
"groupId               integer not null,  "
"answerDateTime    	datetime,          "
"value                 double,            "
"createDateTime        datetime )";

const char tb_exam_discount_record[] = "create table tb_exam_discount_record(       "
"id                integer primary key AUTOINCREMENT,  "
"userId            integer not null,  "
"examtopicId       integer not null,  "
"discount          integer not null,  "
"reason            varchar(255) not null,  "
"createDateTime    datetime          )";





struct KeyItem
{
    int         id;         //标识
    QString     username;   //用户名
    QString     password;   //密码
    int         role;       //角色,1表示系统管理员,0表示管理员,2表示普通用户
    KeyItem()
    {
        id = role = 0;
        username = password = "";
    };
    KeyItem(QString _username, QString _password, int _role)
    {
        id = 0;
        username = _username;
        password = _password;
        role = _role;
    };
};





#endif

