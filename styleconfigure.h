#ifndef STYLECONFIGURE_H
#define STYLECONFIGURE_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class StyleConfigure;
}

class StyleConfigure : public QWidget
{
    Q_OBJECT

public:
    explicit StyleConfigure(QWidget *parent = nullptr);
    ~StyleConfigure();
    void initStyleConfigure();

private slots:
    void setBackColor();
    void setForeColor();
    void setFontStyle();
    void setFontSize();
    void setFont(QWidget*, QFont&);

private:
    Ui::StyleConfigure *ui;
    QFont appFont;
    const int MIN_FONTSIZE = 5;
    const int MAX_FONTSIZE = 20;
};

#endif // STYLECONFIGURE_H
