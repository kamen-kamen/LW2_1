#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include <QHeaderView>


enum Columns { ColDate, ColNext, ColLeap, ColDiff, ColCount };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(ColCount);
    ui->tableWidget->setHorizontalHeaderLabels({"Дата", "След. день", "Високосный", "Разница (дни)"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

    QStringList tokens = content.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    for (const QString &token : tokens) {
        QStringList parts = token.split('.');
        if (parts.size() == 3) {
            bool okD, okM, okY;
            int d = parts[0].toInt(&okD);
            int m = parts[1].toInt(&okM);
            int y = parts[2].toInt(&okY);

            if (okD && okM && okY) {
                try {
                    dates.emplace_back(d, m, y);
                } catch (...) {
                    qDebug() << "Invalid date in file:" << token;
                }
            }
        }
    }
    updateTable();
}

void MainWindow::on_btnAdd_clicked()
{
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
        Date current;
        int days = current.daysTillYourBirthday(bday);
        ui->lblResult->setText(QString("До дня рождения: %1 дн.").arg(days));
    } catch (...) {
        ui->lblResult->setText("Ошибка расчета");
    }
}

void MainWindow::updateTable()
{
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setRowCount(0);

    for (size_t i = 0; i < dates.size(); ++i) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QString diffStr = (i < dates.size() - 1)
            ? QString::number(dates[i].duration(dates[i + 1]))
            : "-";

        ui->tableWidget->setItem(row, ColDate, new QTableWidgetItem(dates[i].toString()));
        ui->tableWidget->setItem(row, ColNext, new QTableWidgetItem(dates[i].nextDay().toString()));
        ui->tableWidget->setItem(row, ColLeap, new QTableWidgetItem(dates[i].isLeap() ? "Да" : "Нет"));
        ui->tableWidget->setItem(row, ColDiff, new QTableWidgetItem(diffStr));
    }
    ui->tableWidget->setSortingEnabled(true);
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
