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
    void setTip(QString s);                       //设置提示

    QString text = nullptr;           //按钮提示
    QString imgPath = nullptr;        //按钮图标1的路径
    QString imgPath2 = nullptr;       //按钮图标2的路径（只有最大化按钮会用到）
    QString highLightColor = nullptr; //按钮高亮颜色（HEX格式）
    int w, h;                         //按钮宽、高
};

#endif // MYBUTTON_H
