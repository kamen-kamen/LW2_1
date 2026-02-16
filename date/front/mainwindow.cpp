#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QTableWidgetItem> // Обязательно для работы с таблицей

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);

    // Инициализация таблицы
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Дата", "След. день", "Високосный", "Разница (дни)"});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    currentFilePath = fileName;
    dates.clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Регулярное выражение для поиска дат формата DD.MM.YYYY
    QStringList tokens = content.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    for (const QString &token : tokens) {
        QStringList parts = token.split('.');
        if (parts.size() == 3) {
            try {
                Date newDate(parts[0].toInt(), parts[1].toInt(), parts[2].toInt());
                dates.push_back(newDate);
            } catch (...) {
                qDebug() << "Некорректная дата в файле:" << token;
            }
        }
    }
    updateTable();
}

void MainWindow::on_btnAdd_clicked()
{
    // Используем QDateEdit (стандартный виджет Qt)
    QDate qDt = ui->dateEditAdd->date();

    try {
        Date newDate(qDt.day(), qDt.month(), qDt.year());
        dates.push_back(newDate);

        if (!currentFilePath.isEmpty()) {
            saveDateToFile(newDate);
        }
        updateTable();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::on_btnCalcBday_clicked()
{
    QDate qDt = ui->dateEditBday->date();
    try {
        Date bday(qDt.day(), qDt.month(), qDt.year());
        Date current; // Текущая системная дата
        int days = current.daysTillYourBirthday(bday);
        ui->lblResult->setText("До дня рождения: " + QString::number(days) + " дн.");
    } catch (...) {
        ui->lblResult->setText("Ошибка расчета");
    }
}

void MainWindow::updateTable()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(dates.size());

    for (int i = 0; i < (int)dates.size(); ++i) {
        Date next = dates[i].nextDay();
        QString diffStr = "-";

        if (i < (int)dates.size() - 1) {
            diffStr = QString::number(dates[i].duration(dates[i+1]));
        }

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(dates[i].toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(next.toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(dates[i].isLeap() ? "Да" : "Нет"));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(diffStr));
    }
}

void MainWindow::saveDateToFile(const Date& date)
{
    QFile file(currentFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << " " << date.toString();
        file.close();
    }
}