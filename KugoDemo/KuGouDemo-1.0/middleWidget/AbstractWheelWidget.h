#pragma once

#include <QObject>
#include <QScroller>
#include <QEvent>
#include <QScrollPrepareEvent>
#include <QPainter>
#include"baseWidget.h"

#define WHEEL_SCROLL_OFFSET 50000  //define a huge number to prevent scroll to the end;
#define SCROLL_TIME 200

class AbstractWheelWidget : public baseWidget
{
	Q_OBJECT
public:
	AbstractWheelWidget(bool touch, QWidget* parent = 0);
	virtual ~AbstractWheelWidget();

	inline  int currentIndex() const;
	void setCurrentIndex(int index);

	virtual void paintItem(QPainter* painter, int index, QRect& rect) = 0;
	virtual void paintItemMask(QPainter* painter) = 0;

	virtual int itemHeight() const = 0;
	virtual int itemCount() const = 0;

public slots:
	void scrollTo(int index);

signals:
	void stopped(int index);
signals:
	void changeTo(int index);

protected:
	virtual void paintEvent(QPaintEvent* e);
	bool event(QEvent*);

	int m_currentItem;
	int m_itemOffset; // 0-itemHeight()
	bool m_isScrolled;
	bool m_doSignal;//滚动是否触发信号,用于区别是否是人为滚动

	QRect m_currentRollrect;
	QFont m_currentMaskFont;
	float m_maskLength;
	QString m_realCurrentText;
};
