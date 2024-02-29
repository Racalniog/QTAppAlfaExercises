#include "mainwindow.h"
#include <QApplication>
/*
 *  author: Giancarlo Stooss
 *  All rights reserved.
 *
 *  This program is proprietary and confidential. Any commercial use, reproduction,
 *  distribution, or disclosure of this program, or any part of it, without
 *  the express written consent of the author is strictly prohibited.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
