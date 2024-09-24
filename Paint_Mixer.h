#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Paint_Mixer.h"
#include "Paint_Widget.h"
#include "Project_List_Widget.h"


class Paint_Mixer : public QMainWindow
{
    Q_OBJECT

public:
    Paint_Mixer(QWidget* parent = nullptr);
    ~Paint_Mixer();

private slots:
    void on_Paint_catalog_button_clicked();
    void on_Project_management_button_clicked();
    

private:
    Ui::Paint_MixerClass ui;
    Paint_Widget* Paint_Widget_ui;
    Project_List_Widget* Project_List_Widget_ui;
    

};