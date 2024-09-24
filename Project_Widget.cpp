#include "Project_Widget.h"

Project_Widget::Project_Widget(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    
    loadColorsFromJSON();

    

    ui.Color_list_table_widget->setSortingEnabled(true);
    

    connect(ui.Filter_push_button, &QPushButton::clicked, this, &Project_Widget::applyFilter);
}

Project_Widget::~Project_Widget()
{}

void Project_Widget::set_paint_colors(const std::list<Color>& colors)
{
    available_colors = colors;
    ui.Color_list_table_widget->setRowCount(static_cast<int>(colors.size()));
    ui.Color_list_table_widget->setColumnCount(6);

    int c = 0;
    for (const Color& color : colors) {
        QTableWidgetItem* name_item = new QTableWidgetItem(QString::fromStdString(color.get_name()));
        QString hexValue = QString::fromStdString(color.get_hex()).toUpper();
        QTableWidgetItem* hex_item = new QTableWidgetItem("#" + hexValue);
        QTableWidgetItem* description_item = new QTableWidgetItem(QString::fromStdString(color.get_description()));
        QTableWidgetItem* type_item = new QTableWidgetItem(QString::fromStdString(color.get_type()));
        QTableWidgetItem* number_of_paints_item = new QTableWidgetItem(QString::number(color.get_number_of_paints()));
        QTableWidgetItem* color_item = new QTableWidgetItem();

        if (!hexValue.startsWith("#")) {
            hexValue.prepend("#");
        }

        QColor hexColor(hexValue);
        color_item->setBackground(QBrush(hexColor));

        ui.Color_list_table_widget->setItem(c, 0, name_item);
        ui.Color_list_table_widget->setItem(c, 1, hex_item);
        ui.Color_list_table_widget->setItem(c, 2, type_item);
        ui.Color_list_table_widget->setItem(c, 3, number_of_paints_item);
        ui.Color_list_table_widget->setItem(c, 4, description_item);
        ui.Color_list_table_widget->setItem(c, 5, color_item);
        c++;
    }

    ui.Color_list_table_widget->setSortingEnabled(true);

}

void Project_Widget::add_paint_to_project(Color& color)
{
    ui.Paint_table_widget->setSortingEnabled(false);
    current_project.add_color(color);

    QString paintName = QString::fromStdString(color.get_name());
    QString hexValue = QString::fromStdString(color.get_hex()).toUpper();
    QString description = QString::fromStdString(color.get_description());
    QString type = QString::fromStdString(color.get_type());
    int numberOfPaints = color.get_number_of_paints();

    int new_row = ui.Paint_table_widget->rowCount();
    ui.Paint_table_widget->insertRow(new_row);

    ui.Paint_table_widget->setItem(new_row, 0, new QTableWidgetItem(paintName));
    ui.Paint_table_widget->setItem(new_row, 1, new QTableWidgetItem(hexValue));
    ui.Paint_table_widget->setItem(new_row, 2, new QTableWidgetItem(type));
    ui.Paint_table_widget->setItem(new_row, 3, new QTableWidgetItem(QString::number(numberOfPaints)));
    ui.Paint_table_widget->setItem(new_row, 4, new QTableWidgetItem(description));
    QTableWidgetItem* color_item = new QTableWidgetItem();
    QColor hexColor(hexValue);
    color_item->setBackground(QBrush(hexColor));
    ui.Paint_table_widget->setItem(new_row, 5, color_item);

    
    ui.Paint_table_widget->setSortingEnabled(true);
}




void Project_Widget::on_Add_image_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!filename.isEmpty()) {
        QImage image;
        bool valid = image.load(filename);
        if (valid) {
            ui.Label_image->setPixmap(QPixmap::fromImage(image));
            std::string path = filename.toStdString();
            current_project.set_image_path(path);
        }
        else {
            QMessageBox::warning(this, "Image failed", "Try again with new image");
        }
    }
}


void Project_Widget::on_Add_project_button_clicked()
{
    QString title = ui.Line_title_of_project->text();
    QString description = ui.Plain_text_description_edit->toPlainText();

    if (title.isEmpty() || description.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Title and description cannot be empty");
        return;
    }

    int lastId = getLastProjectIdFromJson("projects.json");
    int projectId = lastId + 1;

    std::list<Color> colors = current_project.get_colors();
    std::string imagePath = current_project.get_image_path();

    current_project = Project(projectId, title.toStdString(), description.toStdString(), colors, imagePath);
    
    
    saveProjectToJson(current_project);

    emit projectSaved();

    this->close();
    
    
}




