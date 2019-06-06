/****************************************************************************/
/*      版权所有: Copyright (C), 2010-2020,ShenZhen Bozone IT Co., Ltd.     */
/****************************************************************************/

/****************************************************************************/
/*文件名称：BzScrollAreaPrivate.cpp                                         */
/*文件描述：滚动私有类                                                      */
/*应用平台：Linux (FC14下编译通过)                                          */
/*创建时间：2015-12-21                                                      */
/*内部版本：V3.0                                                            */
/*作者：奇世雅一                                                            */
/*                                                                          */
/*修改记录：[修改日期]  [修改人姓名]   [外部版本]   [修改标记]   [修改内容] */
/*修改记录：2015-12-21    奇世雅一      15.00.01       创建       完成初稿  */
/****************************************************************************/
/****************************************************************************/




#include <QScrollBar>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QDebug>

#include "../BzScrollArea.h"
#include "BzScrollAreaPrivate.h"

BzScrollAreaPrivate::BzScrollAreaPrivate(QObject * parent)
: QObject(parent),q_ptr((BzScrollArea*)(parent))
{
	m_upMaxOffset   = 0;
	m_downMaxOffset = 0;
	m_firstDy		= 0;
	m_lastDy        = 0;
	m_scrollSpeed	= 50;
	m_scrollTime	= 2000;
    m_scrollWidgetHeight	= 0;
    m_scrollAreaHeight	= 0;

	m_isFirst		= false;
    m_barIsShow     = BzScrollArea::ScrollBarAlwaysOff;

	m_pClickedWidget = NULL;
	m_pTargetWidget  = NULL;
	m_pVerticalScrollBar = NULL;
}


BzScrollAreaPrivate::~BzScrollAreaPrivate()
{


}


void  BzScrollAreaPrivate::freshSize()
{
#if 0
	Q_Q(BzScrollArea);
	if (QAbstractAnimation::Running == m_active.state())
	{
		m_active.stop();
	}

	if (m_pTargetWidget)
	{
        m_scrollWidgetHeight = m_pTargetWidget->height();
	}
	else
	{
		return;	//滚动目标窗口为空，返回
	}

	if (m_pVerticalScrollBar)
	{
		m_pVerticalScrollBar->setValue(0);
	}
	else
	{
		return;
	}

	//取得它父窗口的大小，即可视大小
    m_scrollAreaHeight =  q->height();

	m_pTargetWidget->update();
	m_pTargetWidget->show();
	m_pTargetWidget->move(0, 0);

    m_upMaxOffset = m_scrollAreaHeight - m_scrollWidgetHeight; 	//获取往上可以到达最小的位置
	m_upMaxOffset = (m_upMaxOffset >= 0)? 0 : m_upMaxOffset;	//如果滚动窗口大小小于或者等于父窗口大小，设置不滚动

	if (0 <= m_upMaxOffset)
	{
		m_pVerticalScrollBar->hide();
	}
	else
	{
		m_pVerticalScrollBar->setRange(0, -m_upMaxOffset);
		m_pVerticalScrollBar->show();
	}
#endif
}


void  BzScrollAreaPrivate::moveAction(const int &dy1, const int &dy2)
{
	QPoint pos = m_pTargetWidget->pos();  	//当前目标窗口在其父窗口中的起始位置

	int dy = pos.y() + dy2 - dy1;					   //以下两句都为是为防止视图越界看不到
	dy = (dy < m_upMaxOffset)? m_upMaxOffset : dy;     //假如比最小起始坐标小，取最小其实坐标
	dy = (dy > m_downMaxOffset)? m_downMaxOffset : dy; //假如比最大坐标大，取最大坐标

	m_pTargetWidget->move(0, dy);
	if (m_pVerticalScrollBar)
	{
		m_pVerticalScrollBar->setSliderPosition(-dy);
	}
}


void  BzScrollAreaPrivate::decelerateAction(const int &dy1, const int &dy2)
{
	int dy = m_pTargetWidget->pos().y();	//当前目标窗口在其父窗口中的起始位置

	int maxY = dy+m_scrollSpeed*(dy2-dy1);					 //以下两句都为是为防止视图越界看不到
	maxY = (maxY < m_upMaxOffset)? m_upMaxOffset : maxY;     //假如比最小起始坐标小，取最小其实坐标
	maxY = (maxY > m_downMaxOffset)? m_downMaxOffset : maxY; //假如比最大坐标大，取最大坐标

	m_active.setDuration(m_scrollTime);			//设置动画过程时间
	m_active.setKeyValueAt(0, QPoint(0, dy));   //动画开始
	m_active.setEndValue(QPoint(0, maxY));		//动画结束点
	m_active.start();	//开始动画
}


void  BzScrollAreaPrivate::vScrollBarChangeValue(const QVariant &value)
{
	QPoint p = value.toPoint();
	int dy = p.y();
	if (m_pVerticalScrollBar)
	{
		m_pVerticalScrollBar->setSliderPosition(-dy);
	}
}


void NBaseToastr::setShadow(QWidget *w, qreal blurRadius, qreal dx, qreal dy)
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(blurRadius);
    shadow->setColor(QColor(0, 0, 0, 160));
    shadow->setOffset(dx, dy);
    w->setGraphicsEffect(shadow);
}

NBaseToastr::NBaseToastr(QWidget *parent, const QString &message, int w, int h, const QString &style)
    :QLabel(parent)
{
    setText(message);
    setFixedSize(w, h);
    setAlignment(Qt::AlignCenter);
    startx = (parent->width()-width()) / 2;
    starty = parent->height();
    endy = parent->height()*3/4;
    setStyleSheet(style);
    this->setShadow(this);
}

void NBaseToastr::toastr()
{
    QPropertyAnimation *animal= new QPropertyAnimation(this,"pos");
    animal->setDuration(500);
    animal->setStartValue(QPoint(startx,starty));
    animal->setEndValue(QPoint(startx,endy));
    animal->setEasingCurve(QEasingCurve::OutCubic);
    connect(animal, SIGNAL(finished()), this, SLOT(animationFinished()));
    this->show();
    animal->start();
}

void NBaseToastr::animationFinished()
{
    QTimer::singleShot(400, this, SLOT(delSelf()));
}

void NBaseToastr::delSelf()
{
    delete this;
}

