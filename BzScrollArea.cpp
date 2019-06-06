/****************************************************************************/
/*      版权所有: Copyright (C), 2010-2020,ShenZhen Bozone IT Co., Ltd.     */
/****************************************************************************/

/****************************************************************************/
/*文件名称：BzScrollArea.cpp                                                */
/*文件描述：滚动区域控件                                                    */
/*应用平台：Linux (FC14下编译通过)                                          */
/*创建时间：2015-12-21                                                      */
/*内部版本：V3.0                                                            */
/*作者：奇世雅一                                                            */
/*                                                                          */
/*修改记录：[修改日期]  [修改人姓名]   [外部版本]   [修改标记]   [修改内容] */
/*修改记录：2015-12-21    奇世雅一      15.00.01       创建       完成初稿  */
/****************************************************************************/
/****************************************************************************/


#include <QEvent>
#include <QPoint>
#include <QDebug>
#include <synchapi.h>

#include  "BzScrollArea.h"
#include  "BzScrollAreaPrivate.h"

//在手指松开屏幕的最后两次滚动事件中,手指划过的距离超过这个值，即进入滚动
const int fingerAccuracyThreshold = 3;


BzScrollArea::BzScrollArea(QWidget * parent)
    : QScrollArea(parent),d_ptr(new BzScrollAreaPrivate(this))
{
	setFocusPolicy(Qt::NoFocus);

	//更改边框样式
    /*this->setStyleSheet(
				    "QScrollArea, QListView {"
				    "border: 2px solid grey;"
				    " }"
            );
	//背景透明
	QPalette pal = this->palette();
	pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0x00));
    setPalette(pal);*/

    setScrollBarShow();
}

BzScrollArea::~BzScrollArea()
{


}

void BzScrollArea::setScrollBarShow()
{
	this->verticalScrollBar()->setStyleSheet(	"QScrollBar:vertical"
                                       "{"
                                       "width:8px;"
                                       "background:rgba(0,0,0,0%);"
                                       "margin:0px,0px,0px,0px;"
                                       "padding-top:0px;"   // 留出9px给上面和下面的箭头
                                       "padding-bottom:0px;"
                                       "}"
                                       "QScrollBar::handle:vertical"
                                       "{"
                                       "width:8px;"
                                       "background:rgba(0,0,0,25%);"
                                       " border-radius:4px;"    // 滚动条两端变成椭圆
                                       "min-height:20;"//设置滚动条的最小高度
                                       "}"
                                       "QScrollBar::handle:vertical:hover"
                                       "{"
                                       "width:8px;"
                                       "background:rgba(0,0,0,50%);"    // 鼠标放到滚动条上的时候，颜色变深
                                       " border-radius:4px;"
                                       "min-height:20;"
                                       "}"
                                       "QScrollBar::add-line:vertical" // 这个应该是设置下箭头的，3.png就是箭头
                                       "{"
                                       "height:9px;width:0px;"
                                      // "border-image:url(:/img/3.png);"
                                       "subcontrol-position:bottom;"
                                       "}"
                                       "QScrollBar::sub-line:vertical"  // 设置上箭头
                                       "{"
                                       "height:9px;width:0px;"
                                      // "border-image:url(:/img/1.png);"
                                       "subcontrol-position:top;"
                                       "}"
                                       "QScrollBar::add-line:vertical:hover"// 当鼠标放到下箭头上的时候
                                       "{"
                                       "height:9px;width:8px;"
                                      // "border-image:url(:/img/4.png);"
                                       "subcontrol-position:bottom;"
                                       "}"
                                       "QScrollBar::sub-line:vertical:hover"// 当鼠标放到上箭头上的时候
                                       "{"
                                       "height:9px;width:8px;"
                                      // "border-image:url(:/img/2.png);"
                                       "subcontrol-position:top;"
                                       "}"

                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"    // 当滚动条滚动的时候，上面的部分和下面的部分
                                       "{"
                                       "background:rgba(0,0,0,10%);"
                                       "border-radius:4px;"
                                       "}"
                                 );


}


