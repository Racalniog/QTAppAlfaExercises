#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "windowtwo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void buttonCopy_clicked();
    void buttonDelete_clicked();
    void lineEdit_textChanged(const QString &arg1);
    void buttonExit_clicked();
    void buttonClickCount_clicked();
    void buttonCalculate_BMI_clicked();
    void actionWindow_1_clicked();
    void actionWindow_2_clicked();

private:
    QStackedWidget *m_stackedWidget;
    Ui::MainWindow *ui;
    int counter;
    WindowTwo *secondWindow;
    QWidget *centralZone;
};
#endif // MAINWINDOW_H
