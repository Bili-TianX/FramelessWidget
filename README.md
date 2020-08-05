[toc]

# 自制无边框窗口

## 〇、示例

无参数版本![image-20200805142435572](README_IMAGES\image-20200805142435572.png)

修改后版本![image-20200805080858860](README_IMAGES\image-20200805080858860.png)

## 一、特点

- 没有边框。
- 可以自定义最大化、最小化、关闭按钮。
- 相比默认的窗口，排版更加整洁
- 可以自定义大部分内容
- 有鼠标拖动窗口、改变大小、双击最大化等功能

## 二、功能

- 更改窗口标题样式（内容、字体）：`FramelessWidget::setTitle`![image-20200805081129147](README_IMAGES\image-20200805081129147.png)

- 更改窗口背景颜色：`FramelessWidget::setBackgroundColor`![image-20200805081515724](README_IMAGES\image-20200805081515724.png)

- 更改边框样式（颜色、粗细）：`FramelessWidget::setBorder`![image-20200805082750178](README_IMAGES\image-20200805082750178.png)

- 更改分割线样式（颜色、粗细）：`FramelessWidget::setSplitLine`![image-20200805082819834](README_IMAGES\image-20200805082819834.png)

- 更改最大化、最小化、关闭按钮样式（大小、图标、高亮颜色（即鼠标放上去时会变成什么颜色）、提示）：`void setSize(int w, int h);`

	`void setImg(QString imgPath);`

	`void setHighLightColor(QString c);`

	`void setTip(QString s);`

	![image-20200805084855608](README_IMAGES\image-20200805084855608.png)

## 三、源码

`FramelessWidget.cpp`

