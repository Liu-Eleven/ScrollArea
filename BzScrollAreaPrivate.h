/****************************************************************************/
/*      版权所有: Copyright (C), 2010-2020,ShenZhen Bozone IT Co., Ltd.     */
/****************************************************************************/

/****************************************************************************/
/*文件名称：BzScrollAreaPrivate.h                                           */
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


#ifndef _BOZONE_SCROLLAREA_PRIVATE_H_
#define _BOZONE_SCROLLAREA_PRIVATE_H_

#include <QScrollBar>
#include <QLabel>
#include <QPropertyAnimation>

class QWidget;
class QScrollBar;
class BzScrollArea;

class BzScrollAreaPrivate : public QObject
{
	Q_OBJECT

public:
	BzScrollAreaPrivate(QObject * parent = 0);
	~BzScrollAreaPrivate();


signals:

private:
	/*************************************************
	功能	: 	刷新窗口大小调用的函数
	说明	:	刷新滚动目标窗口或者刷新滚动父窗口，必须调用
			这个函数，更新目标窗口向上移动的像素
	输入	：	无
	输出	:	无
	返回	:	无
	***************************************************/
	void  freshSize();   //刷新滚动窗口大小

	/*************************************************
	功能	: 	手触滑动动作
	说明	:	无
	输入	：	两个Y坐标
	输出	:	无
	返回	:	无
	***************************************************/
	void moveAction(const int &dy1, const int &dy2);  //进行手触滑动

	/*************************************************
	功能	: 	启动滚动阻尼动画效果
	说明	:	无
	输入	：	两个Y坐标
	输出	:	无
	返回	:	无
	***************************************************/
	void decelerateAction(const int &dy1, const int &dy2);  //进行减速滚动

private slots:
	/*************************************************
	功能	: 	启动滚动阻尼动画效果
	说明	:	界面滑动和滚动与列滚动条同步
	输入	：	界面位置坐标
	输出	:	无
	返回	:	无
	***************************************************/
	void vScrollBarChangeValue(const QVariant &value);

private:
	int 	m_upMaxOffset;		//往上最大偏移量
	int 	m_downMaxOffset;	//往下最大偏移量
	int     m_PressDy;			//按下去的Y坐标
	int 	m_firstDy;			//倒数第二次鼠标滑动事件Y坐标
	int 	m_lastDy;			//倒数第一次鼠标滑动事件Y坐标

    int     m_scrollWidgetHeight;	//滚动视窗(控件)的高度
    int		m_scrollAreaHeight;	 //滚动区域的高度

	int 	m_scrollSpeed;		//滑动速度
	int     m_scrollTime;		//滚动时间，单位是毫秒

	bool    m_isFirst;			//记录第一次事件
    char 	m_barIsShow;        //滚动框是否显示 0-需要时 1-总不显示 2-总显示

	QWidget *m_pClickedWidget;	//鼠标按下的目标窗口
	QWidget *m_pTargetWidget;   //滚动的目标窗口

	QScrollBar  *m_pVerticalScrollBar;	//列滚动条

	QPropertyAnimation  m_active;	//减速动画效果

protected:
	BzScrollArea* const q_ptr;

private:
	Q_DECLARE_PUBLIC(BzScrollArea);
};

#define DEFULT_TOAST_STYLE "\
QLabel{\
    color:#FFFFFF;\
    font:18px;\
    font-weight:500;\
    background-color:rgb(98,92,82,90);\
    padding:3px;\
    border-radius:2;\
}\
"

/**
 * @brief The NBaseToastr class 自动消失的提示框
 */
class  NBaseToastr : public QLabel
{
    Q_OBJECT
public:
    explicit NBaseToastr(QWidget *parent,
                   const QString &message = QString(),
                   int w = 200,
                   int h = 50,
                   const QString &style = DEFULT_TOAST_STYLE);
    void toastr();

private:
    /**
     * @brief setShadow 设置窗体阴影效果
     * @param w 窗体指针
     * @param blurRadius 阴影清晰度，越小越清晰
     * @param dx x方向阴影位置和偏移量，正值在右方，值越大偏移越大
     * @param dy y方向阴影位置和偏移量，正值在下方，值越大偏移越大
     * @return
     */
    void setShadow(QWidget *w, qreal blurRadius=10, qreal dx=5, qreal dy=5);

private slots:
    void animationFinished();
    void delSelf();

private:
    int startx;
    int starty;
    int endy;
};

#endif
