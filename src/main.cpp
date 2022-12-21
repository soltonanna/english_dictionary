
//Headers from this project
#include "main_menu.hpp"

// Headers from third party library
#include <QApplication>
#include <QIcon>

// Headers from standard library
#include <string>

bool is_show(int, char**);

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
	QIcon p("res/app.ico");
        QApplication::setWindowIcon(p);
        main_menu* m = new main_menu(0, is_show(argc, argv));
        m->show();
	return a.exec();
}

bool is_show(int c, char** v)
{
        bool t = (c > 1 && 
                "colors" == std::string(v[1]));
        return t;
}
