void CRocketUI::OnBtn3Click(void)
{
    //QPushButton * button1 = new QPushButton(NULL);
    //button1->move(20, 20);
    //button1->setFixedSize(100, 80);
    //button1->show();
    //状态机
    static QStateMachine machine;
    static QState* group = new QState();
    static QTimer timer;
    timer.setInterval(1250);
    timer.setSingleShot(true);
    QObject::connect(group, SIGNAL(entered()), &timer, SLOT(start()));

    //变化效果
    QState* state1= new QState(group);
    state1->assignProperty(button1, "text", QString::fromLocal8Bit("1"));
    QState* state2= new QState(group);
    state2->assignProperty(button1, "text", QString::fromLocal8Bit("2"));
    QState* state3= new QState(group);
    state3->assignProperty(button1, "text", QString::fromLocal8Bit("3"));
    QState* state4= new QState(group);
    state4->assignProperty(button1, "text", QString::fromLocal8Bit("4"));
    QState* state5= new QState(group);
    state5->assignProperty(button1, "text", QString::fromLocal8Bit("5"));
    QState* state6= new QState(group);
    state6->assignProperty(button1, "text", QString::fromLocal8Bit("6"));
    QState* state7= new QState(group);
    state7->assignProperty(button1, "text", QString::fromLocal8Bit("7"));
    group->setInitialState(state1);
    group->setObjectName("group");

    QFinalState* final = new QFinalState;

    //切换方式
    StateSwitcher* stateSwitcher = new StateSwitcher(&machine);
    stateSwitcher->setObjectName("stateSwitcher");
    group->addTransition(&timer, SIGNAL(timeout()), stateSwitcher);
    stateSwitcher->addState(state1);
    stateSwitcher->addState(state2);
    stateSwitcher->addState(state3);
    stateSwitcher->addState(state4);
    stateSwitcher->addState(state5);
    stateSwitcher->addState(state6);
    stateSwitcher->addState(state7);
    stateSwitcher->addState(final);

    //开始工作
    machine.addState(group);
    machine.addState(final);
    machine.setInitialState(group);
    machine.start();

    //qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
}
