#include "Paint_Mixer.h"

Paint_Mixer::Paint_Mixer(QWidget* parent)
    : QMainWindow(parent)
{
   
    ui.setupUi(this);
    QPixmap pix("main_menu_graphic.png");
    QPixmap scaledPixmap = pix.scaled(ui.Label_main_graphic->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.Label_main_graphic->setPixmap(scaledPixmap);

    
}

Paint_Mixer::~Paint_Mixer()
{}

void Paint_Mixer::on_Paint_catalog_button_clicked()
{

    Paint_Widget_ui = new Paint_Widget(this);
    Paint_Widget_ui->setWindowTitle("Paint Widget");
    Paint_Widget_ui->show();
    


}


void Paint_Mixer::on_Project_management_button_clicked()
{
    Project_List_Widget_ui = new Project_List_Widget(this);
    Project_List_Widget_ui->setWindowTitle("Paint Widget");
    Project_List_Widget_ui->show();
    
}

