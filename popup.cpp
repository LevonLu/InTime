#include "popup.h"
#include "ui_popup.h"
#include <QTimer>
#include "lock.h"
#include <QScreen>
#include <QMenu>
#include <QMessageBox>
#include "intime.h"
#include <QMouseEvent>

Popup::Popup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Popup)
{
    ui->setupUi(this);
    mLbTime = this->findChild<QLabel*>("label_time");
    mLbPrompt = this->findChild<QLabel*>("label_prompt");

    // 去除标题栏 及任务栏图标 置顶
    setWindowFlags( windowFlags()&~Qt::Window | Qt::CustomizeWindowHint);
    setWindowFlags( Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);

    // 读取配置文件
    config = new Config();

    // 设置透明度
    double trans = config->getValue({"Popup", "transparency"}).toDouble();
    setWindowOpacity(trans);

    // 设置初始位置
    int x = config->getValue({"Popup", "x"}).toInt();
    int y = config->getValue({"Popup", "y"}).toInt();
    //move(x,y);

    // TODO 获取多个屏幕的信息，创建lock的list 使用lockmanager来管理
    QList<QScreen*> screens = QGuiApplication::screens();
    foreach (QScreen *screen, screens) {
        qDebug() << "Screen:" << screen->name() << "geometry:" << screen->geometry();
        qDebug() << "availableSize:" << screen->availableSize() << "availableVirtualSize:" << screen->availableVirtualSize();
        qDebug() << "availableGeometry:" << screen->availableGeometry() << "availableVirtualGeometry:" << screen->availableVirtualGeometry();
    }
    QRect rc = screens[0]->geometry();
    if(x == 0 && y == 0)
        move(rc.width() - width() - 50, rc.height() - height() - 50);
    else move(x, y);


    // 读取颜色
    QString strCNormal = config->getValue({"Popup","color"}).toString();
    QString strCAlert = config->getValue({"Popup","alert_color"}).toString();
    QString strCText = config->getValue({"Popup","prompt","color"}).toString();
    mColorNormal = new QColor(strCNormal);
    mColorAlert = new QColor(strCAlert);
    mColorText = new QColor(strCText);

    // 更改背景颜色
    QPalette pal;
    pal.setColor(QPalette::Window, *mColorNormal);
    setAutoFillBackground(true);
    setPalette(pal);
    show();

    // 设置字体颜色
    QString strTextStyleSheetColor = QString("color:")+strCText;
    mLbPrompt->setStyleSheet(strTextStyleSheetColor);
    mLbTime->setStyleSheet(strTextStyleSheetColor);

    // 读取配置文件 设置时间

    work_time = config->getValue({"Setting", "time", "work"}).toInt() * 60;
    left_time = work_time;
    //QString strTime = formatTime(work_time);
    //mLbTime->setText(strTime);

    // 启动timer，刷新时间
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Popup::update);
    timer->start(1000);
}

Popup::~Popup()
{

    delete ui;
    delete config;
    delete timer;
    delete mLbPrompt;
    delete mLbTime;
    delete mColorAlert;
    delete mColorNormal;
    delete mColorText;
}

void Popup::update()
{
    if(left_time == 0)
    {
        timer->stop();
        this->destroy();
        Lock *lock = new Lock();
        lock->show();
    } else if (left_time == 5)
    {
        QPalette pal;
        pal.setColor(QPalette::Window, *mColorAlert);
        setPalette(pal);
        show();
    }
    QString strTime = formatTime(left_time);
    mLbTime->setText(strTime);
    left_time--;
    return;
}

void Popup::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPosition().toPoint();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void Popup::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPosition().toPoint() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void Popup::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;

        // 保存位置
        //获得鼠标移动的距离
        QPoint distance = event->globalPosition().toPoint() - mouseStartPoint;
        //改变窗口的位置
        QPoint nowPoint = windowTopLeftPoint + distance;
        config->setValue(QJsonValue(nowPoint.x()), {"Popup", "x"});
        config->setValue(QJsonValue(nowPoint.y()), {"Popup", "y"});
        config->save();
    }
}



void Popup::on_Popup_customContextMenuRequested(const QPoint &pos)
{
    //创建菜单
    QMenu *pMenu = new QMenu(this);

    //设置快捷键为T
    QAction *pSectionSetting = new QAction("首选项(&S)", this);
    QAction *pSectionReset = new QAction("重置计时(&R)", this);
    QAction *pSectionQuit = new QAction("推出(&Q)", this);

    //把QAction对象添加到菜单上
    pMenu->addAction(pSectionSetting);
    //添加分隔线
    //pMenu->addSeparator();
    pMenu->addAction(pSectionReset);
    pMenu->addAction(pSectionQuit);

    //设置点击后发送的数据
    pSectionSetting->setData(1);
    pSectionReset->setData(2);
    pSectionQuit->setData(3);

    //连接鼠标右键点击信号
    connect(pSectionSetting, &QAction::triggered, this, &Popup::OnClickedPopMenu);
    connect(pSectionReset, &QAction::triggered, this, &Popup::OnClickedPopMenu);
    connect(pSectionQuit, &QAction::triggered, this, &Popup::OnClickedPopMenu);

    //在鼠标右键点击的地方显示菜单
    pMenu->exec(cursor().pos());

    //释放内存,若此处不手动释放，则必须等到程序结束时才都能释放
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list)
        delete pAction;
    delete pMenu;
}

void Popup::OnClickedPopMenu()
{
    QAction *pEven = qobject_cast<QAction *>(this->sender());

    //获取发送信息
    int iType = pEven->data().toInt();
    switch (iType)
    {
    case 1: {
        // 设置界面
        InTime *intime = new InTime();
        intime->show();
        timer->stop();
        this->destroy();
        break;
    }
    case 2:{
        // 重置计时
        left_time = work_time;
        QPalette pal;
        pal.setColor(QPalette::Window, *mColorNormal);
        setPalette(pal);
        show();
        update();
        break;
    }
    case 3:
        QCoreApplication::exit();
        break;
    default:
        break;
    }
}

