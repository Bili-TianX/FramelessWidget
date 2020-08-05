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
