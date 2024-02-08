#include "windowtwo.h"
#include "ui_windowtwo.h"

WindowTwo::WindowTwo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WindowTwo)
{
    ui->setupUi(this);
}

WindowTwo::~WindowTwo()
{
    delete ui;
}
