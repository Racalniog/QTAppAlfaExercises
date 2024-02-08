#ifndef WINDOWTWO_H
#define WINDOWTWO_H

#include <QWidget>

namespace Ui {
class WindowTwo;
}

class WindowTwo : public QWidget
{
    Q_OBJECT

public:
    explicit WindowTwo(QWidget *parent = nullptr);
    ~WindowTwo();

private:
    Ui::WindowTwo *ui;
};

#endif // WINDOWTWO_H
