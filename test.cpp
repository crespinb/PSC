#include "serverManager.h"
#include "headtrackerManager.h"
#include <iostream>
#include <Windows.h>

#include "JuceHeader.h"

int main()
{
    ServerManager *mgr = new ServerManager();
    if(mgr->startUp())
    {
        std::cout << "started up on port " << mgr->getPort() << std::endl;
    }
    else
    {
        std::cout << "failed to start up" << std::endl;
    }
    /*if(mgr->waitForClient())
    {
        std::cout << "got client!" << std::endl;
    }
    else
    {
        std::cout << "failed to get client" << std::endl;
    }
    delete mgr;
    _sleep(5000);*/
    HeadtrackerManager *hMgr = new HeadtrackerManager();
    hMgr->startUp();
    for(int i = 0; i < 3; ++i)
    {
        if(hMgr->isConnected())
        {
            std::cout << "connexion au headtracker établie" << std::endl;
        }
        else
        {
            std::cout << "headtracker non trouvé, nouvel essai dans 5 secondes" << std::endl;
        }
        Sleep(5000);
    }
    return 0;
}