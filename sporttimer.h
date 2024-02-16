#ifndef SPORTTIMER_H
#define SPORTTIMER_H

#include <QWidget>

namespace Ui {
class SportTimer;
}

class SportTimer : public QWidget
{
    Q_OBJECT

public:
    explicit SportTimer(QWidget *parent = nullptr);
    ~SportTimer();

private:
    Ui::SportTimer *ui;
};

#endif // SPORTTIMER_H
