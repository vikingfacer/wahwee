

#include <sstream>
#include <string>

namespace JScmds {

std::string subopt{ "JScomms" };
struct cmd
{
    std::string channel;
    std::string cmd;
    short pin;
    float percent;
};

std::string
tomsg(std::string cmd, short pin, float percent)
{
    std::stringstream cmdmsg;
    cmdmsg << JScmds::subopt << cmd << pin << " " << percent << "\n";
    return cmdmsg.str();
};

cmd
fromMsg(std::string msg)
{
    std::stingstream msgstream(msg);
    JScmds::cmd cmd;
    msgstream >> cmd.channel;
    msgstream >> cmd.cmd;
    msgstream >> cmd.pin;
    msgstream >> cmd.percent;
    return cmd;
};

}
