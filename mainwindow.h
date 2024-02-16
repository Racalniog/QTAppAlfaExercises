#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "paintwidget.h"

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
    void initQStackedWidget();
    void initBMI();
    void initNumberSystem();
    void initColorMixer();
    void initList();

signals:
    void emitDrawCircle(bool);

private slots:
    //initQStackedWidget
    void actionWindow_1_clicked();
    void actionWindow_2_clicked();
    void actionWindow_3_clicked();
    void actionWindow_4_clicked();
    void actionWindow_5_clicked();
    void actionWindow_6_clicked();
    void actionWindow_7_clicked();
    void actionWindow_8_clicked();

    //initBMI
    void buttonCopy_clicked();
    void buttonDelete_clicked();
    void lineEdit_textChanged(const QString &arg1);
    void buttonExit_clicked();
    void buttonClickCount_clicked();
    void buttonCalculate_BMI_clicked();

    //initNumberSystem
    void convertNumberSystem();

    //initColorMixer
    void onRedChanged(int value);
    void onGreenChanged(int value);
    void onBlueChanged(int value);

    //initList
    void deleteMarkedItem();
    void addItem();

private:
    Ui::MainWindow *ui;
    int counter = {};
    int basis;
};
#endif // MAINWINDOW_H
