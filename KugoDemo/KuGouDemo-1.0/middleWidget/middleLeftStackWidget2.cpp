#include "middleLeftStackWidget2.h"
#include"qlabel.h"

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

middleLeftStackWidget2::middleLeftStackWidget2(QWidget*parent):baseWidget(parent)
{
      QVBoxLayout *vLyout=new QVBoxLayout;
      m_Wid=new buttonStackedWidget(this);
      m_Wid->addPushButton("音乐电台");
      m_Wid->addPushButton("MV电台");
      m_Wid->showLayout();
      m_Wid->m_stackwid->addWidget(new QWidget());
      m_Wid->m_stackwid->addWidget(new QWidget());


      vLyout->addWidget(m_Wid);
      vLyout->setContentsMargins(0,0,0,0);
      setLayout(vLyout);



}