void Project_Widget::on_Add_paint_button_clicked()
{
    int row = ui.Color_list_table_widget->currentRow();
    if (row >= 0) {
        QString paintName = ui.Color_list_table_widget->item(row, 0)->text();
        QString hexValue = ui.Color_list_table_widget->item(row, 1)->text();
        QString type = ui.Color_list_table_widget->item(row, 2)->text();
        int numberOfPaints = ui.Color_list_table_widget->item(row, 3)->text().toInt();
        QString description = ui.Color_list_table_widget->item(row, 4)->text();
        
        

        Color selected_color(paintName.toStdString(), hexValue.toStdString(), description.toStdString(), type.toStdString(), numberOfPaints);
        add_paint_to_project(selected_color);
    }
    else {
        QMessageBox::warning(this, "Selection Error", "No paint selected");
    }
}


void Project_Widget::on_Remove_paint_button_clicked()
{
    int row = ui.Paint_table_widget->currentRow();
    if (row >= 0) {
        
        ui.Paint_table_widget->removeRow(row);
    }
    else {
        
        QMessageBox::warning(this, "Error", "No row selected for deletion.");
    }
    

}

void Project_Widget::applyFilter() {
    QString filterText = ui.lineEdit->text();
    QString filterColumn = ui.comboBox->currentText();

    int column = -1;
    if (filterColumn == "Name") {
        column = 0;
    }
    else if (filterColumn == "Hex") {
        column = 1;
    }
    else if (filterColumn == "Type") {
        column = 2;
    }
    else if (filterColumn == "Number of Paints") {
        column = 3;
    }
    else if (filterColumn == "Description") {
        column = 4;
    }
    
    else if (filterColumn == "Color") {
        column = 5;
    }

    if (column == -1) return;

    for (int i = 0; i < ui.Color_list_table_widget->rowCount(); ++i) {
        bool match = false;
        if (column == 5) {
            
            QColor cellColor = ui.Color_list_table_widget->item(i, column)->background().color();
            match = cellColor.name().contains(filterText, Qt::CaseInsensitive);
        }
        else {
            match = ui.Color_list_table_widget->item(i, column)->text().contains(filterText, Qt::CaseInsensitive);
        }
        ui.Color_list_table_widget->setRowHidden(i, !match);
    }

    
}

int Project_Widget::getLastProjectIdFromJson(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "File Error", "Could not open JSON file.");
        return 0; 
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        return 0; 
    }

    QJsonArray projectsArray = doc.array();

    int lastId = 0;
    for (int i = 0; i < projectsArray.size(); ++i) {
        QJsonObject project = projectsArray[i].toObject();
        lastId = qMax(lastId, project["Id"].toInt()); 
    }
    return lastId;
}




void Project_Widget::loadColorsFromJSON() {
    QFile file("colors.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open JSON file.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::warning(this, "Error", "Unable to open JSON file.");
        return;
    }

    QJsonObject jsonObject = doc.object();
    QJsonArray jsonArray = jsonObject["colors"].toArray();

    ui.Color_list_table_widget->setRowCount(0); 

    for (const QJsonValue& value : jsonArray) {
        QJsonObject rowObject = value.toObject();
        int newRow = ui.Color_list_table_widget->rowCount();
        ui.Color_list_table_widget->insertRow(newRow);

        
        QTableWidgetItem* nameItem = new QTableWidgetItem(rowObject["Name"].toString());
        QTableWidgetItem* hexItem = new QTableWidgetItem(rowObject["Hex"].toString());
        QTableWidgetItem* typeItem = new QTableWidgetItem(rowObject["Type"].toString());
        QTableWidgetItem* numberOfPaintsItem = new QTableWidgetItem(QString::number(rowObject["Number of Paints"].toInt()));
        QTableWidgetItem* descriptionItem = new QTableWidgetItem(rowObject["Description"].toString());

        
        QString hexValue = rowObject["Hex"].toString();
        if (!hexValue.startsWith("#")) {
            hexValue.prepend("#");
        }
        QColor hexColor(hexValue);
        QTableWidgetItem* colorItem = new QTableWidgetItem();
        colorItem->setBackground(QBrush(hexColor));

        
        ui.Color_list_table_widget->setItem(newRow, 0, nameItem);
        ui.Color_list_table_widget->setItem(newRow, 1, hexItem);
        ui.Color_list_table_widget->setItem(newRow, 2, typeItem);
        ui.Color_list_table_widget->setItem(newRow, 3, numberOfPaintsItem);
        ui.Color_list_table_widget->setItem(newRow, 4, descriptionItem);
        ui.Color_list_table_widget->setItem(newRow, 5, colorItem);
    }

    
    ui.Color_list_table_widget->resizeColumnsToContents(); 
}



