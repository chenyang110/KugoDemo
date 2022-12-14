#include "myMediaList.h"
#include<QTime>
#include<tableWidgetFiles/mytablewidget.h>

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

myMediaList::myMediaList(QObject *parent) : QObject(parent)
{
    musicIndex=0;
    m_list.empty();
    setPlayMode(PlayMode::playInOrder);
}

QUrl myMediaList::mediaUrl(int index)
{
    if(m_list.isEmpty())
    return QUrl("");
     musicIndex=index;
    return m_list.at(index);
}
void myMediaList::setPlayMode(PlayMode p)
{
    if(p==PlayMode::playInOrder)
         indexMode=0;
    if(p==PlayMode::playRandom)
        indexMode=1;
    if(p==PlayMode::playOneCircle)
        indexMode=2;
}
int myMediaList::nextMediaIndex()//下一曲
{
    switch (indexMode) {
    case 0://playInOrder
      if(musicIndex+1>=m_list.count())//在最后一行了的话
          musicIndex=0;
      else
          musicIndex++;
    break;

    case 1://playRandom
        QTime time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int xxx=qrand()%m_list.count();
        musicIndex=xxx;
    break;
    }
    return musicIndex;
}

int myMediaList::preMediaIndex()//上一曲
{
    switch (indexMode)
    {
        case 0://playInOrder
          if(musicIndex==0)//在第一行了的话
              musicIndex=0;
          else
              musicIndex--;
            break;

        case 1://playRandom
            QTime time= QTime::currentTime();
            qsrand(time.msec()+time.second()*1000);
            int xxx=qrand()%m_list.count();
            musicIndex=xxx;
            break;
    }
    return musicIndex;
}
void myMediaList::slot_removeSong(int index)
{
    m_list.removeAt(index);
    myTableWidget*t=parent()->findChild<myTableWidget*>();
    int PlayWidindex=t->currentSongIndex();
    if(PlayWidindex>=index)
    {
        musicIndex--;
    }

}
