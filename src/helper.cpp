// Headers from this Project
#include "helper.hpp"

// Headers from standard library
#include <cassert>
#include <iostream>

// Headers from third party library
#include <QPixmap>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QKeyEvent>

// Defined macros
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32)&&!defined(__CYGWIN__)
#define TABLES_PATH "\\res\\table"
#else
#define TABLES_PATH "res/table"
#endif

#define MIN_WIDTH 640
#define MIN_HEIGHT 370
#define MAX_WIDTH 2000
#define MAX_HEIGHT 1600
#define BORDER_SIZE 2
#define MAX_TABLES 4
#define ENTER 16777220
#define F11   16777274

void helper::
resize_min_max()
{
	QRect d = QApplication::desktop()->screenGeometry();
	int h = d.height();
	int w = d.width();	
	static bool t = false;
	if (t) {
		resize(MIN_WIDTH, MIN_HEIGHT);
		to_center_screen();
	} else {
		resize(w, h);
	}
	t = !t;
}

void helper::
keyPressEvent(QKeyEvent* e)
{
	if (Qt::Key_Escape == e->key()) {
		this->close();
	}
	if (F11 == e->key()) {
		resize_min_max();
		return;
	}
	if (e->key() == ENTER || e->key() == Qt::Key_Right) {
		m_next->animateClick();
	}
	if (e->key() == Qt::Key_Left) {
		m_current_index -= 2;
		if (-1 == m_current_index) {
			m_current_index = MAX_TABLES - 1;
		}
		show_next_table();
	}
}

void helper::
resizeEvent(QResizeEvent*)
{
	resize_image();
	m_image->setStyleSheet("background-color: #fff;");
}

void helper::
show_next_table() {
	++m_current_index;
	if (MAX_TABLES + 1 == m_current_index) {
		m_current_index = 1;
	}
	resize_image();
	this->setFocus();
}

void helper::
resize_image()
{
	size_t w = m_image->width() - BORDER_SIZE;
	size_t h = m_image->height() - BORDER_SIZE;
	QString p = TABLES_PATH; 
	p += QString::number(m_current_index);
	p += ".jpg";
	QPixmap i(p);
	if (i.isNull()) {
		return;
	}
	if ((size_t)i.width() > w || (size_t)i.height() > h) {
		m_image->setPixmap(i.scaled(w, h, 
					Qt::KeepAspectRatio, Qt::SmoothTransformation));
		m_previous_width = w;
		m_previous_height = h;
		return;
	}
	if (m_previous_width < w || m_previous_height < h) {
		m_image->setPixmap(i);
		return;
	}
	m_image->setPixmap(i);
}

void helper::
setup_connections()
{
	connect(m_next, SIGNAL(clicked()), this, SLOT(show_next_table()));
}

void helper::
layouts()
{
	QHBoxLayout* h = new QHBoxLayout;
	h->addStretch(1);
	h->addWidget(m_next);
	assert(0 == m_vbox);	
	m_vbox = new QVBoxLayout(this);
	m_vbox->addWidget(m_image);
	m_vbox->addLayout(h);	
}

void helper::
initialize_image()
{
	assert(0 == m_image);
	m_image = new QLabel(this);
	m_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_image->setObjectName("text_region");
	m_image->setAlignment(Qt::AlignCenter);
	QFont f("Times", 35, QFont::Bold, true);
	m_image->setFont(f);
}

void helper::
initialize_next()
{
	assert(0 == m_next);
	m_next = new QPushButton("Next", this);
	m_next->setFixedSize(120, 60);
	m_next->setObjectName("button");
}

void helper::
to_center_screen()
{
	QRect d = QApplication::desktop()->screenGeometry();
	int h = (d.height() - MIN_WIDTH) / 2;
	int w = (d.width() - MIN_HEIGHT) / 2;	
	move(w, h);
}

void helper::
initialize_menu()
{
	setWindowTitle("Dictionary | Helper Tables");
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	setMaximumSize(MAX_WIDTH, MAX_HEIGHT);
	to_center_screen();
	initialize_image();
	initialize_next();
	layouts();
	//setup_connections();
	//this->setFocus();
}

helper::
helper(QWidget* p)
	: QWidget(p)
	, m_image(0)
	, m_next(0)
	, m_vbox(0)
	, m_current_index(1)
	, m_previous_width(0)
	  , m_previous_height(0)
{
	initialize_menu();
}

helper::
~helper()
{}
