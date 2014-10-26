#include <QApplication>
#include <QMessageBox>
#include <application/application.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const application::Application & app = application::Application::instance();
    app.init();

    if (app.hasErrors())
        QMessageBox::warning(nullptr, QObject::tr("Application loaded with some errors: "),
                             app.errors()->join("\n"), QMessageBox::Ok);

    return a.exec();
}
