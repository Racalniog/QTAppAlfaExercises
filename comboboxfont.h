#ifndef COMBOBOXFONT_H
#define COMBOBOXFONT_H

#include <QWidget>

namespace Ui {
class ComboBoxFont;
}

class ComboBoxFont : public QWidget
{
    Q_OBJECT

public:
    explicit ComboBoxFont(QWidget *parent = nullptr);
    ~ComboBoxFont();

private slots:
    void updateFont();
    void updateFontColor();

private:
    Ui::ComboBoxFont *ui;
    QFont font;
};

#endif // COMBOBOXFONT_H
