#pragma once
#include <iostream>
#include <list>
#include "Palette.h"


class Project
{
private:
    int id;
    std::string title; 
    std::string description; 
    
    std::list<Color> colors; 
    std::string image_path; 

public:
    Project();
    Project(const int id, const std::string& title, const std::string& description, const std::list<Color>& colors, const std::string& image_path);
    ~Project();
    void set_id(int id);
    void set_title(std::string title);
    void set_description(std::string description);
    void set_colors(std::list<Color> colors);
    void set_image_path(std::string image_path);
   
    void add_color(Color& c);
    void remove_color(Color& c);
    int get_id() const;
    std::string get_title() const;
    std::string get_description() const;
    std::list<Color>& get_colors();
    const std::list<Color>& get_colors() const;

    std::string get_image_path() const;
};

