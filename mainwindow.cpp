#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int counter = {};
    ui->buttonCopy->setEnabled(false);
    connect(ui->buttonCopy, &QPushButton::clicked, this, &MainWindow::buttonCopy_clicked);
    connect(ui->buttonDelete, &QPushButton::clicked, this, &MainWindow::buttonDelete_clicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::lineEdit_textChanged);
    connect(ui->buttonExit, &QPushButton::clicked, this, &MainWindow::buttonExit_clicked);
    connect(ui->buttonClickCount, &QPushButton::clicked, this, &MainWindow::buttonClickCount_clicked);
    connect(ui->buttonCalculate_BMI_clicked, &QPushButton::clicked, this, &MainWindow::buttonCalculate_BMI_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
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
