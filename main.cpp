#include <QApplication>
#include <QMessageBox>
#include <application/application.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    application::Application & app = application::Application::instance();
    app.init();

    QMessageBox errorBox(QMessageBox::Warning, "Loading problems",
                         app.errors()->join("\n"), QMessageBox::Ok);
    if (app.hasErrors())
        errorBox.show();

    return a.exec();
}