void  BzScrollArea::setWidget(QWidget * widget)
{
	Q_D(BzScrollArea);
	QScrollArea::setWidget(widget);
	d->m_pTargetWidget = qobject_cast<QWidget*>(widget);
	
	if (d->m_pTargetWidget)
	{
		
        d->m_scrollWidgetHeight  = d->m_pTargetWidget->height();
        //qDebug()<<"滚动窗口(视窗)的高度 m_scrollWidgetHeight=:" <<d->m_scrollWidgetHeight;

        //取得滚动区域大小，即可视大小
        d->m_scrollAreaHeight = this->height();
        //qDebug()<<"滚动区域(m_scrollAreaHeight)的高度"<<d->m_scrollAreaHeight;

        //获取往上可以到达最小的位置
        d->m_upMaxOffset = d->m_scrollAreaHeight - d->m_scrollWidgetHeight;
        //qDebug()<<"往上到达的最小位置:"<<d->m_upMaxOffset;

		d->m_active.setTargetObject(d->m_pTargetWidget);		//为目标窗口设置动画
		d->m_active.setPropertyName("pos");				//为目标窗口设置位置属性
		d->m_active.setEasingCurve(QEasingCurve::OutQuint);//为目标窗口设置减速效果

        //如果滚动区域大于等于要滚动展示数据的窗口，则不滚动
		d->m_upMaxOffset = (d->m_upMaxOffset >= 0)? 0 : d->m_upMaxOffset;

		d->m_pVerticalScrollBar = this->verticalScrollBar();
        //增加一个信号,用来隐藏垂直滚动条
        connect(d->m_pVerticalScrollBar , SIGNAL(sliderReleased()) , this , SLOT(ScrollBarIsHide()));
		if (0 <= d->m_upMaxOffset)
		{
			d->m_pVerticalScrollBar->hide();
		}
		else
		{
			d->m_pVerticalScrollBar->setRange(0, -d->m_upMaxOffset);
            //d->m_pVerticalScrollBar->hide();
		}

		//控制界面滑动、滚动与滚动条同步
        connect(&d->m_active, SIGNAL(valueChanged(const QVariant &)), d, SLOT(vScrollBarChangeValue(const QVariant &)));
        connect(&d->m_active, SIGNAL(finished()), this, SLOT(ScrollBarIsHide()) );
        //在界面滚动的过程中，点击滚动条滑动会出现bug。连接这个信号槽解决
		connect(d->m_pVerticalScrollBar, SIGNAL(sliderPressed()), &d->m_active, SLOT(stop()));
	}
	else
	{
        qDebug()<<"获取滚动面板失败!!!";
	}
}


QWidget *BzScrollArea::takeWidget()
{
	Q_D(BzScrollArea);
	d->m_pTargetWidget = NULL;
	d->m_upMaxOffset	= 0;
	return QScrollArea::takeWidget();
}


void  BzScrollArea::setScrollSpeed(const int &speed)
{
	Q_D(BzScrollArea);
	d->m_scrollSpeed = speed;
}


int  BzScrollArea::scrollSpeed()
{
	Q_D(BzScrollArea);
	return d->m_scrollSpeed;
}


void  BzScrollArea::setScrollTime(const int &time)
{
	Q_D(BzScrollArea);
	d->m_scrollTime = time;
}


int  BzScrollArea::scrollTime()
{
	Q_D(BzScrollArea);
	return d->m_scrollTime;
}

void BzScrollArea::ScrollBarIsHide()
{
    Q_D(BzScrollArea);
    Sleep(220);
    d->m_pVerticalScrollBar->hide();
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    int vPosition = d->m_pVerticalScrollBar->sliderPosition();

    if(0 == vPosition)
    {
        NBaseToastr * tempToa = new NBaseToastr(this, "已经到达顶部");
        tempToa->toastr();
    }
    else if( vPosition == qAbs(d->m_upMaxOffset) )
    {
        NBaseToastr * tempToa = new NBaseToastr(this, "已经到达底部");
        tempToa->toastr();
    }

}

