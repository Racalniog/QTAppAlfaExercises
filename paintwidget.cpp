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

void PaintWidget::drawFilledRectangle(bool)
{
    circle = false;
    filledCircle = false;
    rectangle = true;
    filledRectangle = true;
    line = false;
    update();
}

void PaintWidget::drawLine(bool)
{
    circle = false;
    filledCircle = false;
    rectangle = false;
    filledRectangle = false;
    line = true;
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
    }
}
