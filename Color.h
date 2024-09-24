#pragma once
#include <iostream>
#include <QJsonObject>


class Color
{
private:
	int id; 
	std::string name;
	
	std::string hex;
	std::string description;
	std::string type;
	int number_of_paints;
public:
	Color();
	
	Color(std::string name, std::string hex, std::string description, std::string type, int number_of_paints);
	~Color();
	void set_id(int id);
	void set_name(std::string name);
	
	void set_hex(std::string hex);
	void set_description(std::string description);
	void set_type(std::string type);
	void set_number_of_paints(int number_of_paints);
	int get_id() const;
	std::string get_name() const;
	
	std::string get_hex() const;
	std::string get_description() const;
	std::string get_type() const;
	int get_number_of_paints() const;

	Color operator=(const Color& c);
	bool operator==(const Color& c) const;


	QJsonObject toJson() const {
		QJsonObject json;
		json["Name"] = QString::fromStdString(name);
		json["Hex"] = QString::fromStdString(hex);
		json["Type"] = QString::fromStdString(type);
		json["Number of Paints"] = number_of_paints;
		json["Description"] = QString::fromStdString(description);
		json["Color"] = ""; 
		return json;
	}


	void fromJson(const QJsonObject& json) {
		id = json["id"].toInt();
		name = json["Name"].toString().toStdString();
		hex = json["Hex"].toString().toStdString();
		description = json["Description"].toString().toStdString();
		type = json["Type"].toString().toStdString();
		number_of_paints = json["Number of Paints"].toInt();  
	}


};

