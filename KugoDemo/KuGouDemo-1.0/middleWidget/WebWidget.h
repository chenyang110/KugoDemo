//修改
#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QObject>
#include <QWidget>
#include"baseWidget.h"
#include<QWebEngineView>
#include<QWebEngineSettings>
class WebWidget : public baseWidget
{
public:
   explicit WebWidget(QWidget*p=0);
   QWebEngineView*m_web;
};

#endif // WEBWIDGET_H

