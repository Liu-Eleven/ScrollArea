#include "widget.h"
#include "listWidget.h"
#include "BzScrollArea.h"
#include <QPushButton>
#include <QLabel>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pMListWidget = NULL;
    m_pScrollArea  = NULL;


    //按钮
    m_pBtn1 = NULL;
    m_pBtn2 = NULL;
    m_pBtn3 = NULL;
    m_pBtnTarget50000 = NULL;
    m_pBtnTarget600 = NULL;

    //与按钮对应的标签
    m_pLabel1 = NULL;
    m_pLabel2 = NULL;
    m_pLabel3 = NULL;
    m_pLabel50000 = NULL;
    m_pLabel600 = NULL;
}

Widget::~Widget()
{

}

void Widget::initUi()
{
    m_pScrollArea = new BzScrollArea(this);
    m_pScrollArea->setObjectName("m_pScrollArea");
    m_pScrollArea->setGeometry(0, 0, 300, 600);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    m_pMListWidget = new MListWidget(m_pScrollArea);
    m_pMListWidget->setObjectName("m_pMListWidget");
    m_pMListWidget->setGeometry(0, 0, m_pScrollArea->width(), 3000);//300控制显示个数

    m_pScrollArea->setWidget(m_pMListWidget);

    int num = 1;
    for (; num <= 1000; num++)
    {
        m_pMListWidget->addItem(tr("我是文本%1").arg(num));
    }
    m_pMListWidget->initUi();

#if 0
    m_pBtn1 = new QPushButton(this);
    m_pBtn1->setGeometry(350, 0, 100, 50);
    m_pBtn1->setObjectName("700");
    m_pBtn1->setText("ScrollArea700");

    m_pBtn2 = new QPushButton(this);
    m_pBtn2->setGeometry(350, 60, 100, 50);
    m_pBtn2->setObjectName("200");
    m_pBtn2->setText("ScrollArea200");

    m_pBtn3 = new QPushButton(this);
    m_pBtn3->setGeometry(350, 140, 100, 50);
    m_pBtn3->setObjectName("400");
    m_pBtn3->setText("ScrollArea400");


    m_pBtnTarget600 = new QPushButton(this);
    m_pBtnTarget600->setGeometry(350, 240, 100, 50);
    m_pBtnTarget600->setObjectName("600");
    m_pBtnTarget600->setText("Target600");

    m_pBtnTarget50000 = new QPushButton(this);
    m_pBtnTarget50000->setGeometry(350, 300, 100, 50);
    m_pBtnTarget50000->setObjectName("50000");
    m_pBtnTarget50000->setText("Target50000");

    m_pLabel1 = new QLabel(this);
    m_pLabel1->setText("scrollWidget height change 700"); //滚动目标父窗口高度改变为700
    m_pLabel1->setGeometry(450, 0, 300, 50);

    m_pLabel2 = new QLabel(this);
    m_pLabel2->setText("scrollWidget height change 200"); //滚动目标父窗口高度改变为200
    m_pLabel2->setGeometry(450, 60, 300, 50);

    m_pLabel3 = new QLabel(this);
    m_pLabel3->setText("scrollWidget height change 400"); //滚动目标父窗口高度改变为400
    m_pLabel3->setGeometry(450, 140, 300, 50);

    m_pLabel50000 = new QLabel(this);
    m_pLabel50000->setText("targetWidget height change 50000"); //滚动目标窗口高度改变为50000
    m_pLabel50000->setGeometry(450, 240, 300, 50);

    m_pLabel600 = new QLabel(this);
    m_pLabel600->setText("targetWidget height change 600");	//滚动目标窗口高度改变为600
    m_pLabel600->setGeometry(450, 300, 300, 50);

    connect(m_pBtn1, SIGNAL(clicked()), this, SLOT(btnClicked()));
    connect(m_pBtn2, SIGNAL(clicked()), this, SLOT(btnClicked()));
    connect(m_pBtn3, SIGNAL(clicked()), this, SLOT(btnClicked()));

    connect(m_pBtnTarget600, SIGNAL(clicked()), this, SLOT(btnClicked1()));
    connect(m_pBtnTarget50000, SIGNAL(clicked()), this, SLOT(btnClicked1()));
#endif
}


void Widget::btnClicked()
{
    QPushButton *pBtn = qobject_cast<QPushButton *>(sender());
    if (pBtn)
    {
        QString string = pBtn->objectName();
        int height = string.toInt();
        if (m_pScrollArea)
        {
            m_pScrollArea->resize(300, height);
        }
    }
}


void Widget::btnClicked1()
{

    QPushButton *pBtn = qobject_cast<QPushButton *>(sender());
    if (pBtn)
    {
        QString string = pBtn->objectName();
        int height = string.toInt();
        if (m_pMListWidget)
        {
            m_pMListWidget->resize(300, height);
        }
    }
}
