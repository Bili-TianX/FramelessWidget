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
        this->setText(this->text);
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
