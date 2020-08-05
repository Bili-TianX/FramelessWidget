#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include "mybutton.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QLabel>
#include <QDebug>
#include <QLine>
#include <QMouseEvent>
#include <windows.h>
#include <windowsx.h>

class FramelessWidget : public QWidget
{
        Q_OBJECT

public:
        FramelessWidget(QWidget *parent = nullptr,
                        QString title = "未设置标题",
                        QString maximize_imgPath1 = nullptr,
                        QString maximize_imgPath2 = nullptr,
                        QString minimize_imgPath = nullptr,
                        QString close_imgPath = nullptr);
        ~FramelessWidget();
        void paintEvent(QPaintEvent *event) override;

        void setSplitLine(int thickness, QString c);                        //设置分割线样式
        void setBorder(int thickness, QColor c);                            //设置边框样式
        void setBackgroundColor(QColor c);                                  //设置背景颜色
        void setTitle(QString title, QFont myFont = QFont("微软雅黑", 17)); //设置标题

        MyButton *close_btn = new MyButton("关闭", nullptr, nullptr, "#c0c0c0"),     //关闭按钮
            *maximize_btn = new MyButton("最大化", nullptr, nullptr, "#c0c0c0"),     //最大化按钮
                *minimize_btn = new MyButton("最小化", nullptr, nullptr, "#c0c0c0"); //最小化按钮
        QPushButton *split_line = new QPushButton();                                 //分割线

protected:
        void mousePressEvent(QMouseEvent *) override;

        void mouseMoveEvent(QMouseEvent *) override;

        void mouseDoubleClickEvent(QMouseEvent *) override;

        bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
        int boundaryWidth = 10; //鼠标调整窗口大小的范围
        QPoint clickPos;
        QColor background_color = QColor(255, 255, 255); //背景颜色
        QColor border_color = QColor(0, 0, 0);           //边框颜色
        QString split_line_color;
        int border_thickness = 10;    //窗口边框粗细
        int split_line_thickness = 1; //分割线粗细
        QLabel *title = new QLabel(); //标题
};
#endif // FRAMELESSWIDGET_H
