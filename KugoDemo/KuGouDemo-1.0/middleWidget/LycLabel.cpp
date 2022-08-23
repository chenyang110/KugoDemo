#include "LycLabel.h"
#include <QScroller>
#include <QEvent>
#include <QScrollPrepareEvent>
#include <QPainter>
#include <QFontDialog>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QColorDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <windows.h>
#include"Global_ValueGather.h"
#include"deskTopLrcWidget.h"
#include"basewindow.h"
#include"middleLeftStackWidget0.h"
#include"myTablePlayListFinal.h"
#include"mytablewidget.h"
#include"mainwindow.h"

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

LycLabel::LycLabel(bool touch, QWidget* parent)
	:AbstractWheelWidget(touch, parent)
{
	lyriclabelPointer = this;
	m_realCurrentText = "";

	m_lyric = new Lyric();
	m_lrcFont = QFont("微软雅黑", 14, QFont::Medium);
	m_lrcHightLight = QColor(255, 220, 26);
	connect(this, SIGNAL(changeTo(int)), this, SLOT(changeToEvent(int)));

	m_menu = new QMenu;
	m_menu->setContentsMargins(4, 10, 3, 10);
	QMenu* submenu = new QMenu("字体大小设置", m_menu);
	submenu->setContentsMargins(4, 10, 3, 10);

	QAction* forwardSecond = new QAction("歌词前进0.5秒", m_menu);
	connect(forwardSecond, SIGNAL(triggered(bool)), this, SLOT(setForwardHalfSecond()));
	QAction* backSecond = new QAction("歌词后退0.5秒", m_menu);
	connect(backSecond, SIGNAL(triggered(bool)), this, SLOT(setBackHalfSecond()));

	QAction* forwardtwoSeconds = new QAction("歌词前进2.0秒", m_menu);
	connect(forwardtwoSeconds, SIGNAL(triggered(bool)), this, SLOT(setForwardTwoSeconds()));
	QAction* backtwoSeconds = new QAction("歌词后退2.0秒", m_menu);
	connect(backtwoSeconds, SIGNAL(triggered(bool)), this, SLOT(setBackTwoSeconds()));

	QAction* selectLyric = new QAction("打开本地歌词", m_menu);
	connect(selectLyric, SIGNAL(triggered(bool)), this, SLOT(slot_openLocalLrcFile()));

	QAction* singerBG = new QAction("歌手写真开关", m_menu);
	connect(singerBG, SIGNAL(triggered(bool)), this, SLOT(setSingerBGShowOrNot()));

	QAction* colorHighLight = new QAction("高亮颜色", m_menu);
	connect(colorHighLight, SIGNAL(triggered(bool)), this, SLOT(changeHightLightColor()));
	///// submenu
	QActionGroup* actgroup = new QActionGroup(submenu);
	QAction* fontbig = new QAction("大", submenu);
	connect(fontbig, SIGNAL(triggered(bool)), this, SLOT(changeFont()));
	QAction* fontmedium = new QAction("中", submenu);
	connect(fontmedium, SIGNAL(triggered(bool)), this, SLOT(changeFont()));
	QAction* fontsmall = new QAction("小", submenu);
	connect(fontsmall, SIGNAL(triggered(bool)), this, SLOT(changeFont()));

	submenu->addAction(fontbig);
	submenu->addAction(fontmedium);
	submenu->addAction(fontsmall);

	actgroup->addAction(fontbig);
	actgroup->addAction(fontmedium);
	actgroup->addAction(fontsmall);
	fontbig->setCheckable(true);
	fontmedium->setCheckable(true);
	fontsmall->setCheckable(true);

	fontbig->setChecked(true);
	////

	m_menu->addAction(forwardSecond);
	m_menu->addAction(backSecond);

	m_menu->addAction(forwardtwoSeconds);
	m_menu->addAction(backtwoSeconds);

	m_menu->addAction(selectLyric);
	m_menu->addSeparator();
	m_menu->addMenu(submenu);
	m_menu->addAction(singerBG);
	m_menu->addAction(colorHighLight);
}

