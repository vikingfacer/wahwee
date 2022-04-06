#include <iostream>
#include <wiringPi.h>
#include <zmq.hpp>

#include <argparse/argparse.hpp>

auto
main(int argc, char** argv) -> int
{
    argparse::ArgumentParser program("joystick-service");

    program.add_argument("--addr")
      .default_value(std::string("127.0.0.1"))
      .help("Address connected to to receive commands");

    program.add_argument("--port")
      .default_value(std::string("9999"))
      .help("Port connected to to receive commands");

    program.parse_args(argc, argv);

    zmq::context_t context(1);

    zmq::socket_t socket(context, zmq::socket_type::sub);
    socket.connect("tcp://" + program.get<std::string>("--addr") +
                   program.get<std::string>("--port"));
    std::string opt = "jscomms";
    socket.set(zmq::sockopt::subscribe, opt);

    for (;;) {
        zmq::message_t msg;
        (void)socket.recv(msg);
        std::cout << "received: " << msg.to_string() << "\n";
        // dissect message
        // add do stuff
    }
    return 0;
}
