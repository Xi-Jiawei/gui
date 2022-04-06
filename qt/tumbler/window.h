#ifndef WINDOW_H
#define WINDOW_H

#include <QCoreApplication>
#include <QApplication>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QEvent>
#include <QPixmap>
#include <QFont>
#include <QFontMetrics>
#include <QPushButton>
#include <QGesture>
#include <QMenu>

#include "tumbler.h"

#define TOUCHSCREEN_SUPPORT 1

class Window : public QWidget {
	Q_OBJECT
public:
	Window();
	//~Window();
protected:
	//bool eventFilter(QObject *, QEvent *);
	bool event(QEvent *);
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
private:
private slots:
};

#endif

