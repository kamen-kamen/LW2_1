#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QString>
#include <ui_mainwindow.h>

#include  "../back/Date.h" // Убедитесь, что файл лежит в той же папке или укажите верный путь

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnOpen_clicked();
    void on_btnAdd_clicked();
    void on_btnCalcBday_clicked();

private:
    Ui::mainwindow *ui;
    std::vector<Date> dates;
    QString currentFilePath;

    void updateTable();
    void saveDateToFile(const Date& date);
};

#endif // MAINWINDOW_H