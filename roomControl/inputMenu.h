#pragma once

#include <QtWidgets/QWidget>
#include "WorkWithCamera/QtConnect.h"

#include "ui_inputMenu.h"

class inputMenu : public QWidget
{
    Q_OBJECT

public:
    inputMenu(QWidget *parent = nullptr);
    ~inputMenu();

private:
    Ui::inputMenu ui;
    QtConnect* connectCamera{};
private slots:
    void slot_openMainMenu();
};
