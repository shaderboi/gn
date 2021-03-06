#include "example_lib.h"

GnExampleApp* GnExampleApp::g_app = nullptr;

int main(int argc, const char* argv[])
{
    if (GnExampleApp::g_app == nullptr)
        return -1;

    if (!GnExampleApp::g_app->Init())
        return -1;

    return GnExampleApp::g_app->Run();
}
