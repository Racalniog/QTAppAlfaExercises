#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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
    void actionWindow_3_clicked();
    void actionWindow_4_clicked();
    void convertNumberSystem();

private:
    Ui::MainWindow *ui;
    int counter;
    int basis;
};
#endif // MAINWINDOW_H
