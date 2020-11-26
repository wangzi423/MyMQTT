#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectData(QString strDevice, QString strUsrName, QString pw);
    void subscribeTopic(QString topics, QString QOS);
    void publishTopic(QString topics, QString data);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
