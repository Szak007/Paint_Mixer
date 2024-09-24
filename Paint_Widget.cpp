#include "Paint_Widget.h"






Paint_Widget::Paint_Widget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    
    loadColorsFromJSON();

    


    
	ui.Color_list_table_widget->setColumnCount(6);
    QStringList headers = { "Name", "Hex", "Type", "Number of Paints","Description", "Color" };
    ui.Color_list_table_widget->setHorizontalHeaderLabels(headers);



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

        QColor hexColor(QString::fromStdString(color.get_hex()));
        color_item->setBackground(QBrush(hexColor));


        ui.Color_list_table_widget->setItem(c, 0, name_item);
        ui.Color_list_table_widget->setItem(c, 1, hex_item);
        ui.Color_list_table_widget->setItem(c, 2, description_item);
        ui.Color_list_table_widget->setItem(c, 3, type_item);
        ui.Color_list_table_widget->setItem(c, 4, number_of_paints_item);
        ui.Color_list_table_widget->setItem(c, 5, color_item);
        
        c++;
    }

    ui.Color_list_table_widget->setSortingEnabled(true);

    connect(ui.Filter_push_button, &QPushButton::clicked, this, &Paint_Widget::applyFilter);
}

Paint_Widget::~Paint_Widget()
{
    
}




void Paint_Widget::saveColorsToJSON() {
    QString jsonString = "{ \"colors\": [\n";

    for (int i = 0; i < ui.Color_list_table_widget->rowCount(); ++i) {
        QString name = ui.Color_list_table_widget->item(i, 0)->text();
        QString hex = ui.Color_list_table_widget->item(i, 1)->text();
        QString type = ui.Color_list_table_widget->item(i, 2)->text();
        int numberOfPaints = ui.Color_list_table_widget->item(i, 3)->text().toInt();
        QString description = ui.Color_list_table_widget->item(i, 4)->text();
        QString color = "";  

        jsonString += QString("{\n") +
            QString("  \"Name\": \"%1\",\n").arg(name) +
            QString("  \"Hex\": \"%1\",\n").arg(hex) +
            QString("  \"Type\": \"%1\",\n").arg(type) +
            QString("  \"Number of Paints\": %1,\n").arg(numberOfPaints) +
            QString("  \"Description\": \"%1\",\n").arg(description) +
            QString("  \"Color\": \"%1\"\n").arg(color) +
            QString("},\n");
    }

    jsonString.chop(2); 
    jsonString += "\n]}";

    QFile file("colors.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to save JSON file.");
        return;
    }

    QTextStream out(&file);
    out << jsonString;
    file.close();
}



void Paint_Widget::loadColorsFromJSON() {
    QFile file("colors.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open JSON file.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::warning(this, "Error", "Could not open JSON file.");
        return;
    }

    QJsonObject jsonObject = doc.object();
    QJsonArray jsonArray = jsonObject["colors"].toArray();

    ui.Color_list_table_widget->setRowCount(0); 

    for (const QJsonValue& value : jsonArray) {
        QJsonObject rowObject = value.toObject();
        int newRow = ui.Color_list_table_widget->rowCount();
        ui.Color_list_table_widget->insertRow(newRow);

        for (int j = 0; j < ui.Color_list_table_widget->columnCount(); ++j) {
            QString header = ui.Color_list_table_widget->horizontalHeaderItem(j)->text();

            QTableWidgetItem* item = nullptr;
            if (header == "Number of Paints") {
                item = new QTableWidgetItem(QString::number(rowObject[header].toInt()));  
            }
            else {
                item = new QTableWidgetItem(rowObject[header].toString());
            }

            ui.Color_list_table_widget->setItem(newRow, j, item);

            
            if (j == 5) { 
                QString hexValue = rowObject["Hex"].toString();
                if (!hexValue.startsWith("#")) {
                    hexValue.prepend("#");
                }
                QColor hexColor(hexValue);
                item->setBackground(QBrush(hexColor));
            }
        }
    }

    ui.Color_list_table_widget->resizeColumnsToContents(); 
}







void Paint_Widget::applyFilter() {
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




void Paint_Widget::on_Add_paint_button_clicked() {
    QString paint_name = ui.Line_name_of_paint_edit->text();
    QString hexString = ui.Line_hex_edit->text();  

    
    bool ok;
    int hexTest = hexString.toInt(&ok, 16);  
    if (!ok) {
        QMessageBox::warning(this, "Error", "The entered HEX code is invalid.");
        return;
    }

    QString description_name = ui.Line_description_edit->text();
    QString type_name = ui.Line_type_edit->text();
    int number_of_paint_name = ui.Line_number_of_paints_edit->text().toInt();

    bool color_found = false;
    int row_to_update = -1;

    
    for (int i = 0; i < ui.Color_list_table_widget->rowCount(); i++) {
        if (ui.Color_list_table_widget->item(i, 0)->text() == paint_name) {
            row_to_update = i;
            color_found = true;
            break;
        }
    }

    if (color_found) {
        
        ui.Color_list_table_widget->item(row_to_update, 1)->setText("#" + hexString.toUpper());
        ui.Color_list_table_widget->item(row_to_update, 2)->setText(type_name);
        ui.Color_list_table_widget->item(row_to_update, 3)->setText(QString::number(number_of_paint_name));
        ui.Color_list_table_widget->item(row_to_update, 4)->setText(description_name);

        
        QColor hexColor("#" + hexString.toUpper());
        ui.Color_list_table_widget->item(row_to_update, 5)->setBackground(QBrush(hexColor));
    }
    else {
        
        Color new_color(paint_name.toStdString(), hexString.toStdString(), description_name.toStdString(), type_name.toStdString(), number_of_paint_name);

        int new_row = ui.Color_list_table_widget->rowCount();
        ui.Color_list_table_widget->insertRow(new_row);

        ui.Color_list_table_widget->setItem(new_row, 0, new QTableWidgetItem(paint_name));
        ui.Color_list_table_widget->setItem(new_row, 1, new QTableWidgetItem("#" + hexString.toUpper()));
        ui.Color_list_table_widget->setItem(new_row, 2, new QTableWidgetItem(type_name));
        ui.Color_list_table_widget->setItem(new_row, 3, new QTableWidgetItem(QString::number(number_of_paint_name)));
        ui.Color_list_table_widget->setItem(new_row, 4, new QTableWidgetItem(description_name));

        
        QTableWidgetItem* color_item = new QTableWidgetItem();
        QColor hexColor("#" + hexString.toUpper());
        color_item->setBackground(QBrush(hexColor));
        ui.Color_list_table_widget->setItem(new_row, 5, color_item);
    }

    
    ui.Line_name_of_paint_edit->clear();
    ui.Line_hex_edit->clear();
    ui.Line_description_edit->clear();
    ui.Line_type_edit->clear();
    ui.Line_number_of_paints_edit->clear();

    saveColorsToJSON();
}

void Paint_Widget::on_Remove_paint_button_clicked() {
    
    int row = ui.Color_list_table_widget->currentRow();
    if (row >= 0) {
        
        ui.Color_list_table_widget->removeRow(row);
    }
    else {
        
        QMessageBox::warning(this, "Error", "No row selected for deletion.");
    }
    
    saveColorsToJSON();
}