void BzScrollArea::setScrollBarState(ScrollBarState state)
{
    Q_D(BzScrollArea);

    d->m_barIsShow = state;

    switch (state)
    {
    case ScrollBarState::ScrollBarAsNeeded:
        break;
    case ScrollBarState::ScrollBarAlwaysOff:
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        break;
    case ScrollBarState::ScrollBarAlwaysOn:
        break;
    default:
        qDebug()<<"state:"<<state<<"error!!!LINE:"<<__LINE__;
    }
}


bool  BzScrollArea::eventFilter(QObject *object, QEvent *event)
{
	Q_D(BzScrollArea);

	if (object != d->m_pTargetWidget || NULL == d->m_pTargetWidget)	//如果目标不等于object，直接退出。事件原路传递
	{
		return QScrollArea::eventFilter(object, event);
	}
	else if (0 <= d->m_upMaxOffset && event->type() == QEvent::Resize)//如果是改变窗口大小事件调用freshSize()
	{
		d->freshSize();
		return QScrollArea::eventFilter(object, event);
	}
	else if (0 <= d->m_upMaxOffset) //如果目标窗口为空，或者滚动窗口比它的父窗口小(不进行滚动)，直接返回
	{
		return QScrollArea::eventFilter(object, event);
	}

	QWidget *widget = qobject_cast<QWidget*>(object);	//转换成QWidget类型指针,后面有用到

	const QEvent::Type type = event->type();	//获取事件类型，下面用来判断

	switch (type)
	{
		case QEvent::Resize:
			{
				d->freshSize();
			   return QScrollArea::eventFilter(object, event);
			}

		case QEvent::MouseButtonPress:	//只截取这三种事件，别的事件原路传递
		case QEvent::MouseMove:
		case QEvent::MouseButtonRelease:
			break;
		default:
			return false;
	}

	QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
	if (QEvent::MouseMove == type && Qt::LeftButton != mouseEvent->buttons())
	   return false;

	if (mouseEvent->modifiers() != Qt::NoModifier)
	   return false;

	int mouseDy = mouseEvent->globalY();  //获取鼠标相对于屏幕的Y轴坐标,必须是相对于屏幕的

	if (QEvent::MouseButtonPress == type)
	{
        d->m_active.stop();	 //点击目标窗口，终止之前的滚动效果
		d->m_isFirst = true;    //后面有用到这个属性

		//这里获取按下窗口，因为
		d->m_pClickedWidget = widget;

		d->m_firstDy = mouseEvent->globalY(); //获取第一次鼠标按下相对于屏幕的Y轴坐标
		d->m_PressDy = d->m_firstDy;
		d->m_lastDy  = d->m_firstDy;	    //这里设置相等是为了静止不动时，要不然
                                            //会取上次滑动的Y坐标，导致滑动
	}
	else if (QEvent::MouseButtonRelease == type)
	{
		if (d->m_pClickedWidget != widget)	//因为事件过滤器的原因(会重复收到这个事件)，只处理点击处的窗口控件，其他的原路传递
		{
			return false;
		}

        if (d->m_firstDy != d->m_lastDy) //因为最后的滚动时间Y坐标和释放事件的Y坐标一致，在此处理
		{
			int dsY = d->m_lastDy - d->m_firstDy;
			int dy = qAbs(dsY);

			if (dy > fingerAccuracyThreshold) //当最后两次滚动事件中,手指划过的距离超过这个值，即进入滚动
            {
                //启动滚动
                d->decelerateAction(d->m_firstDy, d->m_lastDy);
				return true;
			}
			d->m_firstDy = d->m_lastDy;
		}

        //不启用加减速时滚动完毕，隐藏进度条
        //Sleep(220);//延时一下，不至于松开按键就消失
        ScrollBarIsHide();
		if (3 < (d->m_lastDy - d->m_PressDy))
		{
			return true;
		}

		return false;
	}
	else if (QEvent::MouseMove == type)
	{
        //因为事件过滤器的原因(会重复收到这个事件)，只处理点击处的窗口控件，其他的原路传递
        if (widget != d->m_pClickedWidget)
		{
			return false;
		}

		if (d->m_isFirst)
		{
			d->m_lastDy = mouseDy;
			d->m_isFirst = false;
		}
		else
		{

            if (d->m_lastDy !=mouseDy)
			{
				d->m_firstDy = d->m_lastDy;
                d->m_lastDy = mouseDy;
			}
			else
			{
				return false;
			}
		}

        //开启显示
        d->m_pVerticalScrollBar->show();
        //qDebug()<<d->m_pVerticalScrollBar->sliderPosition();
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        //手跟着滑动效果
        d->moveAction(d->m_firstDy, d->m_lastDy);

		return true;
	}

	return false;
}


