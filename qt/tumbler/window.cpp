#include "window.h"

Window::Window()
{
    //Tumbler *tumbler = new Tumbler(5, this);
	DigitTumbler *tumbler1 = new DigitTumbler(this);
	DigitTumbler *tumbler2 = new DigitTumbler(this);
	DigitTumbler *tumbler3 = new DigitTumbler(this);
	DigitTumbler *tumbler4 = new DigitTumbler(this);

	QGridLayout *vlayout = new QGridLayout;
    vlayout->setContentsMargins(20, 0, 20, 0);
	vlayout->addWidget(tumbler1, 0, 0);
	vlayout->addWidget(tumbler2, 0, 1);
	vlayout->addWidget(tumbler3, 0, 2);
	vlayout->addWidget(tumbler4, 0, 3);
	this->setLayout(vlayout);

	this->setFixedSize(tumbler1->width() * 4 + 100, tumbler1->height() + 100);
}

/*!\reimp
*/
void Window::paintEvent(QPaintEvent *e)
{
	//fprintf(stdout, "[window] call paintEvent()\n");
	QWidget::paintEvent(e);
}

/*bool Window::eventFilter(QObject *watched, QEvent *event)
{
	fprintf(stdout, "[window] call eventFilter()\n");

	fprintf(stdout, "[window] event->type(): %d\n", event->type());

	return QWidget::eventFilter(watched, event);
}*/

bool Window::event(QEvent *e)
{
	return QWidget::event(e);
}

void Window::keyPressEvent(QKeyEvent *e)
{
	fprintf(stdout, "[window] call keyPressEvent()\n");
	switch(e->key()) {
		case Qt::Key_Escape:
			QCoreApplication::exit(0);
			break;
		case Qt::Key_R:
			fprintf(stdout, "[window] Key_R\n");
			break;
		default:
			break;
	}
}

void Window::mousePressEvent(QMouseEvent *e)
{
	QWidget::mousePressEvent(e);
}

void Window::mouseReleaseEvent(QMouseEvent *e)
{
	QWidget::mouseReleaseEvent(e);
}
