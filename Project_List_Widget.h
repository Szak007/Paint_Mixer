#pragma once

#include <QMainWindow>
#include "ui_Project_List_Widget.h"
#include "Project_Widget.h"
#include "Project.h"

class Project_List_Widget : public QMainWindow
{
    Q_OBJECT

public:
    Project_List_Widget(QWidget* parent = nullptr);
    ~Project_List_Widget();
    

private slots:
    void on_Add_new_project_button_clicked();
    void on_Edit_project_button_clicked();
    void on_Subtract_project_button_clicked();
    void refreshProjectTable();



private:
    std::list<Project> loadProjectsFromJson(const QString& filePath);
    void displayProjectsInTable(const std::list<Project>& projects);
    
    void removeProjectFromJson(int proejctId);
    
    

    Ui::Project_List_WidgetClass ui;
    Project_Widget* Project_Widget_ui;
};
