#include "comboboxfont.h"
#include "ui_comboboxfont.h"

ComboBoxFont::ComboBoxFont(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComboBoxFont)
{
    ui->setupUi(this);
    QStringList fontStyles = {"Normal", "Bold", "Italic"};
    QStringList fontSizes = {"8", "10", "12", "14", "16"};
    QStringList fontColors = {"Black", "Red", "Blue"};

    ui->fontStyleComboBox->addItems(fontStyles);
    connect(ui->fontStyleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComboBoxFont::updateFont);

    ui->fontSizeComboBox->addItems(fontSizes);
    connect(ui->fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComboBoxFont::updateFont);

    ui->fontColorComboBox->addItems(fontColors);
    connect(ui->fontColorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComboBoxFont::updateFontColor);

    connect(ui->fontTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComboBoxFont::updateFont);
}

void ComboBoxFont::updateFont() {
    font.setBold(ui->fontStyleComboBox->currentIndex() == 1);
    font.setItalic(ui->fontStyleComboBox->currentIndex() == 2);
    font.setPointSize(ui->fontSizeComboBox->currentText().toInt());
    font.setFamily(ui->fontTypeComboBox->currentText());
    ui->fontExampleQLabel->setFont(font);
}

void ComboBoxFont::updateFontColor() {
    QString color = ui->fontColorComboBox->currentText();
    if (color == "Black") {
        ui->fontExampleQLabel->setStyleSheet("color: black;");
    } else if (color == "Red") {
        ui->fontExampleQLabel->setStyleSheet("color: red;");

    } else if (color == "Blue") {
        ui->fontExampleQLabel->setStyleSheet("color: blue;");
    }
}

ComboBoxFont::~ComboBoxFont()
{
    delete ui;
}
