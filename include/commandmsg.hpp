

#include <sstream>
#include <string.h>
#include <string>
#include <vector>

namespace JScmds {

const char subopt[] = "JS ";
class motor_cmd
{
  public:
    motor_cmd(char cmd, short mod, float per = 0)
      : moto_cmd(cmd_struct{ .ch = "JS ", .cmd = cmd, .mod = mod, .per = per }){};

    std::vector<char> serialize()
    {
        char* ptr = (char*)&moto_cmd;
        return std::vector<char>(ptr, ptr + sizeof(moto_cmd));
    }

  private:
    struct cmd_struct
    {
        const char ch[4];
        const char cmd;
        const short mod;
        const float per;
    };
    cmd_struct moto_cmd;
};
}
