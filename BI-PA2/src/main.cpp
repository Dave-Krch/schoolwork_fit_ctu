#include <iostream>
#include "CApp.h"

/**
 *
 *
 * @param argc
 * @param argv 2 optional parameters, path to save files and path to new game files
 * @return
 */

int main(int argc, char* argv[]) {
    try{
        while (true) {
            CApp app(argc, argv);
            if(!app.run()) break;
        }
    }
    catch (const std::exception & exc) {
        std::cout << exc.what() << std::endl;
    }
}