void LycLabel::setCurLrcChangeTime(int time, bool isadding)
{
	int time2 = time;
	if (!isadding)
		time2 = (-time2);
	if (m_lyric->m_lrcmap.isEmpty())
		return;
	QMap<qint64, QString> lrcmap;
	foreach(qint64 key, m_lyric->m_lrcmap.keys()) {
		lrcmap.insert(key + time2, m_lyric->m_lrcmap.value(key));
	}
	QMap<qint64, QMap<qint64, qint64>> intervalmap;
	foreach(qint64 key, m_lyric->m_lrcmap.keys()) {
		intervalmap.insert(key + time2, m_lyric->m_lrcintervalmap.value(key));
	}

	m_lyric->m_lrcmap = lrcmap;
	m_lyric->m_lrcintervalmap = intervalmap;
	// m_lyric->changeLrcFileTime(time,isadding);

}

void LycLabel::setForwardHalfSecond()
{
	setCurLrcChangeTime(500, false);
}

void LycLabel::setBackHalfSecond()
{
	setCurLrcChangeTime(500, true);
}

void LycLabel::setForwardTwoSeconds()
{
	setCurLrcChangeTime(2000, false);
}

void LycLabel::setBackTwoSeconds()
{
	setCurLrcChangeTime(2000, true);
}

void LycLabel::setSingerBGShowOrNot()
{
	if (!backgroundPointer)
		return;
	if (backgroundPointer->isShowSingerBG())
		backgroundPointer->setShowSingerBG(false);
	else
		backgroundPointer->setShowSingerBG(true);
	backgroundPointer->update();
}

void LycLabel::slot_replyLrc(const QByteArray& byt, const QString& songname)
{
	QString playsongname = m_mainwindow->middleStack0()->nowPlayFinalTable()->currentSongName();
	if (songname == playsongname.replace("&", " "))
	{
		analyzeLrc(byt, QString("D://ExcellentLrc/") + songname + ".lrc");
	}
}

void LycLabel::analyzeLrc(const QByteArray& lrccontent, const QString filePath)
{
	m_lyric->analyzeLrcContent(lrccontent, filePath);
	this->update();
}

void LycLabel::paintItem(QPainter* painter, int index, QRect& rect)
{
	int ih = itemHeight() * 1.2 / 10;
	int ch = m_itemOffset * 1.2 / 10; //change Hight
	painter->setFont(m_lrcFont);

	if (index == m_currentItem)//current lyric
	{
		if (m_itemOffset == 0)//滚动到这里停止 画就在这里画
		{
			QFont font(m_lrcFont);
			font.setPointSize(font.pointSize() + ih);
			painter->setFont(font);

			m_currentRollrect = rect;
			m_currentMaskFont = painter->font();
		}
		else //当前行滚动到上一行变成了上一行
		{
			QFont font(m_lrcFont);
			font.setPointSize(font.pointSize() + ih - ch);
			painter->setFont(font);
		}
	}
	if (index == m_currentItem + 1)//next lyric will gradually become  more and more bigger
	{
		QFont font(m_lrcFont);
		font.setPointSize(font.pointSize() + ch);
		painter->setFont(font);
	}

	QFontMetrics metrics(painter->font());
	QString text = m_lyric->getLineAt(index);

	painter->drawText((rect.width() - metrics.width(text)) / 2,
		rect.y() + (rect.height() - metrics.height()) / 2,
		metrics.width(text),
		rect.height(),
		Qt::AlignLeft, text);
}

void LycLabel::paintItemMask(QPainter* painter)
{
	if (m_itemOffset == 0)
	{
		painter->setFont(m_currentMaskFont);
		painter->setPen(m_lrcHightLight);
		QFontMetrics metrics(m_currentMaskFont);
		painter->drawText((m_currentRollrect.width() - metrics.width(m_realCurrentText)) / 2,
			m_currentRollrect.y() + (m_currentRollrect.height() - metrics.height()) / 2,
			m_maskLength,
			m_currentRollrect.height(),
			Qt::AlignLeft, m_realCurrentText);
	}
}

