#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ui_Project_Widget.h"
#include "Project.h"


class Project_Widget : public QMainWindow
{
    Q_OBJECT

public:
    Project_Widget(QWidget* parent = nullptr);
    ~Project_Widget();

    void loadProject(const Project& project);

signals:
    void projectSaved();  


private slots:
    void on_Add_image_button_clicked();
    void on_Add_project_button_clicked();
    void on_Add_paint_button_clicked();
    void on_Remove_paint_button_clicked();
    void applyFilter();
    

private:
    void set_paint_colors(const std::list<Color>& colors);
    void add_paint_to_project(Color& color);  
    
    void loadColorsFromJSON();
    
    void saveProjectToJson(const Project& project);
    int getLastProjectIdFromJson(const QString& filePath);
    
    
    
    void closeEvent(QCloseEvent* event);


    Ui::Project_WidgetClass ui;
    Project current_project;
    std::list<Color> available_colors;
    int highestColorId = 0;
};
