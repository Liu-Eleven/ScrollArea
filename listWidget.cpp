/************************************************************************/
/*版权所有(C) 2016-2020 博众科技  */
/************************************************************************/

/************************************************************************/
/*文件名称：MainFrame.cpp												*/
/*文件描述：把基础控件显示在界面上，测试控件相关功能				        */
/*应用平台：Linux (FC14下编译通过)										*/
/*创建时间：2016-01-05			 										*/
/*内部版本：V1.0 														*/
/*作者：刘世一															*/
/**/
/*修改记录：[修改日期]	[修改人姓名]	[外部版本]	[修改标记]	[修改内容]	*/
/*修改记录：2016-01-05	   刘世一		 10.00.01					创建		*/
/************************************************************************/
/************************************************************************/

#include <QPushButton>
#include <QDebug>

#include "HLabel.h"
#include "listWidget.h"


MListWidget::MListWidget(QWidget * parent, Qt::WindowFlags f)
: QWidget(parent, f)
{


}


MListWidget::~MListWidget()
{

	clearList();
}


void MListWidget::addItem(const QString &buttonName)
{

	HLabel *label = new HLabel(this);
	label->setText(buttonName); //设置文本
	label->setObjectName(buttonName); //设置按钮携带的数据
	label->setAlignment(Qt::AlignCenter);
	//connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked())); //点击按钮由当前界面发射出信号buttonClicked
	label->hide();
	m_buttonVector.append(label);
}


void MListWidget::initUi()
{

	int num = 0;
	int vectNum = m_buttonVector.size();
    qDebug()<<vectNum;
	for (; num < vectNum; num++)
	{
		m_buttonVector[num]->setGeometry(10, num*10+num*40, width()-10, 40);
		m_buttonVector[num]->show();
	}
}


void MListWidget::clearList()
{

	QVector<HLabel*>::iterator it = m_buttonVector.begin();
	for (; it != m_buttonVector.end(); it++)
	{
		if ((*it) != NULL)
		{
			delete (*it);
			(*it) = NULL;
		}
	}

	m_buttonVector.clear();
}


void MListWidget::buttonClicked()
{

	HLabel *label = qobject_cast<HLabel *>(sender());
	if (label)
	{
		QString text = label->objectName();
		emit clicked(text);
	}
}