void LycLabel::clearLrc()
{
	m_lyric->m_lrcmap.clear();
	m_lyric->m_lrcintervalmap.clear();
	m_lyric->m_lrcwordsmap.clear();
	m_lyric->m_filedir = "";
	m_maskLength = -1000;
	m_currentRollrect = QRect(0, 0, 0, 0);
	m_realCurrentText = "";
	update();
}

int LycLabel::itemHeight() const
{
	QFontMetrics fm(m_lrcFont);
	return fm.height() * 2;
}

int LycLabel::itemCount() const
{
	return m_lyric->getCount();
}

void LycLabel::slot_timerWork()
{
	if (m_itemOffset != 0)
		return;

	int interval = 0;
	float precent = 0;
	QString str = " ";
	m_lyric->getItemPrecent(m_pos, interval, precent, str);

	QFontMetrics metrics(m_currentMaskFont);
	if (m_itemPrecent == precent)//&&m_lyric->getIndex(time)==m_currentItem
	{
		qreal count = interval / 25;
		float lrcMaskMiniStep = metrics.width(str) / count;
		m_maskLength += lrcMaskMiniStep;
	}
	else
		m_maskLength = metrics.width(m_realCurrentText) * precent;
	update();
	m_itemPrecent = precent;
	emit sig_currentPrecentChange(str, precent, interval);
}

void LycLabel::slot_openLocalLrcFile()
{
	QString filePath = m_lyric->m_filedir;//"D://ExcellentLrc/周杰伦-彩虹.lrc"       we have to change to this url format    /select,F:\MP3s764????\MUSIC\?????-????.mp3
	QString fileFull = filePath;
	fileFull.replace("//", QString(char(92))).replace('/', char(92));
	QString head = "/select,";
	QString selectfile = head + fileFull;
	LPCSTR strr = selectfile.toLocal8Bit().constData();
	ShellExecuteA(0, "open", "explorer.exe", strr, NULL, true);
}

void LycLabel::positionChanged(qint64 length)
{
	length = length / 1000 + SCROLL_TIME;
	m_pos = length;
	if (m_isScrolled) //if we are scrolling the lyric content just return
		return;
	int index = m_lyric->getIndex(length); //using binary search for the current lyric index;
	if (index != m_currentItem && m_itemOffset == 0)
	{
		m_maskLength = -1000;
		m_currentRollrect = QRect(0, 0, 0, 0);
		m_realCurrentText = m_lyric->getLineAt(index);
		m_itemOffset = -1; // have to set this value ,cause when update event was done , the last lyric mask will leave on window!!!

		emit sig_currentLrcChange(m_lyric->getLineAt(index), m_lyric->getLineAt(index + 1));
		this->scrollTo(index);
	}
	slot_timerWork();
}

void LycLabel::setPostion(qint64 pos)
{
	int index = m_lyric->getIndex(pos);
	this->setCurrentIndex(index);
}

void LycLabel::changeToEvent(int index)
{
	emit changeToPosition(m_lyric->getPostion(index) * 1000);

}

void LycLabel::changeFont()
{
	QAction* act = (QAction*)sender();
	if (act->text() == "大")
	{
		m_lrcFont = QFont("微软雅黑", 14, QFont::Medium);
	}
	if (act->text() == "中")
	{
		m_lrcFont = QFont("微软雅黑", 12, QFont::Medium);
	}
	if (act->text() == "小")
	{
		m_lrcFont = QFont("微软雅黑", 10, QFont::Medium);
	}
}


void LycLabel::changeHightLightColor()
{
	QColor color = QColorDialog::getColor(m_lrcHightLight, NULL);
	m_lrcHightLight = (color.isValid()) ? color : Qt::yellow;
}

void LycLabel::contextMenuEvent(QContextMenuEvent*)
{

	emit rightClicked();

	m_menu->exec(QCursor::pos());
}

void LycLabel::enterEvent(QEvent* e)
{
	QWidget::enterEvent(e);
	emit mouseEnter();
}
////////////////////////////////////////////////////////////////////////////////////////