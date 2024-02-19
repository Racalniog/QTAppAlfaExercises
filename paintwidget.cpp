#include "paintwidget.h"
#include "ui_paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::PaintWidget)
{
    ui->setupUi(this);
    ui->paintArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(ui->penColorButton, &QPushButton::clicked, this, &PaintWidget::setPenColor);
    connect(ui->brushColorButton, &QPushButton::clicked, this, &PaintWidget::setBrushColor);
    connect(ui->circleButton, &QPushButton::clicked, this, &PaintWidget::drawCircle);
    connect(ui->filledCircleButton, &QPushButton::clicked, this, &PaintWidget::drawFilledCircle);
    connect(ui->rectangleButton, &QPushButton::clicked, this, &PaintWidget::drawRectangle);
    connect(ui->filledRectangleButton, &QPushButton::clicked, this, &PaintWidget::drawFilledRectangle);
    connect(ui->lineButton, &QPushButton::clicked, this, &PaintWidget::drawLine);

    brush = QBrush(Qt::black);
    pen = QPen(Qt::black);
    circle = false;
    filledCircle = false;
    rectangle = false;
    filledRectangle = false;
    line = false;
    egg = false;
}

PaintWidget::~PaintWidget()
{
    delete ui;
}

void PaintWidget::setPenColor()
{
    QColor color = QColorDialog::getColor(pen.color(), this, "Select Pen Color");
    if (color.isValid()) {
        pen.setColor(color);
        update();
    }
}

void PaintWidget::setBrushColor()
{
    QColor color = QColorDialog::getColor(brush.color(), this, "Select Brush Color");
    if (color.isValid()) {
        brush.setColor(color);
        update();
    }
}

void PaintWidget::drawCircle(const bool)
{
    circle = true;
    filledCircle = false;
    rectangle = false;
    filledRectangle = false;
    line = false;
    update();
}

void PaintWidget::drawFilledCircle(const bool)
{
    circle = true;
    filledCircle = true;
    rectangle = false;
    filledRectangle = false;
    line = false;
    update();
}

void PaintWidget::drawRectangle(const bool)
{
    circle = false;
    filledCircle = false;
    rectangle = true;
    filledRectangle = false;
    line = false;
    update();
}

void PaintWidget::drawFilledRectangle(const bool)
{
    circle = false;
    filledCircle = false;
    rectangle = true;
    filledRectangle = true;
    line = false;
    update();
}

void PaintWidget::drawLine(const bool)
{
    circle = false;
    filledCircle = false;
    rectangle = false;
    filledRectangle = false;
    line = true;
    update();
}

void PaintWidget::drawEgg(const bool)
{
    circle = false;
    filledCircle = false;
    rectangle = false;
    filledRectangle = false;
    line = false;
    egg = true;
    update();
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(brush);
    painter.setPen(pen);

    if (circle) {
        if (filledCircle)
            painter.drawEllipse(ui->paintArea->rect());
        else{
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(ui->paintArea->rect().adjusted(10, 10, -10, -10));
        }
    } else if (rectangle) {
        if (filledRectangle)
            painter.drawRect(ui->paintArea->rect());
        else{
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(ui->paintArea->rect().adjusted(10, 10, -10, -10));
        }
    } else if (line) {
        painter.drawLine(ui->paintArea->rect().topLeft(), ui->paintArea->rect().bottomRight());
    } else if (egg){
        drawEasterEgg();
    }

}

void PaintWidget::drawEasterEgg()
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    // Draw the egg shape
    painter.setBrush(Qt::yellow);
    painter.drawEllipse(100, 100, 100, 150);

    // Draw the decorations on the egg
    painter.setBrush(Qt::red);
    painter.drawEllipse(125, 110, 30, 30);

    painter.setBrush(Qt::blue);
    painter.drawEllipse(155, 120, 30, 30);

    painter.setBrush(Qt::green);
    painter.drawEllipse(130, 200, 30, 30);

    painter.setBrush(Qt::magenta);
    painter.drawEllipse(140, 150, 30, 30);

    painter.setBrush(Qt::cyan);
    painter.drawEllipse(165, 180, 30, 30);

    painter.setBrush(Qt::darkYellow);
    painter.drawEllipse(105, 150, 30, 30);
}
