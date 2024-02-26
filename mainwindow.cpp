#include "mainwindow.h"
#include "./ui_mainwindow.h"

//TODO: add missing ui, cpp and h files
// add     //findstr /R /N "^" ".\*.cpp" ".\*.h" | find /C ":"
// 22.2 1525 loc

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
    ui->stackedWidget->setCurrentWidget(ui->page_9);
    setWindowTitle("Multi Purpose Application");
    QFile file(":/Adaptic.qss");

    if (!file.exists()) {
        qDebug() << "Error: Stylesheet file does not exist";
        qDebug() << "File path: " << file.fileName();
    }

    if (!file.permissions().testFlag(QFile::ReadUser)) {
        qDebug() << "Error: No read permissions for stylesheet file";
        qDebug() << "File path: " << file.fileName();
    }

    if (file.open(QIODevice::ReadOnly)) {
        QString stylesheet = QTextStream(&file).readAll();
        ui->page->setStyleSheet(stylesheet);
        ui->page_2->setStyleSheet(stylesheet);
        ui->page_5->setStyleSheet(stylesheet);
        ui->page_7->setStyleSheet(stylesheet);

        file.close();
    } else {
        qDebug() << "Error: Could not open stylesheet file";
        qDebug() << "File path: " << file.fileName();
        qDebug() << "Error string: " << file.errorString();
    }

}

void MainWindow::updateWindowTitle()
{
    QAction *action = qobject_cast<QAction*>(sender()); // Get the sender QAction
    if (action) {
        currentAction = action; // Update the currentAction pointer
        setWindowTitle(action->text()); // Set the window title to the text of the QAction
    }
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
    connect(ui->actionWindow_1, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_2, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_3, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_4, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_5, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_6, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_7, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_8, &QAction::triggered, this, &MainWindow::updateWindowTitle);
    connect(ui->actionWindow_9, &QAction::triggered, this, &MainWindow::updateWindowTitle);

    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->actionWindow_1, &QAction::triggered, this, &MainWindow::actionWindow_1_clicked);
    connect(ui->actionWindow_2, &QAction::triggered, this, &MainWindow::actionWindow_2_clicked);
    connect(ui->actionWindow_3, &QAction::triggered, this, &MainWindow::actionWindow_3_clicked);
    connect(ui->actionWindow_4, &QAction::triggered, this, &MainWindow::actionWindow_4_clicked);
    connect(ui->actionWindow_5, &QAction::triggered, this, &MainWindow::actionWindow_5_clicked);
    connect(ui->actionWindow_6, &QAction::triggered, this, &MainWindow::actionWindow_6_clicked);
    connect(ui->actionWindow_7, &QAction::triggered, this, &MainWindow::actionWindow_7_clicked);
    connect(ui->actionWindow_8, &QAction::triggered, this, &MainWindow::actionWindow_8_clicked);
    connect(ui->actionWindow_9, &QAction::triggered, this, &MainWindow::actionWindow_9_clicked);
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

void MainWindow::actionWindow_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_6);
}

void MainWindow::actionWindow_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_7);
}

void MainWindow::actionWindow_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_8);
}

void MainWindow::actionWindow_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_9);
}
