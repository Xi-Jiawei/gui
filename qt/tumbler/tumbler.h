#ifndef TUMBLER_H
#define TUMBLER_H

#include <QCoreApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>

#define RANGE(num, min, max) num < min ? min : (num > max ? max : num)

class Tumbler : public QWidget {
	Q_OBJECT
	Q_PROPERTY(int offset READ getOffset WRITE setOffset)
public:
	Tumbler(QWidget *parent = 0);
	Tumbler(int cnt = 5, QWidget *parent = 0);
	//~Tumbler();
	int getOffset() { return this->offsetY; }
	void setOffset(int offset) { this->offsetY = offset; repaint(); }
signals:
	void changed(int);
protected:
	bool event(QEvent *);
	bool filterEvent(QObject *, QEvent *);
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void homing();
	int isVertical;
	int isDragging; // is mouse/finger pressed?
	int cnt;
	int semiCnt;
	int listCount;
	int index;
	int posX, posY;
	int scalePosX, scalePosY; // 量化位置
	int offsetX, offsetY;
	int speedX, speedY;
	int accelX, accelY;
	int w, h;
	int iw, ih;
	int threshold;
	bool liveUpdate;
private:
	void paintItem(QPainter &painter, int val, int off);
	QPropertyAnimation *animation;
	QTimer *emitTimer;
	int fontSize;
	float factor;
private slots:
	void finished();
	void emitTimeout();
};

class DigitTumbler : public Tumbler {
	Q_OBJECT
	Q_PROPERTY(int offset READ getOffset WRITE setOffset)
public:
	DigitTumbler(QWidget *parent = 0);
	DigitTumbler(int cnt = 1, QWidget *parent = 0);
	//~Tumbler();
	int getOffset() { return this->offsetY; }
	void setOffset(int offset) { this->offsetY = offset; repaint(); }
protected:
	bool event(QEvent *);
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
private:
	void paintItem(QPainter &painter, int val, int off);
	QPropertyAnimation *animation;
	QTimer *emitTimer;
	int fontSize;
	float factor;
private slots:
	void finished();
	void emitTimeout();
};

#endif
