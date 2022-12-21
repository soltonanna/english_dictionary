// Headers from this Project
#include "search.hpp"

// Headers from standard library
#include <cassert>
#include <iostream>

// Headers from third party library
#include <QPixmap>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

// Defined macros
#define MIN_WIDTH 640
#define MIN_HEIGHT 270
#define ENTER 16777220

void search::
search_by_word()
{
        emit searched(m_word->text());
}

void search::
keyPressEvent(QKeyEvent* e)
{
        if (Qt::Key_Escape == e->key()) {
                this->close();
        }
        if (ENTER != e->key()) {
                return;
        }
        search_by_word();
}

void search::
setup_connections()
{
        connect(m_search, SIGNAL(clicked()), this, SLOT(search_by_word()));
}

void search::
layouts()
{
        m_vbox = new QVBoxLayout(this);
        QHBoxLayout* h = new QHBoxLayout;
        m_vbox->addWidget(m_title);
        m_vbox->addWidget(m_word);
        h->addStretch(1);
        h->addWidget(m_search);
        m_vbox->addLayout(h);
}

void search::
initialize_widgets()
{
        m_title = new QLabel("Input word", this);
        m_title->setObjectName("info_text");
        m_word = new QLineEdit(this);
        m_search = new QPushButton("Search", this);
        m_search->setObjectName("button");
        m_search->setMinimumSize(70, 32);
}

void search::
to_center_screen()
{
        QRect d = QApplication::desktop()->screenGeometry();
        int h = (d.height() - MIN_HEIGHT) / 2;
        int w = (d.width() - MIN_WIDTH) / 2;
        move(w, h);
        setWindowTitle("Find");
}

search::
search(QWidget* p)
	: QWidget(p)
        , m_title(0)
        , m_word(0)
        , m_search(0)
        , m_vbox(0)
{
        to_center_screen();
	initialize_widgets();
        setup_connections();
        layouts();
}

search::
~search()
{}
