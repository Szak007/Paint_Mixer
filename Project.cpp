#include "Project.h"

Project::Project() : id(0)
{
}

Project::Project(const int id, const std::string& title, const std::string& description, const std::list<Color>& colors, const std::string& image_path)
	: id(id), title(title), description(description), colors(colors), image_path(image_path)
{
}




Project::~Project()
{
}

void Project::set_id(int id)
{
	this->id = id;
}

void Project::set_title(std::string title)
{
	this->title = title;
}

void Project::set_description(std::string description)
{
	this->description = description;
}

void Project::set_colors(std::list<Color> colors)
{
	this->colors = colors;
}



void Project::add_color(Color& c)
{
	colors.push_back(c);
}

void Project::remove_color(Color& c)
{
	colors.erase(std::remove(colors.begin(), colors.end(), c), colors.end());
}

int Project::get_id() const
{
	return id;
}

std::string Project::get_title() const
{
	return title;
}

std::string Project::get_description() const
{
	return description;
}

std::list<Color>& Project::get_colors() 
{
	return colors;
}

const std::list<Color>& Project::get_colors() const
{
	return colors;
}

void Project::set_image_path(std::string image_path) {
	this->image_path = image_path;
}

std::string Project::get_image_path() const {
	return image_path;
}