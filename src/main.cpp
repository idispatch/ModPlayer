#include <bb/cascades/Application>
#include "applicationui.hpp"

Q_DECL_EXPORT int main(int argc, char **argv) {
    QCoreApplication::setOrganizationName("Kosenkov");
    QCoreApplication::setOrganizationDomain("kosenkov.ca");
    QCoreApplication::setApplicationName("ModPlayer");

    bb::cascades::Application app(argc, argv);
    new ApplicationUI(&app);
    return bb::cascades::Application::exec();
}
