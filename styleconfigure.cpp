#include "styleconfigure.h"
#include "ui_styleconfigure.h"

StyleConfigure::StyleConfigure(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StyleConfigure)
{
    ui->setupUi(this);
    ui->frame_3->setAutoFillBackground(true);
    initStyleConfigure();
}

void StyleConfigure::initStyleConfigure()
{
    appFont = this->font();
    ui->radioButtonBackgroundGrey->setChecked(true);
    ui->radioButtonForegroundBlack->setChecked(true);
    ui->lineEditSize->setText(QString::number(this->font().pointSize()));
    ui->lineEditSize->setMaxLength(2);

    for(QRadioButton* rb : ui->groupBoxBackground->findChildren<QRadioButton*>())
        connect (rb, &QRadioButton::toggled, this, &StyleConfigure::setBackColor);

    for(QRadioButton* rb : ui->groupBoxForeground->findChildren<QRadioButton*>())
        connect (rb, &QRadioButton::toggled, this, &StyleConfigure::setForeColor);

    connect(ui->lineEditSize, &QLineEdit::editingFinished, this, &StyleConfigure::setFontSize);
    connect(ui->lineEditSize, &QLineEdit::returnPressed, this, &StyleConfigure::focusNextChild);
    connect(ui->checkBoxBold, &QCheckBox::toggled, this, &StyleConfigure::setFontStyle);
    connect(ui->checkBoxItalic, &QCheckBox::toggled, this, &StyleConfigure::setFontStyle);
}

void StyleConfigure::setBackColor()
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


void StyleConfigure::setForeColor()
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

void StyleConfigure::setFontStyle()
{
    appFont.setWeight(QFont::Normal);
    if (ui->checkBoxBold->isChecked())
        appFont.setWeight(QFont::Bold);
    appFont.setItalic(ui->checkBoxItalic->isChecked());
    ui->groupBoxBackground->setFont(appFont);
    ui->groupBoxForeground->setFont(appFont);
    ui->groupBoxFont->setFont(appFont);
}

void StyleConfigure::setFontSize()
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

void StyleConfigure::setFont(QWidget *widget, QFont& font)
{
    widget->setFont(font);

    for(QWidget* child : widget->findChildren<QWidget*>())
        child->setFont(font);
}


StyleConfigure::~StyleConfigure()
{
    delete ui;
}
