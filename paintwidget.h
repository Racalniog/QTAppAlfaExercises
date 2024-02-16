#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QObject>
#include <QPainter>
#include <qcolordialog.h>

namespace Ui {
class PaintWidget;
}

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    PaintWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~PaintWidget();
public slots:
    void setPenColor();
    void setBrushColor();
    void drawCircle(const bool);
    void drawFilledCircle(const bool);
    void drawRectangle(const bool);
    void drawFilledRectangle(bool);
    void drawLine(bool);

private:
    Ui::PaintWidget *ui;
    QBrush brush;
    QPen pen;

    bool boolCircle;
    bool boolFilledCircle;
    bool boolRectangle;
    bool boolFilledRectangle;
    bool boolLine;

    void paintEvent(QPaintEvent* event) override;
};

#endif // PAINTWIDGET_H
