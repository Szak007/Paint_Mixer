#include "Color.h"

Color::Color()
{
}

Color::Color(std::string name, std::string hex, std::string description, std::string type, int number_of_paints):name(name), hex(hex), description(description), type(type), number_of_paints(number_of_paints)
{
}



Color::~Color()
{
}

void Color::set_id(int id)
{
	this->id = id;
}

void Color::set_name(std::string name)
{
	this->name = name;
}

void Color::set_hex(std::string hex)
{
	this->hex = hex;
}

void Color::set_description(std::string description)
{
	this->description = description;

}

void Color::set_type(std::string type)
{
	this->type = type;
}

void Color::set_number_of_paints(int number_of_paints)
{
	this->number_of_paints = number_of_paints;
}

int Color::get_id() const
{
	return id;
}

std::string Color::get_name() const
{
	return name;
}



std::string Color::get_hex() const
{
	return hex;
}

std::string Color::get_description() const
{
	return description;
}

std::string Color::get_type() const
{
	return type;
}

int Color::get_number_of_paints() const
{
	return number_of_paints;
}

Color Color::operator=(const Color& c)
{
	if (this != &c) {
		id = c.id;
		name = c.name;
		hex = c.hex;
		description = c.description;
		type = c.type;
		number_of_paints = c.number_of_paints;
	}
	return *this;
}

bool Color::operator==(const Color& c) const
{
	return id == c.id && name == c.name && hex == c.hex &&
		description == c.description && type == c.type &&
		number_of_paints == c.number_of_paints;
}