```c++
#include "FramelessWidget.h"

FramelessWidget::FramelessWidget(QWidget *parent, QString text,
                                 QString maximize_imgPath1,
                                 QString maximize_imgPath2,
                                 QString minimize_imgPath,
                                 QString close_imgPath)
    : QWidget(parent)
{
    //设置按钮图标
    maximize_btn->imgPath = maximize_imgPath1;
    maximize_btn->imgPath2 = maximize_imgPath2;
    minimize_btn->imgPath = minimize_imgPath;
    close_btn->imgPath = close_imgPath;
    maximize_btn->setImg(maximize_btn->imgPath);
    minimize_btn->setImg(minimize_btn->imgPath);
    close_btn->setImg(close_btn->imgPath);

    this->setWindowFlags(Qt::FramelessWindowHint); //设置窗口样式（无边框）
    this->resize(1280, 720);                       //设置大小
    this->setTitle(text);                          //设置标题

    split_line->resize(3000, 2);   //设置分割线大小
    split_line->setFixedHeight(2); //设置分割线粗细
    split_line->setFocusPolicy(Qt::NoFocus);
    split_line->setStyleSheet("QPushButton{background-color:#000000;}"); //设置分割线颜色（默认黑色）
                                                                         //------------------------------------------------------------------设置布局
    QHBoxLayout *h1 = new QHBoxLayout();
    QVBoxLayout *v1 = new QVBoxLayout();
    v1->setSpacing(5);
    h1->setSpacing(5);
    h1->addWidget(this->title);
    h1->addStretch();
    h1->addWidget(this->minimize_btn);
    h1->addWidget(this->maximize_btn);
    h1->addWidget(this->close_btn);
    v1->addLayout(h1);
    v1->addWidget(this->split_line);
    v1->addStretch();
    this->setLayout(v1);
    //------------------------------------------------------------------设置三个按钮的功能
    connect(close_btn, &MyButton::clicked, this, &FramelessWidget::close);
    connect(minimize_btn, &MyButton::clicked, this, &FramelessWidget::showMinimized);
    connect(maximize_btn, &MyButton::clicked, this, [=]() {
        if (FramelessWidget::isMaximized())
        {
            maximize_btn->setImg(maximize_btn->imgPath);
            FramelessWidget::showNormal();
        }
        else
        {
            maximize_btn->setImg(maximize_btn->imgPath2);
            FramelessWidget::showMaximized();
        }
    });
}

FramelessWidget::~FramelessWidget()
{
}

void FramelessWidget::setTitle(QString title, QFont myFont)
{ //设置标题
    this->title->setText(title);
    this->title->setFont(myFont);
}

void FramelessWidget::setBorder(int thickness, QColor c)
{ //设置边框
    this->border_thickness = thickness;
    this->border_color = c;
}

void FramelessWidget::setSplitLine(int thickness, QString c)
{ //设置分割线
    this->split_line_thickness = thickness;
    this->split_line_color = c;
    this->split_line->setFixedHeight(this->split_line_thickness); //设置粗细
    QString style = "QPushButton{background-color:" + this->split_line_color + ";border:0px}";
    this->split_line->setStyleSheet(style); //设置颜色
}

void FramelessWidget::setBackgroundColor(QColor c)
{ //设置背景颜色
    this->background_color = c;
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, this->background_color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void FramelessWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPen pen;
    pen.setColor(this->border_color);
    pen.setWidth(this->border_thickness);
    p.setPen(pen);
    p.drawRect(0, 0, FramelessWidget::width(), FramelessWidget::height());
}
void FramelessWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (this->isMaximized())
        return;
    if (e->buttons() & Qt::LeftButton)
        move(e->pos() + pos() - clickPos);
}
void FramelessWidget::mousePressEvent(QMouseEvent *e)
{
    if (this->isMaximized())
        return;
    if (e->button() == Qt::LeftButton)
        clickPos = e->pos();
}
void FramelessWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (FramelessWidget::isMaximized())
    {
        maximize_btn->setImg(maximize_btn->imgPath);
        FramelessWidget::showNormal();
    }
    else
    {
        maximize_btn->setImg(maximize_btn->imgPath2);
        FramelessWidget::showMaximized();
    }
}
bool FramelessWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = (MSG *)message;
    switch (msg->message)
    {
    case WM_NCHITTEST:
        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if (xPos < boundaryWidth && yPos < boundaryWidth) //左上角
            *result = HTTOPLEFT;
        else if (xPos >= width() - boundaryWidth && yPos < boundaryWidth) //右上角
            *result = HTTOPRIGHT;
        else if (xPos < boundaryWidth && yPos >= height() - boundaryWidth) //左下角
            *result = HTBOTTOMLEFT;
        else if (xPos >= width() - boundaryWidth && yPos >= height() - boundaryWidth) //右下角
            *result = HTBOTTOMRIGHT;
        else if (xPos < boundaryWidth) //左边
            *result = HTLEFT;
        else if (xPos >= width() - boundaryWidth) //右边
            *result = HTRIGHT;
        else if (yPos < boundaryWidth) //上边
            *result = HTTOP;
        else if (yPos >= height() - boundaryWidth) //下边
            *result = HTBOTTOM;
        else //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;
        return true;
    }
    return false; //此处返回false，留给其他事件处理器处理
}
```

`FramelessWidget.h`

```c++
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
```

`mybutton.cpp`

```c++
#include "mybutton.h"

MyButton::MyButton(QString text,     //按钮提示
                   QString imgPath,  //按钮图标1的路径
                   QString imgPath2, //按钮图标2的路径（只有最大化按钮会用到）
                   QString c)        //按钮高亮颜色（HEX格式）
{
    this->text = text;
    this->imgPath = imgPath;
    this->imgPath2 = imgPath2;
    this->highLightColor = c;
    setTip(this->text);   //设置按钮提示
    setImg(imgPath);      //设置按钮图标
    setHighLightColor(c); //设置高亮颜色（HEX格式）
}
void MyButton::setSize(int w, int h) //设置按钮大小
{
    this->w = w;
    this->h = h;
    this->setFixedSize(this->w, this->h);
}
void MyButton::setImg(QString imgPath) //设置按钮图标
{
    QPixmap pix;
    if (!pix.load(imgPath))
    { //图片加载失败
        qDebug() << "图片加载失败";
        return;
    }
    this->setSize(pix.width(), pix.height());            //设置按钮大小
    this->setIcon(pix);                                  //设置图标
    this->setIconSize(QSize(pix.width(), pix.height())); //设置图标大小
}
void MyButton::setHighLightColor(QString c) //设置高亮颜色（HEX格式）
{
    this->highLightColor = c;
    QString style = "QPushButton{border:0px;}QPushButton::hover{background-color:" + this->highLightColor + ";}";
    this->setStyleSheet(style);
}
void MyButton::setTip(QString s) //设置按钮提示
{
    this->text = s;
    this->setToolTip(this->text);
}
```