void  BzScrollArea::resizeEvent(QResizeEvent *event)
{
	Q_D(BzScrollArea);
    d->freshSize();
    //qDebug()<<"resizeEvent"<<__LINE__;
	QScrollArea::resizeEvent(event);
}


void BzScrollArea::pageUp()
{
	Q_D(BzScrollArea);
	if (QAbstractAnimation::Running == d->m_active.state())
	{
		d->m_active.stop();
	}

	if (d->m_pTargetWidget)
	{
        if (d->m_scrollWidgetHeight <= d->m_scrollAreaHeight)	//如果滚动窗口比父窗口小，不滚动则直接返回，不做处理
		{
			return;
		}
		else
		{
			int posY = d->m_pTargetWidget->pos().y();	//当前Y坐标
			int dist = 0 - posY;	    //位置差
            if (dist > d->m_scrollAreaHeight)	//上限位置差比一页大，则翻一页。反则直接翻到上限位置
			{
                d->m_pTargetWidget->move(0, posY + d->m_scrollAreaHeight);
			}
			else
			{
				d->m_pTargetWidget->move(0, 0);
			}
		}
	}
	else
	{
		return;	//滚动目标窗口或者目标窗口的父窗口为空，返回
	}
}


void BzScrollArea::pageDown()
{
	Q_D(BzScrollArea);
	if (QAbstractAnimation::Running == d->m_active.state())
	{
		d->m_active.stop();
	}
	if (d->m_pTargetWidget)
	{
        if (d->m_scrollWidgetHeight <= d->m_scrollAreaHeight)	//如果滚动窗口比父窗口小，不滚动则直接返回，不做处理
		{
			return;
		}
		else
		{
			int posY = d->m_pTargetWidget->pos().y();	//当前Y坐标
			int dist = posY - d->m_upMaxOffset;	    //位置差
            if (dist > d->m_scrollAreaHeight)
			{
                d->m_pTargetWidget->move(0, posY - d->m_scrollAreaHeight);
			}
			else
			{
				d->m_pTargetWidget->move(0, d->m_upMaxOffset);
			}
		}
	}
	else
	{
		return;	//滚动目标窗口或者目标窗口的父窗口为空，返回
	}
}


void BzScrollArea::ScrollDownOrUp(int posY)
{
	Q_D(BzScrollArea);
	if (d->m_pTargetWidget)
	{
        if (d->m_scrollWidgetHeight <= d->m_scrollAreaHeight)	//如果滚动窗口比父窗口小，不滚动则直接返回，不做处理
		{
			return;
		}
		else
		{
			d->m_pTargetWidget->move(0, 0-posY);
		}
	}
	else
	{
		return;	//滚动目标窗口或者目标窗口的父窗口为空，返回
	}
	
}
