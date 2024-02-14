#include "mainwindow.h"
#include "./ui_mainwindow.h"

//TODO: add missing ui, cpp and h files
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initQStackedWidget();
    initBMI();
    initNumberSystem();
    initColorMixer();
    initList();
}

void MainWindow::initList(){
    for(int i = 0; i < 5; ++i) {
        QListWidgetItem* item = new QListWidgetItem("Item " + QString::number(i + 1));
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
    }

    for(int i = 5; i < 10; ++i) {
        QListWidgetItem* item = new QListWidgetItem("Item " + QString::number(i + 1));
        ui->listWidget_2->addItem(item);
    }
    connect(ui->pushButtonDeleteMarked, &QPushButton::clicked, this, &MainWindow::deleteMarkedItem);
    connect(ui->pushButtonAddItem, &QPushButton::clicked, this, &MainWindow::addItem);
}

void MainWindow::deleteMarkedItem(){
        for (QListWidgetItem* item : ui->listWidget->selectedItems())
        {
            if (item->checkState() == Qt::Checked) {
                ui->listWidget->removeItemWidget(item);
                delete item;
            }
        }
        for (QListWidgetItem* item : ui->listWidget_2->selectedItems())
        {
            if (item->checkState() == Qt::Checked) {
                ui->listWidget_2->removeItemWidget(item);
                delete item;
            }
        }
}

void MainWindow::addItem(){
        ui->listWidget_2->addItem("New Item");
}

void MainWindow::initColorMixer(){
    connect(ui->redSlider, &QSlider::valueChanged, this, &MainWindow::onRedChanged);
    connect(ui->greenSlider, &QSlider::valueChanged, this, &MainWindow::onGreenChanged);
    connect(ui->blueSlider, &QSlider::valueChanged, this, &MainWindow::onBlueChanged);

    ui->colorPreview->setAutoFillBackground(true);
    onRedChanged(ui->redSlider->value());
    onGreenChanged(ui->greenSlider->value());
    onBlueChanged(ui->blueSlider->value());
    ui->redSlider->setMinimum(0);
    ui->redSlider->setMaximum(255);
    ui->redSlider->setValue(127);
    ui->greenSlider->setMinimum(0);
    ui->greenSlider->setMaximum(255);
    ui->greenSlider->setValue(127);
    ui->blueSlider->setMinimum(0);
    ui->blueSlider->setMaximum(255);
    ui->blueSlider->setValue(127);
}

void MainWindow::onRedChanged(int value)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(value, palette.color(QPalette::Window).green(),
                                              palette.color(QPalette::Window).blue()));
    ui->colorPreview->setPalette(palette);
    ui->redValue->setNum(value);
}

void MainWindow::onGreenChanged(int value)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(palette.color(QPalette::Window).red(),
                                              value, palette.color(QPalette::Window).blue()));
    ui->colorPreview->setPalette(palette);
    ui->greenValue->setNum(value);
}

void MainWindow::onBlueChanged(int value)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(palette.color(QPalette::Window).red(),
                                              palette.color(QPalette::Window).green(), value));
    ui->colorPreview->setPalette(palette);
    ui->blueValue->setNum(value);
}

void MainWindow::initQStackedWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->actionWindow_1, &QAction::triggered, this, &MainWindow::actionWindow_1_clicked);
    connect(ui->actionWindow_2, &QAction::triggered, this, &MainWindow::actionWindow_2_clicked);
    connect(ui->actionWindow_3, &QAction::triggered, this, &MainWindow::actionWindow_3_clicked);
    connect(ui->actionWindow_4, &QAction::triggered, this, &MainWindow::actionWindow_4_clicked);
    connect(ui->actionWindow_5, &QAction::triggered, this, &MainWindow::actionWindow_5_clicked);
}



void MainWindow::initNumberSystem()
{
    ui->radioButtonBinary->setChecked(true);
    basis = {};
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

void MainWindow::actionWindow_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_5);
}
