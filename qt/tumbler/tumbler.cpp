#include "tumbler.h"

Tumbler::Tumbler(QWidget *parent)
    : QWidget(parent)
{
	this->animation = new QPropertyAnimation(this, "offset");
	this->animation->setDuration(300);
	this->animation->setEasingCurve(QEasingCurve::OutQuad);

	this->semiCnt    = cnt / 2;
	this->listCount  = 100;
	this->index      = 61;
	this->isDragging = 0;
	this->threshold  = 20;
	this->scalePosY  = 0;
	this->offsetY    = 0;
	this->fontSize   = 30; // font pixel size
	this->iw         = 200;
	this->ih         = fontSize + 20;
	this->w          = iw;
	this->h          = (cnt + 2) * ih;
	this->threshold  = ih / 2;
	this->factor     = 1;
	this->liveUpdate = false;

	this->setFixedSize(this->w, this->h);
	
	this->emitTimer = new QTimer(this);
	this->emitTimer->setSingleShot(true);
	connect(emitTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
}

Tumbler::Tumbler(int cnt, QWidget *parent)
    : QWidget(parent),
    cnt(cnt)
{
	this->animation = new QPropertyAnimation(this, "offset");
	this->animation->setDuration(300);
	this->animation->setEasingCurve(QEasingCurve::OutQuad);

	connect(animation, SIGNAL(finished()), this, SLOT(finished()));

	this->semiCnt    = cnt / 2;
	this->listCount  = 100;
	this->index      = 61;
	this->isDragging = 0;
	this->threshold  = 20;
	this->scalePosY  = 0;
	this->offsetY    = 0;
	this->fontSize   = 30; // font pixel size
	this->iw         = 200;
	this->ih         = fontSize + 20;
	this->w          = iw;
	this->h          = (cnt + 2) * ih;
	this->threshold  = ih / 2;
	this->factor     = 1;
	this->liveUpdate = false;

	this->setFixedSize(this->w, this->h);

	this->installEventFilter(this);
	
	this->emitTimer = new QTimer(this);
	this->emitTimer->setSingleShot(true);
	connect(emitTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
}

bool Tumbler::event(QEvent *e) {
	if (e->type() == QEvent::KeyPress)
    {
        fprintf(stdout, "[tumbler] KeyPress\n");
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_R) {
            fprintf(stdout, "[tumbler] Key_R\n");
            keyEvent->ignore();
            return true;
        }
    }
	return QWidget::event(e);
}

bool Tumbler::filterEvent(QObject *, QEvent *e) {
    if (e->type() == QEvent::KeyPress)
    {
        fprintf(stdout, "[tumbler] filterEvent: KeyPress\n");
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_R) {
            fprintf(stdout, "[tumbler] filterEvent: Key_R\n");
            //keyEvent->ignore();
            return true;
        }
    }
    return QWidget::event(e);
}

void Tumbler::keyPressEvent(QKeyEvent *e) {
	fprintf(stdout, "[tumbler] call keyPressEvent()\n");
    switch(e->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_1:
	case Qt::Key_2:
	case Qt::Key_3:
	case Qt::Key_4:
	case Qt::Key_5:
	case Qt::Key_6:
	case Qt::Key_7:
	case Qt::Key_8:
	case Qt::Key_9:
	case Qt::Key_0:
	case Qt::Key_A:
	case Qt::Key_C:
	case Qt::Key_D:
	case Qt::Key_E:
	case Qt::Key_F:
	case Qt::Key_I:
	case Qt::Key_M:
	case Qt::Key_N:
	case Qt::Key_Q:
	case Qt::Key_R:
	case Qt::Key_S:
	case Qt::Key_W:
        if (emitTimer->isActive()) emit changed(index);
        QWidget::keyPressEvent(e);
		break;
	case Qt::Key_Up:
	case Qt::Key_Left:
        fprintf(stdout, "[tumbler] Key_Left\n");
        if (index > 0) {
        	offsetY = ih;
        	//animation->stop();
        	//homing();
        	repaint();
        	if(!liveUpdate) emitTimer->start(500);
        }
        break;
	case Qt::Key_Down:
	case Qt::Key_Right:
        fprintf(stdout, "[tumbler] Key_Right\n");
        if (index < listCount - 1) {
        	offsetY = -ih;
        	//animation->stop();
        	//homing();
        	repaint();
        	if(!liveUpdate) emitTimer->start(500);
        }
        break;
	default:
        QWidget::keyPressEvent(e);
        break;
    }
}

void Tumbler::mousePressEvent(QMouseEvent *e) {
	animation->stop();
	isDragging = 1;
	scalePosY = e->pos().y();
}

