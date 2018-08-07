
#include "stdafx.h"
#include "crmsystem.h"

DBManage::DBManage(QObject* parent) : QObject(parent)
, m_dbstate(false)
{
}

DBManage::~DBManage()
{
    closeDB();
}

bool DBManage::openDB(const QString& dbName)
{
    //数据库的连接
    QSqlDatabase db;
    if (QSqlDatabase::contains())
    {
        db = QSqlDatabase::database();
        m_dbstate = true;
        return m_dbstate;
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    m_dbstate = db.open();
    return m_dbstate;
}

void DBManage::closeDB()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.close();

    if (QSqlDatabase::contains())
    {
        QSqlDatabase db = QSqlDatabase::database();
        QString databaseName = db.databaseName();
        QSqlDatabase::removeDatabase(databaseName);
    }
    m_dbstate = false;
}

bool DBManage::getDbState(void)
{
    return m_dbstate;
}

bool DBManage::createDB(const QString &dbName)
{
    QFileInfo dbFile(dbName);
    if (!dbFile.isFile())
    {           
        //先打开数据库
        this->openDB(dbName);
        if (!m_dbstate)
            return m_dbstate;
        //第一次创建表结构    
        setquery(QString(tb_admin));
        setquery(QString(tb_user));
        setquery(QString(tb_topic));
        setquery(QString(tb_topic_focus));
        setquery(QString(tb_check_group));
        setquery(QString(tb_trainer));
        setquery(QString(tb_exam_topic));
        setquery(QString(tb_exam_focus));
        setquery(QString(tb_exam_focus_status));
        setquery(QString(tb_exam_discount_record));
        QString sqlStr;
        sqlStr = "insert into tb_admin (username, password,role) values ('admin', 'admin', 1)";
        setquery(sqlStr);
        sqlStr = "insert into tb_user (username, password,role) values ('1', '1', 0)";
        setquery(sqlStr);
    }
    else
    {
        this->openDB(dbName);
        if (!m_dbstate)
            return m_dbstate;
    }
    return true;
}


void DBManage::getAllTable(QList<QString>& _tables)
{
}

QSqlTableModel* DBManage::getTable(QString& tablename)
{
    return NULL;
}

