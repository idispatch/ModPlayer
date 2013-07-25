#include <bb/cascades/Application>
#include "applicationui.hpp"

Q_DECL_EXPORT int main(int argc, char **argv) {
    bb::cascades::Application app(argc, argv);
    new ApplicationUI(&app);
    return bb::cascades::Application::exec();
}