void Tumbler::mouseMoveEvent(QMouseEvent *e) {
	if (isDragging) {
		posY = e->pos().y();

		if ((index == 0 && posY > scalePosY) || (index == listCount - 1 && posY < scalePosY))
			return;

		offsetY = posY - scalePosY;
		repaint();
	}
}

void Tumbler::mouseReleaseEvent(QMouseEvent *) {
	fprintf(stdout, "[tumbler] " "\033[32m" "call mouseReleaseEvent()" "\033[0m" ", index = %d, offsetY = %d\n", index, offsetY);

	if (isDragging) {
		isDragging = 0;
		homing();
	}
}

void Tumbler::paintEvent(QPaintEvent *) {
	QPainter painter(this);

	fprintf(stdout, "[tumbler] index = %d, offsetY = %d\n", index, offsetY);

	if (offsetY >= ih && index > 0) { // down
		scalePosY += ih;
		offsetY -= ih;
		index--;
		if (liveUpdate) emit changed(index);
	} else if (offsetY <= -ih && index < listCount - 1) { // up
		scalePosY -= ih;
		offsetY += ih;
		index++;
		if (liveUpdate) emit changed(index);
	}

	painter.setPen(QPen(QColor(192, 192, 192, 127), 2));
	painter.drawLine(1, h / 2 - ih / 2, w - 1, h / 2 - ih / 2);
	painter.drawLine(1, h / 2 + ih / 2, w - 1, h / 2 + ih / 2);

	paintItem(painter, index, offsetY);

	int idx;
	for (int i = semiCnt + 1; i > 0; --i) {
		idx = index - i;
		if (idx >= 0)
			paintItem(painter, idx, offsetY - ih * i);
	}
	for (int i = 1; i <= semiCnt + 1; ++i) {
		idx = index + i;
		if (idx <= listCount - 1)
			paintItem(painter, idx, offsetY + ih * i);
	}
}

void Tumbler::paintItem(QPainter &painter, int val, int off) {
	float scale = 1 - factor * qAbs(off) / h;
	int fs = RANGE(fontSize * scale, 1, fontSize);
	int transparent = RANGE(255 * scale, 0, 255);
	int y = h / 2 - ih / 2 + off;

	fprintf(stdout, "[tumbler] scale = %.2lf\n", scale);

	QFont font;
	font.setPixelSize(fs);
	painter.setFont(font);
	painter.setPen(QPen(QColor(0, 0, 0, transparent)));
	painter.drawText(0, y, iw, ih, Qt::AlignCenter, tr("row") + QString::number(val));
}

void Tumbler::homing() {
	if (offsetY > threshold && index > 0) {
		index--;
		animation->setStartValue(offsetY - ih);
		animation->setEndValue(0);
	} else if (offsetY < -threshold && index < listCount - 1) {
		index++;
		animation->setStartValue(offsetY + ih);
		animation->setEndValue(0);
	} else {
		animation->setStartValue(offsetY);
		animation->setEndValue(0);
	}
	animation->start();
	if (liveUpdate) emit changed(index);
}

void Tumbler::emitTimeout() {
	emit changed(index);
}

void Tumbler::finished() {
	fprintf(stdout, "[tumbler] " "\033[34m" "finished homing()" "\033[0m" "\n");
	emit changed(index);
}

//
// NetworkMenu
//

DigitTumbler::DigitTumbler(QWidget *parent)
    : Tumbler(parent)
{
	this->animation = new QPropertyAnimation(this, "offset");
	this->animation->setDuration(300);
	this->animation->setEasingCurve(QEasingCurve::OutQuad);

	connect(animation, SIGNAL(finished()), this, SLOT(finished()));

	cnt = 3;

	this->semiCnt    = cnt / 2;
	this->listCount  = 10;
	this->index      = 6;
	this->isDragging = 0;
	this->threshold  = 20;
	this->scalePosY  = 0;
	this->offsetY    = 0;
	this->fontSize   = 30; // font pixel size
	this->iw         = 60;
	this->ih         = fontSize + 20;
	this->w          = iw;
	this->h          = cnt * ih;
	this->threshold  = ih / 2;
	this->factor     = 1;
	this->liveUpdate = false;

	this->setFixedSize(this->w, this->h);

	this->emitTimer = new QTimer(this);
	this->emitTimer->setSingleShot(true);
	connect(emitTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
}

bool DigitTumbler::event(QEvent *e) {
	if (e->type() == QEvent::KeyPress)
    {
        fprintf(stdout, "[tumbler] KeyPress\n");
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_R) {
            fprintf(stdout, "[tumbler] Key_R\n");
            keyEvent->ignore();
            return true;
        }
    }
	return QWidget::event(e);
}

