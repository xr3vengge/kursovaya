#ifndef TOOTHJOURNAL_H
#define TOOTHJOURNAL_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class ToothJournal : public QWidget {
    Q_OBJECT
public:
    ToothJournal(QWidget *parent = nullptr);

private slots:
    void addEntry();
    void removeEntry();
    void saveToFile();
    void loadFromFile();

private:
    void setupUI();

    QTableWidget *tableWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *saveButton;
};

#endif // TOOTHJOURNAL_H