void Project_Widget::saveProjectToJson(const Project& project)
{
    
    QFile file("projects.json");
    QJsonDocument doc;

    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = file.readAll();
        doc = QJsonDocument::fromJson(jsonData);
        file.close();
    }

    
    QJsonArray projectsArray;
    if (!doc.isNull() && doc.isArray()) {
        projectsArray = doc.array();
    }
    else if (!doc.isNull() && doc.isObject()) {
        
        projectsArray.append(doc.object());
    }

    
    QJsonObject projectObject;
    projectObject["Id"] = project.get_id();
    projectObject["Title"] = QString::fromStdString(project.get_title());
    projectObject["Description"] = QString::fromStdString(project.get_description());
    projectObject["Image_path"] = QString::fromStdString(project.get_image_path());

    
    QJsonArray colorsArray;
    for (const Color& color : project.get_colors()) {
        QJsonObject colorObject;
        colorObject["Name"] = QString::fromStdString(color.get_name());
        colorObject["Hex"] = QString::fromStdString(color.get_hex());
        colorObject["Type"] = QString::fromStdString(color.get_type());
        colorObject["Description"] = QString::fromStdString(color.get_description());
        colorObject["Number_of_Paints"] = color.get_number_of_paints();

        colorsArray.append(colorObject);
    }

    projectObject["Colors"] = colorsArray;

    

    bool projectUpdated = false;

    
    for (int i = 0; i < projectsArray.size(); ++i) {
        QJsonObject existingProject = projectsArray[i].toObject();
        if (existingProject["Id"].toInt() == project.get_id()) {
            
            projectsArray[i] = projectObject;
            projectUpdated = true;
            break;
        }
    }

    
    if (!projectUpdated) {
        projectsArray.append(projectObject);
    }

    
    QJsonDocument newDoc(projectsArray);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open JSON file for writing.");
        return;
    }

    file.write(newDoc.toJson(QJsonDocument::Indented)); 
    file.close();

    
    emit projectSaved();
}








void Project_Widget::loadProject(const Project& project) {
    current_project = project;

    
    ui.Line_title_of_project->setText(QString::fromStdString(project.get_title()));
    ui.Plain_text_description_edit->setPlainText(QString::fromStdString(project.get_description()));

    
    QString imagePath = QString::fromStdString(project.get_image_path());
    if (!imagePath.isEmpty()) {
        QImage image;
        if (image.load(imagePath)) {
            ui.Label_image->setPixmap(QPixmap::fromImage(image));
        }
    }

    
    ui.Paint_table_widget->setRowCount(static_cast<int>(project.get_colors().size()));
    ui.Paint_table_widget->setColumnCount(6);  

    int row = 0;
    for (const Color& color : project.get_colors()) {
        ui.Paint_table_widget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(color.get_name())));
        ui.Paint_table_widget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(color.get_hex())));
        ui.Paint_table_widget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(color.get_type())));
        ui.Paint_table_widget->setItem(row, 3, new QTableWidgetItem(QString::number(color.get_number_of_paints())));
        ui.Paint_table_widget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(color.get_description())));

        
        QColor hexColor(QString::fromStdString(color.get_hex()));
        QTableWidgetItem* colorItem = new QTableWidgetItem();
        colorItem->setBackground(hexColor);
        ui.Paint_table_widget->setItem(row, 5, colorItem);

        row++;
    }

    
    loadColorsFromJSON();
}

void Project_Widget::closeEvent(QCloseEvent* event)
{
    
    QString title = ui.Line_title_of_project->text();
    if (title.isEmpty()) {
        event->accept();
    }
    else {
        saveProjectToJson(current_project);
        event->accept();
    }
}

