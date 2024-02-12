#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMessageBox>

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
    void initQStackedWidget();
    void actionWindow_1_clicked();
    void actionWindow_2_clicked();
    void actionWindow_3_clicked();
    void actionWindow_4_clicked();

    void initBMI();
    void buttonCopy_clicked();
    void buttonDelete_clicked();
    void lineEdit_textChanged(const QString &arg1);
    void buttonExit_clicked();
    void buttonClickCount_clicked();
    void buttonCalculate_BMI_clicked();

    void initNumberSystem();
    void convertNumberSystem();

    void initStyleConfigure();
    void setBackColor();
    void setForeColor();
    void setFontStyle();
    void setFontSize();
    void setFont(QWidget*, QFont&);

    void initColorMixer();
    void onRedChanged(int value);
    void onGreenChanged(int value);
    void onBlueChanged(int value);

private:
    Ui::MainWindow *ui;
    QFont appFont;
    int counter;
    int basis;
    const int MIN_FONTSIZE = 5;
    const int MAX_FONTSIZE = 20;
};
#endif // MAINWINDOW_H
