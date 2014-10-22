#include <iostream>
#include <gtkmm.h>

#include <boost/filesystem.hpp>
#include <fstream>

#include "httprequest.h"
#include "gui/ocewindow.h"
#include "computerinstance.h"

extern "C"
{
}

using boost::asio::ip::tcp;


int main(int argc, char **argv) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "ml.altenius.ocemulator");

    OCEWindow win;

    /*HttpRequest req;
    req.get("www.google.com", "/");
    
    std::cout << "data: " << req.readLine() << std::endl;*/

    std::string homeDir = getenv("HOME");

    boost::filesystem::create_directories(homeDir + "/.ocemulator");

    // Download kernel.lua

    if (!boost::filesystem::exists(homeDir + "/.ocemulator/kernel.lua")) {
        HttpRequest kernelReq;
        kernelReq.get("www.pastebin.com", "/raw.php?i=eS7N7Mvh"); // TODO: Check if this failed

        std::ofstream kernelFile;
        kernelFile.open(homeDir + "/.ocemulator/kernel.lua");
        kernelFile << kernelReq.readAll();
        kernelFile.close();
    }

    ComputerInstance comp(homeDir + "/.ocemulator", "default", win.getScreenW());

    app->run(win, argc, argv);
    return 0;
}
