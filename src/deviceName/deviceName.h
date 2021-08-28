#ifndef DEVICENAME_h
#define DEVICENAME_h

namespace inkes
{
    class DeviceName
    {
    public:
        DeviceName();
        char *get();

    private:
        char deviceName[25] = {'I', 'n', 'k', 'D', 'a', 's', 'h', '-', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    };
}

using inkes::DeviceName;

#endif