inline bool DBManage::setquery(QString& sqlStr)
{
    QSqlQuery query;
    bool ret = query.exec(sqlStr);
    if (!ret)
    {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}


TabModel::TabModel(QObject * parent, QSqlDatabase db) : QSqlTableModel(parent, db)
{
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置显示标题
    this->setHead();
    //刷新view
    this->select();
}

TabModel::~TabModel()
{
}

void TabModel::querySql(void)
{
    QSqlDatabase db = QSqlDatabase::database("estimate");
    QString sql = "select count(*) from tb_user where username=:p1 and password=:p2 and role=:p3";

    QSqlQuery query(db);
    query.prepare(sql);
    bool ret = query.exec();
    if (!ret)
        return debugError();
}

void TabModel::setHead(void)
{
    this->setTable("tb_user");
    this->setHeaderData(1, Qt::Horizontal, TCSR("用户名称"));
    this->setHeaderData(2, Qt::Horizontal, TCSR("用户密码"));
    this->setHeaderData(3, Qt::Horizontal, TCSR("用户角色"));
}

void TabModel::debugError(void)
{
    QString text = this->lastError().text();
    qDebug() << text;
}

void TabModel::addRecordData(const KeyItem& _Dat)
{
    QSqlField f1("username", QVariant::String);
    QSqlField f2("password", QVariant::String);
    QSqlField f3("role", QVariant::Int);
    f1.setValue(QVariant(_Dat.username));
    f2.setValue(QVariant(_Dat.password));
    f3.setValue(QVariant(_Dat.role));
    QSqlRecord record;
    record.append(f1);
    record.append(f2);
    record.append(f3);

    //插入并提交
    if (!this->insertRecord(-1, record) || !this->submitAll())
        return debugError();
    //获取增加后的ID
    //record = this->record(this->rowCount() - 1);
    //_Dat.id = record.value("id").toInt();
}

void TabModel::addRecordDatas(const QList<KeyItem>& _Dats)
{
    for (QList<KeyItem>::const_iterator it = _Dats.begin(); it != _Dats.end(); ++it)
        addRecordData(*it);
}

void TabModel::updateRecordData(const KeyItem& _Dat)
{
    int rows = this->rowCount();
    for (int row = 0; row < rows; row++)
    {
        QSqlRecord record = this->record(row);
        if (record.value("id").toInt() == _Dat.id)
        {
            record.setValue("username", _Dat.username);
            record.setValue("password", _Dat.password);
            record.setValue("role", _Dat.role);
            this->setRecord(row, record);
            bool ret = this->submitAll();
            if (!ret)
                return debugError();
            break;
        }
    }
}

void TabModel::deleteRecordDataById(int Id)
{
    int rows = this->rowCount();
    for (int row = 0; row < rows; row++)
    {
        QSqlRecord record = this->record(row);
        if (record.value("id").toInt() == Id)
        {
            this->removeRow(row);
            bool ret = this->submitAll();
            if (!ret)
                return debugError();
            break;
        }
    }
}

void TabModel::deleteRecordDataByIds(QList<int>& Ids)
{
    for (QList<int>::const_iterator it = Ids.begin(); it != Ids.end(); ++it)
        deleteRecordDataById(*it);
}

KeyItem TabModel::queryById(int Id)
{
    KeyItem user;
    int rows = this->rowCount();
    for (int row = 0; row < rows; row++)
    {
        QSqlRecord record = this->record(row);
        if (record.value("id").toInt() == Id)
        {
            user.id = Id;
            user.username = record.value("username").toString();
            user.password = record.value("password").toString();
            user.role = record.value("role").toInt();
            break;
        }
    }
    return user;
}

bool TabModel::CheckUser(const KeyItem& _Dat)
{
    int rows = this->rowCount();
    for (int row = 0; row < rows; row++)
    {
        QSqlRecord record = this->record(row);
        if (record.value("username").toString() == _Dat.username
            &&record.value("password").toString() == _Dat.password
            &&record.value("role").toInt() == _Dat.role)
        {
            return true;
        }
    }
    return false;
}


TabView::TabView(QWidget * parent) : QTableView(parent)
{
    //警告此处调用了虚函数
    initGui();
}

TabView::~TabView()
{
}

void TabView::initGui(void)
{
    m_ationAdd = new QAction(TCSR("添加"), this);
    m_ationModify = new QAction(TCSR("修改"), this);
    m_ationDelete = new QAction(TCSR("删除"), this);
    m_ationShow = new QAction(TCSR("刷新数据"), this);
    connect(m_ationAdd, SIGNAL(triggered()), this, SIGNAL(add_triggered()));
    connect(m_ationModify, SIGNAL(triggered()), this, SIGNAL(edit_triggered()));
    connect(m_ationDelete, SIGNAL(triggered()), this, SIGNAL(del_triggered()));
    connect(m_ationShow, SIGNAL(triggered()), this, SIGNAL(show_triggered()));

    connect(m_ationAdd, SIGNAL(triggered()), this, SLOT(On_AddClicked()));
    connect(m_ationModify, SIGNAL(triggered()), this, SLOT(On_EditClicked()));
    connect(m_ationDelete, SIGNAL(triggered()), this, SLOT(On_DelClicked()));
    connect(m_ationShow, SIGNAL(triggered()), this, SLOT(On_ShowClicked()));
    m_popMenu = new QMenu(this);
    addMenuItem(m_ationAdd);
    addMenuItem(m_ationModify);
    addMenuItem(m_ationDelete);
    addMenuItem(m_ationShow);
    m_popMenuroot = new QMenu(this);
    addMenurootItem(m_ationAdd);
    addMenurootItem(m_ationShow);
}

void TabView::conectModel(QAbstractItemModel *model)
{
    this->setModel(model);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    //表头信息显示居左
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    this->verticalHeader()->setVisible(false);
}

void TabView::contextMenuEvent(QContextMenuEvent* event)
{
    QCursor cur = this->cursor();
    m_index0 = this->indexAt(event->pos());
    qDebug() << m_index0.row();
    if (m_index0.row() < 0)
    {
        if (m_popMenuroot != NULL)
            m_popMenuroot->exec(cur.pos());
    }
    else{
        if (m_popMenu != NULL)
            m_popMenu->exec(cur.pos());
    }
    event->accept();
}

void TabView::addMenurootItem(QAction* action)
{
    m_popMenuroot->insertAction(Act_Suspen, action);
}

void TabView::addMenuItem(QAction* action)
{
    m_popMenu->insertAction(Act_Suspen, action);
}

void TabView::On_ShowClicked()
{
}

void TabView::On_AddClicked()
{
}

void TabView::On_EditClicked()
{
}

void TabView::On_DelClicked()
{
}


dlgUserLogin::dlgUserLogin(QWidget* parent) :dlgUserLogin(parent,NULL)
{
}

dlgUserLogin::dlgUserLogin(QWidget* parent, TabModel* _dbmodel) : QMoveDlg(parent)
, m_dbmodel(_dbmodel)
, ret(0)
, m_checkNum(3)
{
    //设置背景
    QPalette pal = palette();
    pal.setBrush(QPalette::All, QPalette::Window, QBrush(QColor(255, 250, 0)));
    setPalette(pal);
    setWindowOpacity(1);
    setAutoFillBackground(true);
    setMouseTracking(true);
    this->setFixedSize(250, 180);
    //this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setWindowTitle(TCSR("用户登陆"));
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    label = new QLabel(this);
    label->setText(TCSR("用户名:"));
    gridLayout->addWidget(label, 0, 0, 1, 1);
    EditUserName = new QLineEdit(this);
    EditUserName->setText(TCSR("admin"));
    gridLayout->addWidget(EditUserName, 0, 1, 1, 2);
    label_2 = new QLabel(this);
    label_2->setText(TCSR("密码:"));
    gridLayout->addWidget(label_2, 1, 0, 1, 1);
    EditUserPass = new QLineEdit(this);
    EditUserPass->setText(TCSR("123"));
    EditUserPass->setEchoMode(QLineEdit::Password);
    gridLayout->addWidget(EditUserPass, 1, 1, 1, 2);
    label_3 = new QLabel(this);
    label_3->setText(TCSR("角色:"));
    gridLayout->addWidget(label_3, 2, 0, 1, 1);
    comboBoxRole = new QComboBox(this);
    comboBoxRole->clear();
    comboBoxRole->insertItem(0, TCSR("普通用户"));
    comboBoxRole->insertItem(1, TCSR("系统管理员"));
    gridLayout->addWidget(comboBoxRole, 2, 1, 1, 2);
    BtnOK = new QPushButton(this);
    BtnOK->setText(TCSR("登陆"));
    gridLayout->addWidget(BtnOK, 3, 1, 1, 1);
    BtnCancle = new QPushButton(this);
    BtnCancle->setText(TCSR("取消"));
    gridLayout->addWidget(BtnCancle, 3, 2, 1, 1);
    connect(BtnOK, SIGNAL(clicked()), this, SLOT(On_OKClicked()));
    connect(BtnCancle, SIGNAL(clicked()), this, SLOT(On_CancleClicked()));
}

dlgUserLogin::~dlgUserLogin()
{
}

void dlgUserLogin::On_OKClicked()
{
    QString tname = EditUserName->text();
    QString tpass = EditUserPass->text();
    int trole = comboBoxRole->currentIndex();

    if (m_dbmodel->CheckUser(KeyItem(tname,tpass,trole)) )
    //if (!tname.compare("admin") && !tpass.compare("admin"))
    {
        ret = 1;
        emit loginFinished(ret, trole);
        close();
    }
    else{
        //3次密码错误直接挂掉
        m_checkNum--;
        if (m_checkNum < 1)
        {
            emit loginFinished(ret, trole);
            close();
        }
    }
}

void dlgUserLogin::On_CancleClicked()
{
    ret = 0;
    emit loginFinished(ret, comboBoxRole->currentIndex());
    close();
}

inline void dlgUserLogin::SetCheckNum(int _val)
{
    m_checkNum = _val;
}


dlgUserEdit::dlgUserEdit(QWidget *parent) :QDialog(parent)
{
    this->setWindowTitle(TCSR("添加用户"));
    this->resize(239, 168);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    label = new QLabel(this);
    label->setText(TCSR("用户名:"));
    gridLayout->addWidget(label, 0, 0, 1, 1);
    EditUserName = new QLineEdit(this);
    EditUserName->setContextMenuPolicy(Qt::NoContextMenu);
    gridLayout->addWidget(EditUserName, 0, 1, 1, 2);
    label_2 = new QLabel(this);
    label_2->setText(TCSR("密码:"));
    gridLayout->addWidget(label_2, 1, 0, 1, 1);
    EditUserPass = new QLineEdit(this);
    EditUserPass->setContextMenuPolicy(Qt::NoContextMenu);
    gridLayout->addWidget(EditUserPass, 1, 1, 1, 2);
    label_3 = new QLabel(this);
    label_3->setText(TCSR("角色:"));
    gridLayout->addWidget(label_3, 2, 0, 1, 1);
    comboBoxRole = new QComboBox(this);
    comboBoxRole->clear();
    comboBoxRole->insertItem(0, TCSR("普通用户"));
    comboBoxRole->insertItem(1, TCSR("系统管理员"));
    gridLayout->addWidget(comboBoxRole, 2, 1, 1, 2);
    BtnOK = new QPushButton(this);
    BtnOK->setText(TCSR("确定"));
    gridLayout->addWidget(BtnOK, 3, 1, 1, 1);
    BtnCancle = new QPushButton(this);
    BtnCancle->setText(TCSR("取消"));
    gridLayout->addWidget(BtnCancle, 3, 2, 1, 1);
    connect(BtnOK, SIGNAL(clicked()), this, SLOT(On_OKClicked()));
    connect(BtnCancle, SIGNAL(clicked()), this, SLOT(On_CancleClicked()));

}

dlgUserEdit::~dlgUserEdit()
{
}

void dlgUserEdit::EditShow()
{
    this->setWindowTitle(TCSR("用户编辑"));
    //编辑时显示
    EditUserName->setText(usrdata.username);
    EditUserPass->setText(usrdata.password);
    comboBoxRole->setCurrentIndex(usrdata.role);
}

void dlgUserEdit::On_OKClicked()
{
    usrdata.username = EditUserName->text();
    usrdata.password = EditUserPass->text();
    usrdata.role = comboBoxRole->currentIndex();
    if (usrdata.username.isEmpty() || usrdata.password.isEmpty())
        QMessageBox::information(this, TCSR("提示"), TCSR("用户名或密码不能为空!"));
    else
        accept();
}

void dlgUserEdit::On_CancleClicked()
{
    reject();
}


dlgEditDat::dlgEditDat(QWidget* parent):QDialog(parent)
{
}

dlgEditDat::~dlgEditDat()
{
}

void dlgEditDat::CreatePanels(QMap<QString, int>& mapIDs) const
{
    //widget = new QWidget(this);
    //gridLayout = new QGridLayout(widget);
    //gridLayout->setSpacing(6);
    //int i(0);
    //for (QMap<QString, int>::const_iterator it = mapIDs.begin(); it != mapIDs.end(); ++it)
    //{
    //    QLabel* label = new QLabel(widget);
    //    gridLayout->addWidget(label, i, 0, 1, 1);
    //    QLineEdit* lineEdit = new QLineEdit(widget);
    //    gridLayout->addWidget(lineEdit, i, 1, 1, 1);
    //}
    //verticalLayout->addWidget(widget);
    //QDialogButtonBox* buttonBox = new QDialogButtonBox(Widget);
    //buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    //verticalLayout->addWidget(buttonBox);
}

void dlgEditDat::SetPanelDatas(QMap<QString, SItem>& mapDats) const
{
}

void dlgEditDat::GetPanelDatas(QMap<QString, SItem>& mapDats)
{
}


dlgFrame::dlgFrame(QWidget* _parent) :dlgFrame(_parent,NULL)
{
}

dlgFrame::dlgFrame(QWidget* _parent, TabModel* _dbmodel) :QWidget(_parent)
, m_tableView(NULL)
, m_dbmodel(_dbmodel)
{
    ////设置背景
    //QPalette pal = palette();
    //pal.setBrush(QPalette::All, QPalette::Window, QBrush(QColor(255, 170, 0)));
    //setPalette(pal);
    //setWindowOpacity(1);
    //setAutoFillBackground(true);
    //setMouseTracking(true);
    //界面设计
    InitGui();
    setTabModel(m_dbmodel);
}

dlgFrame::~dlgFrame()
{
}

void dlgFrame::InitGui(void)
{
    this->setContextMenuPolicy(Qt::NoContextMenu);
    m_tableView = new TabView(this);
    m_wPanel = new QWidget(this);
    m_grid = new QGridLayout(this);
    m_grid->setSpacing(0);
    m_grid->setContentsMargins(0, 0, 0, 0);
    m_grid->addWidget(m_tableView, 0, 0, 1, 1);
    m_grid->addWidget(m_wPanel, 1, 0, 1, 1);
    m_grid->setRowStretch(0, 1);
    m_grid->setRowStretch(1, 1);
    setBox = new QGroupBox(m_wPanel);
    setBox->setTitle(TCSR("控制面板"));
    stateBox = new QGroupBox(m_wPanel);
    stateBox->setTitle(TCSR("状态信息"));
    m_hLay = new QHBoxLayout(m_wPanel);
    m_hLay->setSpacing(0);
    m_hLay->setContentsMargins(0, 0, 0, 0);
    m_hLay->addWidget(setBox);
    m_hLay->addWidget(stateBox);
    m_hLay->setStretch(0, 1);
    m_hLay->setStretch(1, 1);

    //第一排
    m_labtabname = new QLabel(setBox);
    m_labtabname->setText(TCSR("当前表名:"));
    m_comBoxName = new QComboBox(setBox);
    m_labusername = new QLabel(setBox);
    m_labusername->setText(TCSR("当前用户:"));
    m_lineUser = new QLineEdit(setBox);
    m_lineUser->setAlignment(Qt::AlignCenter);
    m_lineUser->setReadOnly(true);
    m_hLay1 = new QHBoxLayout();
    m_hLay1->setSpacing(6);
    m_hLay1->addWidget(m_labtabname);
    m_hLay1->addWidget(m_comBoxName);
    m_hLay1->addWidget(m_labusername);
    m_hLay1->addWidget(m_lineUser);
    m_hLay1->setStretch(1, 1);
    m_hLay1->setStretch(3, 1);
    //第二排
    m_comBoxSql = new QComboBox(setBox);
    m_comBoxSql->setEditable(true);
    m_btnSql = new QPushButton(setBox);
    m_btnSql->setText(TCSR("执行SQL"));
    m_hLay2 = new QHBoxLayout();
    m_hLay2->setSpacing(6);
    m_hLay2->addWidget(m_comBoxSql);
    m_hLay2->addWidget(m_btnSql);
    m_hLay2->setStretch(0, 8);
    m_hLay2->setStretch(1, 1);
    m_vSpacer = new QSpacerItem(20, 218, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_vLaySetBox = new QVBoxLayout(setBox);
    m_vLaySetBox->setSpacing(6);
    m_vLaySetBox->setContentsMargins(11, 11, 11, 11);
    m_vLaySetBox->addLayout(m_hLay1);
    m_vLaySetBox->addLayout(m_hLay2);
    m_vLaySetBox->addItem(m_vSpacer);

}

void dlgFrame::setTabModel(TabModel* _dbmodel)
{
    m_tableView->conectModel(_dbmodel);
}


CrmSystem::CrmSystem(QWidget* _parent) :QWidget(_parent)
, m_DBmanger(NULL)
, m_tabModel(NULL)
{
    this->setMinimumSize(600, 600);
    m_grid = new QGridLayout(this);
    m_grid->setSpacing(0);
    m_grid->setContentsMargins(0, 0, 0, 0);
    initapp();
}

CrmSystem::~CrmSystem()
{
}

void CrmSystem::initapp(void)
{
    m_DBmanger = new DBManage();
    if (!m_DBmanger->createDB("estimate.db"))
        return CUiTool::Instance()->TestBox("DB ERROR!");
    m_tabModel = new TabModel();
    if (m_tabModel==NULL)       //这里有问题
        return CUiTool::Instance()->TestBox("DB ERROR!");
    initDlgUser();
}

void CrmSystem::initDlgUser(void)
{
    //登陆框测试
    m_login = new dlgUserLogin(this, m_tabModel);
    m_login->show();
    m_grid->addWidget(m_login);
    connect(m_login, SIGNAL(loginFinished(int, int)), this, SLOT(loginFinished(int, int)));
}

void CrmSystem::initDlgMain(void)
{
    //主界面
    m_dbmain = new dlgFrame(this, m_tabModel);
    m_dbmain->show();
    m_grid->addWidget(m_dbmain);
}

void CrmSystem::loginFinished(int _ret, int _pre)
{
    if (_ret == 1)
    {
        initDlgMain();
    }
}