`mybutton.h`

```c++
#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QDebug>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QString text = nullptr,     //按钮提示
                      QString imgPath = nullptr,  //按钮图标1的路径
                      QString imgPath2 = nullptr, //按钮图标2的路径（只有最大化按钮会用到）
                      QString c = nullptr);       //按钮高亮颜色（HEX格式）
    void setSize(int w, int h);                   //设置按钮大小（固定）
    void setImg(QString imgPath);                 //设置图标
    void setHighLightColor(QString c);            //设置高亮颜色（HEX格式）
    void setTip(QString s);                       //设置按钮提示

    QString text = nullptr;           //按钮提示
    QString imgPath = nullptr;        //按钮图标1的路径
    QString imgPath2 = nullptr;       //按钮图标2的路径（只有最大化按钮会用到）
    QString highLightColor = nullptr; //按钮高亮颜色（HEX格式）
    int w, h;                         //按钮宽、高
};

#endif // MYBUTTON_H
```

## 四、函数说明

- `void FramelessWidget::setSplitLine(int thickness, QString c)`设置分割线样式

	int thickness：分割线粗细

	QString c：分割线颜色（HEX格式，如“#C0C0C0”）

- `void FramelessWidget::setBorder(int thickness, QColor c)`设置边框样式

	int thickness：边框粗细

	QColor c：边框颜色（RGB格式，如QColor（100,200,152））

- `void FramelessWidget::setBackgroundColor(QColor c)`设置背景颜色

	QColor c：背景颜色（RGB格式，如QColor（100,200,152））

- `void FramelessWidget::setTitle(QString title, QFont myFont)`设置标题

	QString title：标题文字

	QFont myFont：字体样式（如QFont(“微软雅黑”，17)）

- `void MyButton::setTip(QString s)`设置按钮提示

	QString s：按钮提示显示的内容

- `void MyButton::setHighLightColor(QString c)`设置高亮颜色（HEX格式）

	QString c：高亮的颜色（HEX格式，如“#C0C0C0”）

- `void MyButton::setImg(QString imgPath)`设置图标

	QString imgPath：图标的路径

- `void MyButton::setSize(int w, int h)`设置按钮大小（固定）

	int w,h：按钮的宽，高

## 五、此类的使用方法（示例）

```c++
#include "FramelessWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FramelessWidget w(nullptr,                    //无父亲
                      "示例",                     //标题
                      ":/new/prefix1/max_1.png",  //最大化图标（单正方形）
                      ":/new/prefix1/max_2.png",  //最大化图标2（双正方形）
                      ":/new/prefix1/min.png",    //最小化图标
                      ":/new/prefix1/close.png"); //关闭图标
    w.setBorder(9, QColor(50, 255, 0));
    w.setSplitLine(10, "#FF00FF");
    w.setBackgroundColor(QColor(100, 100, 255));
    w.close_btn->setHighLightColor("#FF0000");
    w.close_btn->setTip("有本事你点我啊！");
    w.show();
    return a.exec();
}
```

![image-20200805143740498](README_IMAGES\image-20200805143740498.png)

## 六、其他

- 使用时需在项目文件夹下同时存在4个文件并保证main.cpp中已包含
- 如有BUG或不知道怎么用，欢迎来B站搜[天下wu双](https://space.bilibili.com/378857076)并私信我，或加QQ：2941636868找我
- 若出现三个按钮图标加载不出来的情况，请尝试更改图标路径并确保已设置按钮的图标路径不为空
- 可以自己随便更改4个文件中的内容并自己个人使用，但**未经同意**禁止抄袭、复制源代码并用于商业用途
- 我的Qt版本：`5.14.2`；编译器：`MinGW73_64`