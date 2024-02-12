#include "mainwindow.h"
#include "./ui_mainwindow.h"
//TODO resize windows to fit content dynamically
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int counter = {};
    ui->radioButtonBinary->setChecked(true);
    basis = {};
    ui->stackedWidget->setCurrentIndex(0);

    ui->buttonCopy->setEnabled(false);
    connect(ui->buttonCopy, &QPushButton::clicked, this, &MainWindow::buttonCopy_clicked);
    connect(ui->buttonDelete, &QPushButton::clicked, this, &MainWindow::buttonDelete_clicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::lineEdit_textChanged);
    connect(ui->buttonExit, &QPushButton::clicked, this, &MainWindow::buttonExit_clicked);
    connect(ui->buttonClickCount, &QPushButton::clicked, this, &MainWindow::buttonClickCount_clicked);
    connect(ui->buttonCalculate_BMI_clicked, &QPushButton::clicked, this, &MainWindow::buttonCalculate_BMI_clicked);
    connect(ui->actionWindow_1, &QAction::triggered, this, &MainWindow::actionWindow_1_clicked);
    connect(ui->actionWindow_2, &QAction::triggered, this, &MainWindow::actionWindow_2_clicked);
    connect(ui->actionWindow_3, &QAction::triggered, this, &MainWindow::actionWindow_3_clicked);
    connect(ui->actionUiButton, &QAction::triggered, this, &MainWindow::actionWindow_4_clicked);
    connect(ui->radioButtonBinary, &QRadioButton::clicked, this, &MainWindow::convertNumberSystem);
    connect(ui->radioButtonOctal, &QRadioButton::clicked, this, &MainWindow::convertNumberSystem);
    connect(ui->radioButtonHexadecimal, &QRadioButton::clicked, this, &MainWindow::convertNumberSystem);
    connect(ui->lineEditInput, &QLineEdit::textChanged, this, &MainWindow::convertNumberSystem);
}

void MainWindow::convertNumberSystem()
{
    long long zahl = ui->lineEditInput->text().toLongLong();
    if(ui->radioButtonBinary->isChecked()){
        basis = 2;
    }else if(ui->radioButtonOctal->isChecked()){
        basis = 8;
    }else if(ui->radioButtonHexadecimal->isChecked()){
        basis = 16;
    }
    ui->lineEditOutput->setText(QString::number(zahl, basis));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actionWindow_1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::actionWindow_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::actionWindow_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void MainWindow::actionWindow_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::buttonCopy_clicked()
{
    QString readLabel = ui->lineEdit->text();
    if(!readLabel.isEmpty())
        ui->copyTarget->setText(readLabel);
    ui->lineEdit->clear();
}

void MainWindow::buttonDelete_clicked()
{
    ui->copyTarget->clear();
}

void MainWindow::lineEdit_textChanged(const QString &arg1)
{
    bool grey = ui->lineEdit->text().isEmpty();
    ui->buttonCopy->setEnabled(!grey);
}

void MainWindow::buttonExit_clicked()
{
    close();
}

void MainWindow::buttonClickCount_clicked()
{
    counter += 1;
    ui->labelCounter->setText(QString::number(counter));
}

void MainWindow::buttonCalculate_BMI_clicked()
{
    double bmi = {};
    QString readheight = ui->lineEdit_Height->text();
    QString readweight = ui->lineEdit_Weight->text();
    if(!readheight.isEmpty() && !readweight.isEmpty()){
        bmi = readweight.toDouble()  / (readheight.toDouble() * readheight.toDouble());
        ui->bmiCalculation->setText(QString::number(bmi));
        ui->lineEdit_Height->clear();
        ui->lineEdit_Weight->clear();
    }
}
