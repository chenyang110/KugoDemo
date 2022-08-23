#ifndef LYCLABEL_H
#define LYCLABEL_H

#include <QWidget>
#include <QMenu>
#include <QDebug>
#include <QThread>
#include <Qtimer>
#include <QFont>

#include"mynetwork.h"
#include"lyric.h"
#include"baseWidget.h"
#include "lyric.h"
#include "AbstractWheelWidget.h"

class mainWindow;

class LycLabel : public AbstractWheelWidget
{
	Q_OBJECT
public:
	LycLabel(bool touch, QWidget* parent = 0);
	~LycLabel() {}
	void setCurLrcChangeTime(int time, bool isadding = true); //改变歌词当前的时间;
	void setInitMainWindow(mainWindow* p) { m_mainwindow = p; }
	void analyzeLrc(const QByteArray&, const QString filepath = NULL);
	virtual void paintItem(QPainter* painter, int index, QRect& rect);
	virtual void paintItemMask(QPainter* painter);

	inline const QString currentLrc() { return  m_lyric->getLineAt(m_currentItem); }
	inline const QString nextLrc() { return  m_lyric->getLineAt(m_currentItem + 1); }

	void clearLrc();
	int itemHeight() const;
	int itemCount() const;

	Lyric* m_lyric;
signals:
	void changeToPosition(qint64 pos);
signals:
	void rightClicked();
signals:
	void mouseEnter();
signals:
	void sig_currentLrcChange(const QString&, const QString&); //this signal is for desktop lyric
signals:
	void sig_currentPrecentChange(const QString&, float& precent, int& interval);

public slots:
	void slot_timerWork();
	void slot_openLocalLrcFile();
	void setForwardHalfSecond();
	void setBackHalfSecond();
	void setForwardTwoSeconds();
	void setBackTwoSeconds();
	void setSingerBGShowOrNot();
	void positionChanged(qint64 length);
	void setPostion(qint64 pos);
	void slot_replyLrc(const QByteArray& byt, const QString& songname);
protected slots:
	void changeToEvent(int index);
	void changeFont();
	void changeHightLightColor();

protected:
	void contextMenuEvent(QContextMenuEvent* event);//右击事件
	void enterEvent(QEvent* e);
	float m_itemPrecent;
	int m_pos;
	QMenu* m_menu;
	QFont m_lrcFont;
	QColor m_lrcHightLight;
private:
	mainWindow* m_mainwindow;
};

#endif // LYCLABEL_H