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
#ifndef  _HK_MH_LIST_WIDGET_H
#define  _HK_MH_LIST_WIDGET_H


#include <QWidget>
#include <QVector>

class HLabel;
class QPushButton;

class MListWidget : public QWidget
{

	Q_OBJECT

public:

	MListWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~MListWidget();

public:

	void addItem(const QString &buttonName);
	void initUi();

protected:


private:

	void clearList();

signals:

	void clicked(const QString &string);  //点击按钮发射的信号

public slots:

	void buttonClicked();

private:


	QVector<HLabel *>	m_buttonVector; //存放按钮向量
};

#endif
