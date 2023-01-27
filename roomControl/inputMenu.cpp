#include "inputMenu.h"

inputMenu::inputMenu(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    connect(ui.pb_Ok, &QPushButton::clicked, this, &inputMenu::slot_openMainMenu);
}

inputMenu::~inputMenu()
{}

void inputMenu::slot_openMainMenu()
{
    connectCamera = new QtConnect{};
    connectCamera->show();
    close();
}