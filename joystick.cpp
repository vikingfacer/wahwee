
#include <fcntl.h>
#include <linux/joystick.h>
#include <sstream>
#include <unistd.h>

#include <array>
#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>

#include <zmq.hpp>

#include "controller.hpp"
#include <argparse/argparse.hpp>

bool
read_event(int fd, struct js_event* event);

float
shortToPercent(short s, short cutoff)
{
    float shortMax = static_cast<float>(INT16_MAX);
    float sFloat = static_cast<float>(s);
    float percent = 0;
    if (sFloat != 0) {
        percent = (sFloat / shortMax) * 100;
    }
    if (percent < 0) {
        percent *= -1;
    }
    return percent;
}

std::string
axis_command(short axisVal, short axisMinabs, short posPin, short negPin)
{
    std::stringstream pos_command, neg_command;
    pos_command << "pwm-set " << posPin << " ";
    neg_command << "pwm-set " << negPin << " ";

    if (axisVal < 0) {
        neg_command << shortToPercent(axisVal, axisMinabs);
        pos_command << 0;
    } else {
        pos_command << shortToPercent(axisVal, axisMinabs);
        neg_command << 0;
    }
    pos_command << "\r\n";
    neg_command << "\r\n";

    return pos_command.str() + neg_command.str();
}

void
output_commands(controller::controller con)
{
    /*
     * output each controller dependent command
     * */
    auto ret_value = [](auto p, std::string f_str) {
        if (p.first.compare(f_str) == 0) {
            return true;
        } else {
            return false;
        }
    };
    std::vector<std::pair<std::string, short>>::iterator it_pair =
      std::find_if(con.axis.begin(),
                   con.axis.end(),
                   std::bind(ret_value, std::placeholders::_1, "leftY"));
    std::cout << axis_command(it_pair->second, 100, 11, 10) << std::endl;
    it_pair =
      std::find_if(con.axis.begin(),
                   con.axis.end(),
                   std::bind(ret_value, std::placeholders::_1, "rightY"));
    std::cout << axis_command(it_pair->second, 100, 3, 5) << std::endl;
}

auto
main(int argc, char** argv) -> int
{
    argparse::ArgumentParser program("joystick-service");

    program.add_argument("--device")
      .default_value(std::string("/dev/input/js0"))
      .help("device path of controller");

    program.add_argument("--config")
      .default_value(std::string("./xboxController.yaml"))
      .help("device path of controller");

    program.add_argument("--port")
      .default_value(std::string("9999"))
      .help("Port Bound to for communication to command server");

    program.add_argument("--rate")
      .default_value(5000)
      .help("rate commands are sent (us)")
      .scan<'i', int>();
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }
    // all these values should be checked
    std::string config_file = program.get<std::string>("--config");
    std::string device = program.get<std::string>("--device");
    std::string port = program.get<std::string>("--port");

    int rate = program.get<int>("--rate");

    int js = open(device.c_str(), O_RDONLY | O_NONBLOCK);
    controller::controller control_config(config_file.c_str());

    if (js == -1) {
        perror("Could not open joystick");
    } else {
        zmq::context_t context(1);
        zmq::socket_t socket(context, zmq::socket_type::pub);
        socket.bind("tcp://*:" + port);

        struct js_event event
        {};

        /* This loop will exit if the controller is unplugged. */
        while (true) {
            read_event(js, &event);
            switch (event.type) {
                case JS_EVENT_BUTTON:
                    control_config.updateButton(&event);
                    break;
                case JS_EVENT_AXIS: {
                    control_config.updateAxis(&event);
                } break;
                default:
                    /* Ignore init events. */
                    break;
            }
            output_commands(control_config);
            usleep(rate);
        }
    }
    close(js);
    return 0;
}

/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
bool
read_event(int fd, struct js_event* event)
{
    ssize_t bytes;
    bool success = false;
    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        success = true;

    /* Error, could not read full event. */
    return success;
}