void DigitTumbler::keyPressEvent(QKeyEvent *e) {
	fprintf(stdout, "[tumbler] call keyPressEvent()\n");
    switch(e->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_1:
	case Qt::Key_2:
	case Qt::Key_3:
	case Qt::Key_4:
	case Qt::Key_5:
	case Qt::Key_6:
	case Qt::Key_7:
	case Qt::Key_8:
	case Qt::Key_9:
	case Qt::Key_0:
	case Qt::Key_A:
	case Qt::Key_C:
	case Qt::Key_D:
	case Qt::Key_E:
	case Qt::Key_F:
	case Qt::Key_I:
	case Qt::Key_M:
	case Qt::Key_N:
	case Qt::Key_Q:
	case Qt::Key_R:
	case Qt::Key_S:
	case Qt::Key_W:
        if (emitTimer->isActive()) emit changed(index);
        QWidget::keyPressEvent(e);
		break;
	case Qt::Key_Up:
	case Qt::Key_Left:
        fprintf(stdout, "[tumbler] Key_Left\n");
        if (index > 0) {
        	offsetY = ih;
        	//animation->stop();
        	//homing();
        	repaint();
        	if(!liveUpdate) emitTimer->start(500);
        }
        break;
	case Qt::Key_Down:
	case Qt::Key_Right:
        fprintf(stdout, "[tumbler] Key_Right\n");
        if (index < listCount - 1) {
        	offsetY = -ih;
        	//animation->stop();
        	//homing();
        	repaint();
        	if(!liveUpdate) emitTimer->start(500);
        }
        break;
	default:
        QWidget::keyPressEvent(e);
        break;
    }
}

void DigitTumbler::mousePressEvent(QMouseEvent *e) {
	animation->stop();
	isDragging = 1;
	scalePosY = e->pos().y();
}

void DigitTumbler::mouseMoveEvent(QMouseEvent *e) {
	if (isDragging) {
		posY = e->pos().y();

		offsetY = posY - scalePosY;
		repaint();
	}
}

void DigitTumbler::mouseReleaseEvent(QMouseEvent *) {
	fprintf(stdout, "[tumbler] " "\033[32m" "call mouseReleaseEvent()" "\033[0m" ", index = %d, offsetY = %d\n", index, offsetY);

	if (isDragging) {
		isDragging = 0;
		homing();
	}
}

void DigitTumbler::paintEvent(QPaintEvent *) {
	QPainter painter(this);

	fprintf(stdout, "[tumbler] index = %d, offsetY = %d\n", index, offsetY);
	
	if (offsetY >= ih) { // down
		scalePosY += ih;
		offsetY -= ih;
		index--;
		if (index < 0) index = 9;
	} else if (offsetY <= -ih) { // up
		scalePosY -= ih;
		offsetY += ih;
		index++;
		if (index > 9) index = 0;
	}

	painter.setPen(QPen(QColor(192, 192, 192, 127), 2));
	painter.drawLine(1, h / 2 - ih / 2, w - 1, h / 2 - ih / 2);
	painter.drawLine(1, h / 2 + ih / 2, w - 1, h / 2 + ih / 2);

	paintItem(painter, index, offsetY);

	int idx;
	for (int i = semiCnt; i > 0; --i) {
		idx = index - i;
		paintItem(painter, idx < 0 ? idx + 10 : idx, offsetY - ih * i);
	}
	for (int i = 1; i <= semiCnt; ++i) {
		idx = index + i;
		paintItem(painter, idx > 9 ? idx - 10 : idx, offsetY + ih * i);
	}
}

void DigitTumbler::paintItem(QPainter &painter, int val, int off) {
	float scale = 1 - factor * qAbs(off) / h;
	int fs = fontSize;
	int transparent = RANGE(255 * scale, 0, 255);
	int y = h / 2 - ih / 2 + off;

	fprintf(stdout, "[tumbler] scale = %.2lf\n", scale);

	QFont font;
	font.setPixelSize(fs);
	painter.setFont(font);
	painter.setPen(QPen(QColor(0, 0, 0, transparent)));
	painter.drawText(0, y, iw, ih, Qt::AlignCenter, QString::number(val));
}

void DigitTumbler::emitTimeout() {
	emit changed(index);
}

void DigitTumbler::finished() {
	fprintf(stdout, "[tumbler] " "\033[34m" "finished homing()" "\033[0m" "\n");
	emit changed(index);
}
