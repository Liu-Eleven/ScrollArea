#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QLabel;
class QPushButton;
class BzScrollArea;
class MListWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
public:

    void initUi();

protected:


private:

signals:

public slots:

    void btnClicked();
    void btnClicked1();

private:

    MListWidget   *m_pMListWidget;
    BzScrollArea	  *m_pScrollArea;
    QPushButton	  *m_pBtn1;
    QPushButton   *m_pBtn2;
    QPushButton	  *m_pBtn3;

    QPushButton	  *m_pBtnTarget50000;
    QPushButton	  *m_pBtnTarget600;

    QLabel		  *m_pLabel1;
    QLabel		  *m_pLabel2;
    QLabel		  *m_pLabel3;

    QLabel		  *m_pLabel50000;
    QLabel		  *m_pLabel600;
};

#endif // WIDGET_H
