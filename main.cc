#include "StreamServer.h"
#include "BaseCamera.h"
#include <signal.h>

// usb camera id (default camera is -1)
#define DEFAULT_CAMERA -1
// #define DEFAULT_CAMERA "http://192.168.1.20:8080/video"

std::vector<std::shared_ptr<iit::StreamServer>> server_list;

//! handle  ctrl+c
inline void signal_intrupt_handle(int var)
{
    // Server->stop();
    for (auto &server : server_list)
    {
        server->stop();
    }
}

int main(int argc, char const *argv[])
{
    // handle intrupt signal to elegantly close server.
    signal(SIGINT, signal_intrupt_handle);

    // vector of cameras
    std::vector<std::shared_ptr<BaseCamera>> camera_list;

    int port = 8080;
    // if no args are passed, use default usb camera
    if (argc < 2 || argc > 3)
    {
        camera_list.push_back(std::make_shared<BaseCamera>(DEFAULT_CAMERA, false));
    }
    else if(argc == 2)
    {
//         // add camera for each index passed in command line argument
//         for (int i = 1; i < argc; i++)
//         {
//             camera_list.push_back(std::make_shared<BaseCamera>(atoi(argv[i]), false));
//         }
        camera_list.push_back(std::make_shared<BaseCamera>(atoi(argv[1]), false));
    }
    else if(argc == 3)
    {
        camera_list.push_back(std::make_shared<BaseCamera>(atoi(argv[1]), false));
        port = atoi(argv[2]);
    }

    // create server for each camera and bind it to port 8080,8081 ... so on
    //int port = 8080;
    for (auto &camera : camera_list)
    {
        server_list.push_back(std::make_shared<iit::StreamServer>(port++, camera));
    }

    // wait for server to close properly
    for (auto &server : server_list)
    {
        server->start();
    }

    return 0;
}
