#include "mainwindow.h"
#include "./ui_mainwindow.h"
//TODO resize windows to fit content dynamically, rename variables
// fix structure: cpp and h files
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initQStackedWidget();
    initBMI();
    initStyleConfigure();
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

void MainWindow::initBMI()
{
    int counter = {};
    ui->buttonCopy->setEnabled(false);
    connect(ui->buttonCopy, &QPushButton::clicked, this, &MainWindow::buttonCopy_clicked);
    connect(ui->buttonDelete, &QPushButton::clicked, this, &MainWindow::buttonDelete_clicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::lineEdit_textChanged);
    connect(ui->buttonExit, &QPushButton::clicked, this, &MainWindow::buttonExit_clicked);
    connect(ui->buttonClickCount, &QPushButton::clicked, this, &MainWindow::buttonClickCount_clicked);
    connect(ui->buttonCalculate_BMI_clicked, &QPushButton::clicked, this, &MainWindow::buttonCalculate_BMI_clicked);
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

void MainWindow::initStyleConfigure()
{
    appFont = this->font();
    ui->radioButtonBackgroundGrey->setChecked(true);
    ui->radioButtonForegroundBlack->setChecked(true);
    ui->lineEditSize->setText(QString::number(this->font().pointSize()));
    ui->lineEditSize->setMaxLength(2);

    for(QRadioButton* rb : ui->groupBoxBackground->findChildren<QRadioButton*>())
        connect (rb, &QRadioButton::toggled, this, &MainWindow::setBackColor);

    for(QRadioButton* rb : ui->groupBoxForeground->findChildren<QRadioButton*>())
        connect (rb, &QRadioButton::toggled, this, &MainWindow::setForeColor);

    connect(ui->lineEditSize, &QLineEdit::editingFinished, this, &MainWindow::setFontSize);
    connect(ui->lineEditSize, &QLineEdit::returnPressed, this, &MainWindow::focusNextChild);
    connect(ui->checkBoxBold, &QCheckBox::toggled, this, &MainWindow::setFontStyle);
    connect(ui->checkBoxItalic, &QCheckBox::toggled, this, &MainWindow::setFontStyle);
}

void MainWindow::setBackColor()
{
    for(QWidget* w : ui->groupBoxForeground->findChildren<QWidget*>())
        w->setEnabled(true);
    QPalette pal = this->palette();
    if (ui->radioButtonBackgroundGreen->isChecked())
        pal.setColor(QPalette::Window, Qt::green);
    else if (ui->radioButtonBackgroundBlue->isChecked())
    {
        pal.setColor(QPalette::Window, Qt::blue);
        ui->radioButtonForegroundBlue->setEnabled(false);
        ui->radioButtonForegroundBlack->setEnabled(false);
    }
    else if (ui->radioButtonBackgroundRed->isChecked())
    {
        pal.setColor(QPalette::Window, Qt::red);
        ui->radioButtonForegroundRed->setEnabled(false);
    }
    else if (ui->radioButtonBackgroundYellow->isChecked())
    {
        pal.setColor(QPalette::Window, Qt::yellow);
        ui->radioButtonForegroundYellow->setEnabled(false);
        ui->radioButtonForegroundWhite->setEnabled(false);
    }
    else if (ui->radioButtonBackgroundGrey->isChecked())
    {
        pal.setColor(QPalette::Window, Qt::lightGray);
        ui->radioButtonForegroundYellow->setEnabled(false);
        ui->radioButtonForegroundWhite->setEnabled(false);
    }

    this->setPalette(pal);
}


void MainWindow::setForeColor()
{
    for(QWidget* w : ui->groupBoxBackground->findChildren<QWidget*>())
        w->setEnabled(true);

    QPalette pal = this->palette();

    if (ui->radioButtonForegroundBlue->isChecked())
    {
        pal.setColor(QPalette::WindowText, Qt::blue);
        ui->radioButtonBackgroundBlue->setEnabled(false);
    }
    else if (ui->radioButtonForegroundRed->isChecked())
    {
        pal.setColor(QPalette::WindowText, Qt::red);
        ui->radioButtonBackgroundRed->setEnabled(false);
    }
    else if (ui->radioButtonForegroundYellow->isChecked())
    {
        pal.setColor(QPalette::WindowText, Qt::yellow);
        ui->radioButtonBackgroundYellow->setEnabled(false);
    }
    else if (ui->radioButtonForegroundBlack->isChecked())
    {
        pal.setColor(QPalette::WindowText, Qt::black);
        ui->radioButtonBackgroundBlue->setEnabled(false);
    }
    else if (ui->radioButtonForegroundWhite->isChecked())
    {
        pal.setColor(QPalette::WindowText, Qt::white);
        ui->radioButtonBackgroundYellow->setEnabled(false);
        ui->radioButtonBackgroundGrey->setEnabled(false);
    }

    this->setPalette(pal);

    ui->groupBoxBackground->setPalette(pal);
    ui->groupBoxForeground->setPalette(pal);
    ui->groupBoxFont->setPalette(pal);
}

void MainWindow::setFontStyle()
{
    appFont.setWeight(QFont::Normal);
    if (ui->checkBoxBold->isChecked())
        appFont.setWeight(QFont::Bold);
    appFont.setItalic(ui->checkBoxItalic->isChecked());
    ui->groupBoxBackground->setFont(appFont);
    ui->groupBoxForeground->setFont(appFont);
    ui->groupBoxFont->setFont(appFont);
}

void MainWindow::setFontSize()
{
    int fontSize = this->appFont.pointSize();
    int newFontSize;
    bool bOK;

    newFontSize = ui->lineEditSize->text().toInt(&bOK, 10);
    if (!bOK)
    {
        QMessageBox::critical(this, "Error", "Invalid Input");
        newFontSize = fontSize;
        ui->lineEditSize->setText(QString::number(fontSize));
    }
    else if (newFontSize < MIN_FONTSIZE)
    {
        QMessageBox::critical(this, "Error", "Minimum Font Size is " +
                                                  QString::number(MIN_FONTSIZE));
        newFontSize = fontSize;
        ui->lineEditSize->setText(QString::number(fontSize));

    }
    else if (newFontSize > MAX_FONTSIZE)
    {
        QMessageBox::warning(this, "Error", "Maximum Font Size is " +
                                                 QString::number(MAX_FONTSIZE));
        newFontSize = fontSize;
        ui->lineEditSize->setText(QString::number(fontSize));

    }

    if (newFontSize == fontSize)
        return;

    appFont.setPointSize(newFontSize);

    ui->groupBoxBackground->setFont(appFont);
    ui->groupBoxForeground->setFont(appFont);
    ui->groupBoxFont->setFont(appFont);
}

void MainWindow::setFont(QWidget *widget, QFont& font)
{
    widget->setFont(font);

    for(QWidget* child : widget->findChildren<QWidget*>())
        child->setFont(font);
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
