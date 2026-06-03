#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Зв'язка кнопки
    connect(ui->pb_calculate, &QPushButton::clicked, this, &MainWindow::on_pb_calculate_clicked);

    // Зв'язка меню
    connect(ui->actionLoad_from_file, &QAction::triggered, this, &MainWindow::on_actionLoad_from_file_triggered);
    connect(ui->actionSave_to_file, &QAction::triggered, this, &MainWindow::on_actionSave_to_file_triggered);

    ui->aValue->setValidator(
        new QDoubleValidator(0.0001, 999999.0, 4, this));

    ui->bValue->setValidator(
        new QDoubleValidator(0.0001, 999999.0, 4, this));

    ui->cValue->setValidator(
        new QDoubleValidator(0.0001, 999999.0, 4, this));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_calculate_clicked()
{
    bool ok1;
    bool ok2;
    bool ok3;

    double a = ui->aValue->text().toDouble(&ok1);
    double b = ui->bValue->text().toDouble(&ok2);
    double c = ui->cValue->text().toDouble(&ok3);

    if (!ok1 || !ok2 || !ok3)
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Введіть коректні числові значення!"
            );
        return;
    }

    if (a <= 0 || b <= 0 || c <= 0)
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Довжини ребер повинні бути більші за нуль!"
            );
        return;
    }

    double V = a * b * c;

    double S = 2 * (a * b + b * c + a * c);

    ui->volumeValue->setText(
        QString::number(V, 'f', 2));

    ui->surfaceValue->setText(
        QString::number(S, 'f', 2));
}

void MainWindow::on_actionLoad_from_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Відкрити файл",
        QDir::currentPath(),
        "Текстові файли (*.txt);;Усі файли (*)"
        );

    if (filePath.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Файл не вибрано"
            );
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Не вдалося відкрити файл"
            );
        return;
    }

    QTextStream in(&file);

    QString a;
    QString b;
    QString c;
    QString V;
    QString S;

    in >> a >> b >> c >> V >> S;

    if (a.isEmpty() ||
        b.isEmpty() ||
        c.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Файл має неправильний формат!"
            );

        file.close();
        return;
    }

    ui->aValue->setText(a);
    ui->bValue->setText(b);
    ui->cValue->setText(c);
    ui->volumeValue->setText(V);
    ui->surfaceValue->setText(S);

    file.close();
}

void MainWindow::on_actionSave_to_file_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Зберегти файл",
        QDir::currentPath(),
        "Текстові файли (*.txt);;Усі файли (*)"
        );

    if (filePath.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Файл не вибрано"
            );
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(
            this,
            "Помилка",
            "Не вдалося відкрити файл:\n" +
                file.errorString()
            );
        return;
    }

    QTextStream out(&file);

    out << ui->aValue->text() << "\n";
    out << ui->bValue->text() << "\n";
    out << ui->cValue->text() << "\n";
    out << ui->volumeValue->text() << "\n";
    out << ui->surfaceValue->text() << "\n";

    file.close();

    QMessageBox::information(
        this,
        "Успіх",
        "Дані успішно збережені!"
        );
}
