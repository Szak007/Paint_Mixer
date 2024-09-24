#pragma once

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

#include "ui_Paint_Widget.h"
#include "Project_Widget.h"

class Paint_Widget : public QMainWindow
{
    Q_OBJECT

public:
    Paint_Widget(QWidget* parent = nullptr);
    ~Paint_Widget();

    

private slots:
    void on_Add_paint_button_clicked();
    void applyFilter();
    void on_Remove_paint_button_clicked();
    

private:
    
    void saveColorsToJSON();
    void loadColorsFromJSON();


    Ui::Paint_WidgetClass ui;
    Project_Widget* project_widget;  
    std::list<Color> colors;  
};