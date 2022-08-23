#include "AbstractWheelWidget.h"

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

AbstractWheelWidget::AbstractWheelWidget(bool touch, QWidget* parent)
    : baseWidget(parent), m_currentItem(0), m_itemOffset(0)
{
    m_maskLength = 0;
    setStyleSheet("baseWidget{background:transparent;}");//rgb(25,125,125)
// ![0]
    QScroller::grabGesture(this, touch ? QScroller::TouchGesture : QScroller::LeftMouseButtonGesture);
    // ![0]
    this->m_isScrolled = false;
    this->m_doSignal = true;
}

AbstractWheelWidget::~AbstractWheelWidget()
{ }

int AbstractWheelWidget::currentIndex() const
{
    return m_currentItem;
}

void AbstractWheelWidget::setCurrentIndex(int index)
{
    if (index >= 0 && index < itemCount()) {
        m_currentItem = index;
        m_itemOffset = 0;
        update();
    }
}

bool AbstractWheelWidget::event(QEvent* e)
{
    switch (e->type())
    {
        // ![1]
    case QEvent::ScrollPrepare:
    {
        QScrollPrepareEvent* se = static_cast<QScrollPrepareEvent*>(e);
        se->setViewportSize(QSizeF(size()));
        // we claim a huge scrolling area and a huge content position and
        // hope that the user doesn't notice that the scroll area is restricted
        se->setContentPosRange(QRectF(0.0, 0.0, 0.0, WHEEL_SCROLL_OFFSET * 2));
        se->setContentPos(QPointF(0.0, WHEEL_SCROLL_OFFSET + m_currentItem * itemHeight() + m_itemOffset)); //相当于这里保存当前位置
        se->accept();

        return true;
    }
    // ![1]
    // ![2]
    case QEvent::Scroll:
    {
        QScrollEvent* se = static_cast<QScrollEvent*>(e);
        qreal y = se->contentPos().y();

        int iy = y - WHEEL_SCROLL_OFFSET;
        int ih = itemHeight();
        // ![2]
        // ![3]

                    // -- calculate the current item position and offset and redraw the widget
        int ic = itemCount();
        if (ic > 0)
        {
            m_currentItem = iy / ih;
            m_itemOffset = iy % ih;

            if (m_currentItem >= ic)
                m_currentItem = ic - 1;
        }



        if (m_doSignal)//只有触发信号的滚动才进行,而且这种是人为滚动
        {
            //开始滚动
            if (se->scrollState() == QScrollEvent::ScrollStarted)
            {
                if (m_currentItem > 1)
                    this->m_isScrolled = true;
            }
        }
        //滚动结束
        if (se->scrollState() == QScrollEvent::ScrollFinished)//
        {
            if (m_doSignal)//users do this
            {
                if (m_currentItem > 1)
                    emit changeTo(this->m_currentItem + 1);
                m_maskLength = 0;
                m_realCurrentText = "";
            }
            this->m_isScrolled = false;
            m_doSignal = true;
            m_itemOffset = 0;
        }
        // -- repaint
        if (m_itemOffset != 0)
            update();
        se->accept();
        return true;
        // ![3]
    }
    case QEvent::MouseButtonPress:
        setFocus();
        return true;
    default:
        return QWidget::event(e);
    }
    return true;
}

void AbstractWheelWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    int w = width();
    int h = height();

    int iH = itemHeight();//the height of each lyric and blank
    int iC = itemCount(); //lyric count
    if (iC > 0)
    {
        for (int i = -h / 2 / iH; i <= h / 2 / iH; i++) // just to print the lyric on the height of the widget
        {
            int itemNum = m_currentItem + i;
            if (itemNum >= 0 && itemNum < iC)
            {
                int len = h / 2 / iH;
                //parabola attenuation ,the value is betweent 220 to 255
                int t = abs(i);
                t = 255 - t * t * 220 / len / len;

                if (t < 0)
                    t = 0;
                painter.setPen(QColor(255, 255, 255, t));
                QRect rect(0, h / 2 + i * iH - m_itemOffset, w, iH);
                paintItem(&painter, itemNum, rect);
            }
        }
    }
    paintItemMask(&painter);
}

/*!
    Rotates the wheel widget to a given index.
    You can also give an index greater than itemCount or less than zero in which
    case the wheel widget will scroll in the given direction and end up with
    (index % itemCount)
*/
void AbstractWheelWidget::scrollTo(int index)
{
    this->m_doSignal = false;//Users do not use mouse to scroll the lyric content
    QScroller* scroller = QScroller::scroller(this);
    scroller->scrollTo(QPointF(0, WHEEL_SCROLL_OFFSET + index * itemHeight()), SCROLL_TIME);

    //We set scrolling during 500ms to next lyric
}
