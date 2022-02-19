/*
 * Jacob Montpetit
 *
 * Description:
 */

#ifndef CONTROLCONFIG_H
#define CONTROLCONFIG_H

#include <yaml-cpp/yaml.h>

#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace controller {

using axisState = std::vector<std::pair<std::string, short>>;
using buttonState = std::vector<std::pair<std::string, bool>>;

struct ConfigException : public std::exception
{
    ConfigException(const char* msg)
      : msg(msg){};
    const char* what() const throw() { return msg; };

  private:
    const char* msg;
};

class controller
{
  public:
    controller(const char* yamlconfig)
    {
        YAML::Node config = YAML::LoadFile(yamlconfig);
        if (config["Type"]) {
            type = config["Type"].as<std::string>();
        } else {
            throw ConfigException("Type Not Defined");
        }
        if (config["Axis"]) {
            for (auto label : config["Axis"].as<std::vector<std::string>>()) {
                axis.push_back(std::pair<std::string, short>{ label, 0 });
            }
        } else {
            throw ConfigException("Axis Not Defined");
        }
        if (config["Button"]) {
            for (auto label : config["Button"].as<std::vector<std::string>>()) {
                button.push_back(std::pair<std::string, bool>{ label, false });
            }
        } else {
            throw ConfigException("Axis Not Defined");
        }
        if (config["Axis Min"]) {
            axisValueMax = config["Axis Min"].as<short>();
        } else {
            throw ConfigException("Axis Min Not Defined");
        }
        if (config["Axis Max"]) {
            axisValueMin = config["Axis Min"].as<short>();
        } else {
            throw ConfigException("Axis Max Not Defined");
        }
    }

    std::vector<std::pair<std::string, bool>>::iterator ButtonByName(
      std::string name)
    {
        auto compare = [](std::pair<std::string, bool> p, std::string f_str) {
            bool found = false;
            if (p.first.compare(f_str) == 0) {
                found = true;
            }
            return found;
        };
        return std::find_if(button.begin(),
                            button.end(),
                            std::bind(compare, std::placeholders::_1, name));
    }

    std::vector<std::pair<std::string, short>>::iterator AxisByName(
      std::string name)
    {
        auto compare = [](std::pair<std::string, short> p, std::string f_str) {
            bool found = false;
            if (p.first.compare(f_str) == 0) {
                found = true;
            }
            return found;
        };
        return std::find_if(axis.begin(),
                            axis.end(),
                            std::bind(compare, std::placeholders::_1, name));
    }

    void updateAxis(struct js_event* event)
    {

        if (event != NULL) {
            axis.at(event->number).second = event->value;
        }
    }

    void updateButton(struct js_event* event)
    {
        if (event != NULL) {
            std::cout << event->number << std::endl;
            button.at(event->number).second = (bool)event->value;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const controller& con)
    {
        os << "Type: " << con.type << '\n';
        os << "Axis:\n";
        int i = 0;
        for (auto ele : con.axis) {
            os << "    " << i++ << ": " << ele.first << " : " << ele.second
               << "\n";
        }
        os << "Max Axis: " << con.axisValueMax << "\n";
        os << "Min Axis: " << con.axisValueMin << "\n";
        return os;
    };
    std::string type;
    axisState axis;
    buttonState button;

    short axisValueMax;
    short axisValueMin;
};
}

#endif /* CONTROLCONFIG_H */
