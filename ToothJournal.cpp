#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
ToothJournal::ToothJournal(QWidget *parent)
    : QWidget(parent) {
    setupUI();
    loadFromFile();
}

void ToothJournal::addEntry() {
    bool ok;
    QString date = QInputDialog::getText(this, "Добавить запись", "Дата прорезывания:", QLineEdit::Normal, "", &ok);
    if (!ok || date.isEmpty() || !QDate::fromString(date, "yyyy-MM-dd").isValid()) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат даты. Введите дату в формате ГГГГ-ММ-ДД.");
        return;
    }

    // Ввод позиции зуба (только числа)
    QString position = QInputDialog::getText(this, "Добавить запись", "Позиция зуба:", QLineEdit::Normal, "", &ok);
    if (!ok || position.isEmpty() || !position.toInt(&ok)) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат позиции зуба. Введите число.");
        return;
    }

    // Ввод номера зуба (только числа)
    QString toothNumber = QInputDialog::getText(this, "Добавить запись", "Номер зуба:", QLineEdit::Normal, "", &ok);
    if (!ok || toothNumber.isEmpty() || !toothNumber.toInt(&ok)) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат номера зуба. Введите число.");
        return;
    }

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(date));
    tableWidget->setItem(row, 1, new QTableWidgetItem(position));
    tableWidget->setItem(row, 2, new QTableWidgetItem(toothNumber));
    saveToFile();
}

void ToothJournal::removeEntry() {
    int selectedRow = tableWidget->currentRow();
    if (selectedRow >= 0) {
        tableWidget->removeRow(selectedRow);
        saveToFile();
    } else {
        QMessageBox::information(this, "Ошибка", "Выберите запись для удаления.");
    }
}

void ToothJournal::saveToFile() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("Текстовые файлы (*.txt);;Все файлы (*)"));
    if (filePath.isEmpty()) return;

    // Проверка на формат файла
    if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        QMessageBox::critical(this, "Ошибка", "Неверный формат файла. Выберите файл с расширением .txt.");
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QString date = tableWidget->item(row, 0)->text();
            QString position = tableWidget->item(row, 1)->text();
            QString toothNumber = tableWidget->item(row, 2)->text();
            out << date << '\t' << position << '\t' << toothNumber << '\n';
        }
        file.close();
    }
}

void ToothJournal::loadFromFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("Текстовые файлы (*.txt);;Все файлы (*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split('\t');
            if (fields.size() == 3) {
                int row = tableWidget->rowCount();
                tableWidget->insertRow(row);
                tableWidget->setItem(row, 0, new QTableWidgetItem(fields[0]));
                tableWidget->setItem(row, 1, new QTableWidgetItem(fields[1]));
                tableWidget->setItem(row, 2, new QTableWidgetItem(fields[2]));
            }
        }
        file.close();
    }
}

void ToothJournal::setupUI() {
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({"Дата прорезывания", "Позиция зуба", "Номер зуба"});

    addButton = new QPushButton("Добавить запись", this);
    connect(addButton, &QPushButton::clicked, this, &ToothJournal::addEntry);

    removeButton = new QPushButton("Удалить запись", this);
    connect(removeButton, &QPushButton::clicked, this, &ToothJournal::removeEntry);

    saveButton = new QPushButton("Сохранить в файл", this);
    connect(saveButton, &QPushButton::clicked, this, &ToothJournal::saveToFile);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}
