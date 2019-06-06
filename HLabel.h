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


#ifndef _H_HKMH_LABEL_H_
#define _H_HKMH_LABEL_H_


#include <QMap>
#include <QLabel>
#include <QPixmap>

class QMouseEvent;

class HLabel : public QLabel
{

	Q_OBJECT

public:

	HLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~HLabel();

public:


protected:

	void    mousePressEvent(QMouseEvent *e);
	void    mouseMoveEvent(QMouseEvent *e);
	void    mouseReleaseEvent(QMouseEvent *e);

};


#endif
