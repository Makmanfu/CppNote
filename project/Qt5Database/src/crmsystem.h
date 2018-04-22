//*************************************************************************
//
//                     基本数据库CRM演示系统 
//
//
//
//*************************************************************************

#ifndef CRMSYSTEM_H
#define CRMSYSTEM_H

#include <QtSql/QtSql>
#include "QPublic.h"
#include "crmsystem_def.h"

//数据库操作
class DBManage : public QObject
{
    Q_OBJECT
public:
    DBManage(QObject* parent = 0);
    ~DBManage();
public:
    //打开连接数据库
    bool openDB(const QString& dbName);
    //关闭断开数据库
    void closeDB();
    //检测数据库打开状态
    bool getDbState(void);
public:
    //创建数据库和表
    bool createDB(const QString& dbName);
    //获取数据库中所有的表名
    void getAllTable(QList<QString>& _tables);
    //获取数据库表
    QSqlTableModel* getTable(QString& tablename);
    //执行SQL语句[创建删除等操作]
    inline bool setquery(QString& sqlStr);
private:
    bool m_dbstate;
};

//数据表操作
class TabModel : public QSqlTableModel
{
    Q_OBJECT
public:
    TabModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual ~TabModel();
//----------------------------内部操作----------------------------
private:
    //显示error
    void debugError(void);
//----------------------------基本操作----------------------------
public:
    //添加数据
    void addRecordData(const KeyItem& _Dat);
    void addRecordDatas(const QList<KeyItem>& _Dats);
    //更新数据
    void updateRecordData(const KeyItem& _Dat);
    //删除数据
    void deleteRecordDataById(int Id);
    void deleteRecordDataByIds(QList<int>& Ids);
    //根据ID查询一条数据
    KeyItem queryById(int Id);
//----------------------------工具操作----------------------------
public:
    //刷数据库sql
    void querySql(void);
    //设置标题显示
    virtual void setHead(void);
    //检测用户登录
    bool CheckUser(const KeyItem& _Dat);
};

//数据表显示
class TabView : public QTableView
{
    Q_OBJECT
public:
    TabView(QWidget * parent = 0);
    virtual ~TabView();
    //界面设计包括菜单设计模板
    virtual void initGui(void);
public:
    //连接数据集
    void conectModel(QAbstractItemModel *model);
protected:
    //鼠标右键菜单
    virtual void contextMenuEvent(QContextMenuEvent* event);
    //添加右键菜单
    void addMenurootItem(QAction* action);
    void addMenuItem(QAction* action);
private:
    QModelIndex m_index0;
    QStandardItem* m_currentItem;
    QMenu *m_popMenu, *m_popMenuroot;
    QAction* Act_Suspen;
    QAction *m_ationShow, *m_ationAdd, *m_ationModify, *m_ationDelete;
signals:
    void add_triggered();
    void edit_triggered();
    void del_triggered();
    void show_triggered();
private slots:
    virtual void On_ShowClicked();
    virtual void On_AddClicked();
    virtual void On_EditClicked();
    virtual void On_DelClicked();
};

//登陆界面
class dlgUserLogin : public QMoveDlg 
{
    Q_OBJECT
public:
    explicit dlgUserLogin(QWidget* parent = 0);
    explicit dlgUserLogin(QWidget* parent, TabModel* _dbmodel);
    ~dlgUserLogin();
    int ret;                    //登陆返回结果
private:
    QGridLayout *gridLayout;
    QLabel *label, *label_2, *label_3;
    QLineEdit *EditUserName, *EditUserPass;
    QComboBox *comboBoxRole;
    QPushButton *BtnOK, *BtnCancle;
signals:
    //验证完抛出信号
    void loginFinished(int _ret, int _pre);
private slots:
    void On_OKClicked();
    void On_CancleClicked();
private:
    TabModel* m_dbmodel;        //数据库操作指针
    int m_checkNum;             //尝试次数
public:
    //设置密码尝试次数
    inline void SetCheckNum(int _val = 3);
};

//用户编辑界面
class dlgUserEdit :public QDialog
{
    Q_OBJECT
public:
    explicit dlgUserEdit(QWidget *parent = 0);
    ~dlgUserEdit();
public:
    KeyItem usrdata;
    //编辑时显示的界面
    void EditShow(void);
private:
    QGridLayout *gridLayout;
    QLabel *label, *label_2, *label_3;
    QLineEdit *EditUserName, *EditUserPass;
    QComboBox *comboBoxRole;
    QPushButton *BtnOK, *BtnCancle;
private slots:
    void On_OKClicked();
    void On_CancleClicked();
};

//数据通用添加编辑界面
class dlgEditDat :public QDialog
{
    Q_OBJECT
public:
    explicit dlgEditDat(QWidget* parent = 0);
    ~dlgEditDat();

    //控件
    struct SItem
    {
        int _itype;         //控件ID 1.EDIT/2.COMBOX
        QLabel* llab;
        QString labname;
        QObject* oobj;
        QVariant _Value;    //值objectName
    };
public:
    //根据数据创建控件布局
    void CreatePanels(QMap<QString, int>& mapIDs) const;
    //设置数据填写
    void SetPanelDatas(QMap<QString, SItem>& mapDats) const;
    //获得数据填写
    void GetPanelDatas(QMap<QString, SItem>& mapDats);
private:
    QGridLayout* gridLayout;
    QWidget* widget;
    //控件设计
    QMap<int, int> mapIDs;
    //控件数据
    QMap<int, int> mapDats;
    //界面控件
    QList<SItem> lisPanels;
};

//主界面UI
class dlgFrame :public QWidget
{
    Q_OBJECT
public:
    explicit dlgFrame(QWidget* _parent = 0);
    explicit dlgFrame(QWidget* _parent, TabModel* _dbmodel);
    ~dlgFrame();
public:
    void InitGui(void);
    void setTabModel(TabModel* _dbmodel);
private:
    TabModel* m_dbmodel;        //数据库操作指针
private:
    QGridLayout *m_grid;
    TabView *m_tableView;               //数据表
    QWidget *m_wPanel;                  //下方面板
    dlgEditDat *m_dlgedit;              //编辑界面
    QHBoxLayout *m_hLay;
    QGroupBox *setBox;                  //控制面板
    QGroupBox *stateBox;                //状态信息
    QVBoxLayout *m_vLaySetBox;
    QHBoxLayout *m_hLay1, *m_hLay2;
    QLabel *m_labtabname, *m_labusername;
    QComboBox *m_comBoxName, *m_comBoxSql;
    QLineEdit *m_lineUser;
    QPushButton *m_btnSql;
    QSpacerItem *m_vSpacer;
};

//基本数据库CRM系统平台
class CrmSystem :public QWidget
{
    Q_OBJECT
public:
    CrmSystem(QWidget* _parent = 0);
    ~CrmSystem();
private:
    void initapp(void);
    void initDlgUser(void);
    void initDlgMain(void);
private slots:
    void loginFinished(int _ret, int _pre);
private:
    QGridLayout* m_grid;
    dlgUserLogin* m_login;              //登陆界面
    dlgFrame* m_dbmain;                 //主界面
    DBManage* m_DBmanger;               //数据库操作
    TabModel* m_tabModel;               //表操作
};





#endif
