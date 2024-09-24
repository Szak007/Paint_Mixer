#include "Project_List_Widget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>

Project_List_Widget::Project_List_Widget(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    
    std::list<Project> projects = loadProjectsFromJson("projects.json");

    
    displayProjectsInTable(projects);
    

}

Project_List_Widget::~Project_List_Widget()
{}


void Project_List_Widget::on_Edit_project_button_clicked() {
    int selectedRow = ui.Project_table_widget->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "Selection Error", "No project selected");
        return;
    }

    
    int projectId = ui.Project_table_widget->item(selectedRow, 0)->text().toInt();

    
    std::list<Project> projects = loadProjectsFromJson("projects.json");

    Project selectedProject;
    bool found = false;
    for (const Project& project : projects) {
        if (project.get_id() == projectId) {  
            selectedProject = project;
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Project Error", "Could not find the selected project in JSON.");
        return;
    }

    
    Project_Widget_ui = new Project_Widget(this);
    connect(Project_Widget_ui, &Project_Widget::projectSaved, this, &Project_List_Widget::refreshProjectTable);
    Project_Widget_ui->loadProject(selectedProject);  
    Project_Widget_ui->setWindowTitle("Paint Widget");
    Project_Widget_ui->show();
    on_Subtract_project_button_clicked();
}

void Project_List_Widget::on_Subtract_project_button_clicked() {
    int selectedRow = ui.Project_table_widget->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "Selection Error", "No project selected");
        return;
    }
    
    int projectId = ui.Project_table_widget->item(selectedRow, 0)->text().toInt();
    
    removeProjectFromJson(projectId);

    
    ui.Project_table_widget->removeRow(selectedRow);
}


std::list<Project> Project_List_Widget::loadProjectsFromJson(const QString& filePath) {
    std::list<Project> projects;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open JSON file for reading.");
        return projects;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(this, "File Error", "Invalid JSON format.");
        return projects;
    }

    QJsonArray projectsArray = doc.array();

    for (const QJsonValue& value : projectsArray) {
        QJsonObject projectObject = value.toObject();
        Project project;
        project.set_id(projectObject["Id"].toInt());
        project.set_title(projectObject["Title"].toString().toStdString());
        project.set_description(projectObject["Description"].toString().toStdString());
        project.set_image_path(projectObject["Image_path"].toString().toStdString());

        QJsonArray colorsArray = projectObject["Colors"].toArray();
        std::list<Color> colors;
        for (const QJsonValue& colorValue : colorsArray) {
            QJsonObject colorObject = colorValue.toObject();
            Color color;
            color.set_name(colorObject["Name"].toString().toStdString());
            color.set_hex(colorObject["Hex"].toString().toStdString());
            color.set_description(colorObject["Description"].toString().toStdString());
            color.set_type(colorObject["Type"].toString().toStdString());
            color.set_number_of_paints(colorObject["Number_of_Paints"].toInt());

            colors.push_back(color);
        }
        project.set_colors(colors);

        projects.push_back(project);
    }

    return projects;
}


void Project_List_Widget::displayProjectsInTable(const std::list<Project>& projects) {
    ui.Project_table_widget->setRowCount(static_cast<int>(projects.size()));
    ui.Project_table_widget->setColumnCount(3);  // Trzy kolumny: Id, Title, Description
    QStringList headers = {"Id", "Title", "Description"};
    ui.Project_table_widget->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const Project& project : projects) {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(project.get_id()));
        QTableWidgetItem* titleItem = new QTableWidgetItem(QString::fromStdString(project.get_title()));
        QTableWidgetItem* descriptionItem = new QTableWidgetItem(QString::fromStdString(project.get_description()));

        ui.Project_table_widget->setItem(row, 0, idItem);
        ui.Project_table_widget->setItem(row, 1, titleItem);
        ui.Project_table_widget->setItem(row, 2, descriptionItem);
        

        row++;
    }
    ui.Project_table_widget->setColumnHidden(0, true);
    ui.Project_table_widget->resizeColumnsToContents();
}

void Project_List_Widget::on_Add_new_project_button_clicked() {
    Project_Widget_ui = new Project_Widget(this);
    connect(Project_Widget_ui, &Project_Widget::projectSaved, this, &Project_List_Widget::refreshProjectTable);
    Project_Widget_ui->setWindowTitle("Paint Widget");
    Project_Widget_ui->show();
}

void Project_List_Widget::refreshProjectTable() {
    std::list<Project> projects = loadProjectsFromJson("projects.json");
    displayProjectsInTable(projects);
}

void Project_List_Widget::removeProjectFromJson(int projectId) {
    QFile file("projects.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open JSON file for reading.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(this, "File Error", "Invalid JSON format.");
        return;
    }

    QJsonArray projectsArray = doc.array();
    QJsonArray newProjectsArray;

    
    for (const QJsonValue& value : projectsArray) {
        QJsonObject projectObject = value.toObject();
        if (projectObject["Id"].toInt() != projectId) {
            newProjectsArray.append(projectObject);
        }
    }

   
    QJsonDocument newDoc(newProjectsArray);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open JSON file for writing.");
        return;
    }

    file.write(newDoc.toJson(QJsonDocument::Indented));
    file.close();
}

