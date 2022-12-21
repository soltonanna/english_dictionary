// Headers from this Project
#include "input.hpp"

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
void input::
cancel()
{
        this->close();
}

void input::
keyPressEvent(QKeyEvent* e)
{
        if (Qt::Key_Escape == e->key()) {
                this->close();
        }
        if (ENTER != e->key()) {
                return;
        }
        if (m_cancel->hasFocus()) {
                m_cancel->animateClick();
        } else {
                m_insert->animateClick();
        }
}

void input::
set_topic_type(const bool  t)
{
        m_is_topic = t;
        size_t s = (m_is_topic) ? 100 : 0;
        setMinimumSize(MIN_WIDTH, MIN_HEIGHT + s);
        setMaximumHeight(MIN_HEIGHT + s);
        if (t) {
                m_type_title->setVisible(true);
                m_type->setVisible(true);
                return;
        }
        m_type_title->setVisible(false);
        m_type->setVisible(false);
}

void input::
set_insert_type(const bool  t)
{
        m_is_insert = t;
        if (t) {
                m_insert->setText("Add");
        } else {
                m_insert->setText("Update");
        }
}

void input::
set_topic_values(QString a, QString b, int c) 
{
        m_arm->setText(a);
        m_eng->setText(b);
        m_type->setCurrentIndex(c);
}

void input::
set_values(QString a, QString b) 
{
        m_arm->setText(a);
        m_eng->setText(b);
}

void input::
send_topic_input()
{
        QString a = m_arm->text();
        QString b = m_eng->text();
        QString c = QString::number(m_type->currentIndex() + 1);
        if ("" != a && "" != b) {
                m_arm->setText("");
                m_eng->setText("");
                if (m_is_insert) {
                        emit words_topic_inserted(a, b, c);
                } else {
                        emit words_topic_updated(a, b, c);
                        this->close();
                }
        } else {
                QMessageBox m;
                m.setText("inputs is empty!");
                m.setWindowTitle("Invalid inputs");
                m.exec();
        }
}

void input::
send_input()
{
        if (m_is_topic) {
                send_topic_input();
                return;
        }
        QString a = m_arm->text();
        QString b = m_eng->text();
        if ("" != a && "" != b) {
                m_arm->setText("");
                m_eng->setText("");
                if (m_is_insert) {
                        emit words_inserted(a, b);
                } else {
                        emit words_updated(a, b);
                        this->close();
                }
        } else {
                QMessageBox m;
                m.setText("inputs is empty!");
                m.setWindowTitle("Invalid inputs");
                m.exec();
        }
}

void input::
setup_connections()
{
        connect(m_insert, SIGNAL(clicked()), this, SLOT(send_input()));
        connect(m_cancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

void input::
layouts()
{
        m_vbox = new QVBoxLayout(this);
	QHBoxLayout* h1 = new QHBoxLayout;
	QHBoxLayout* h2 = new QHBoxLayout;
	QHBoxLayout* h3 = new QHBoxLayout;
	QHBoxLayout* h4 = new QHBoxLayout;
        
        h1->addWidget(m_arm_title);
        h1->addStretch(1);
        h2->addWidget(m_eng_title);
        h2->addStretch(1);
        h4->addWidget(m_type_title);
        h4->addStretch(1);
        h3->addStretch(1);
        h3->addWidget(m_cancel);
        h3->addWidget(m_insert);
        m_vbox->addWidget(m_arm);
        m_vbox->addLayout(h1);
        m_vbox->addWidget(m_eng);
        m_vbox->addLayout(h2);
        m_vbox->addWidget(m_type);
        m_vbox->addLayout(h4);
        m_vbox->addLayout(h3);
        
}

void input::
initialize_arm()
{
	assert(0 == m_arm_title);
	assert(0 == m_arm);
        QFont t("Times", 15, QFont::Bold, false);
	m_arm_title = new QLabel("Armenian", this);
        m_arm_title->setObjectName("info_text");
        m_arm_title->setFont(t);
        m_arm = new QLineEdit(this);
        QFont f("Times", 20, QFont::Bold, true);
        m_arm->setMinimumHeight(50);
        m_arm->setFont(f);
}

void input::
initialize_eng()
{
	assert(0 == m_eng_title);
	assert(0 == m_eng);
        QFont t("Times", 15, QFont::Bold, false);
	m_eng_title = new QLabel("English", this);
        m_eng_title->setObjectName("info_text");
        m_eng_title->setFont(t);
        m_eng = new QLineEdit(this);
        QFont f("Times", 25, QFont::Bold, true);
        m_eng->setMinimumHeight(50);
        m_eng->setFont(f);
}

void input::
initialize_type()
{
        assert(0 == m_type);
        m_type = new QComboBox(this);
        m_type->setEditable(false);
        m_type->insertItem(0, "Animals");
        m_type->insertItem(1, "Thing");
        m_type->insertItem(2, "Family");
        m_type->insertItem(3, "Food");
        m_type->insertItem(4, "Phenomenon");
        m_type->insertItem(5, "Expressions");
        m_type->insertItem(6, "Country");
        m_type->insertItem(7, "Body");
        m_type->insertItem(8, "Numerals");
        m_type->insertItem(9, "IT");
        m_type->insertItem(10, "Regular");
        m_type->setMinimumHeight(50);
        assert(0 == m_type_title);
        QFont f("Times", 15, QFont::Bold, true);
        m_type_title = new QLabel("Type", this);
        m_type_title->setObjectName("info_text");
        m_type_title->setFont(f);
        m_type->setFont(f);
}

void input::
initialize_insert()
{
        assert(0 == m_insert);
        assert(0 == m_cancel);
        m_insert = new QPushButton("Add", this);
        m_insert->setFixedSize(120, 50);
        m_insert->setObjectName("button");
        m_cancel = new QPushButton("Cancel", this);
        m_cancel->setFixedSize(120, 50);
        m_cancel->setObjectName("button");
}

void input::
to_center_screen()
{
	QRect d = QApplication::desktop()->screenGeometry();
        int h = (d.height() - MIN_HEIGHT) / 2;
        int w = (d.width() - MIN_WIDTH) / 2;	
        move(w, h);
}

void input::
initialize_input()
{
	setWindowTitle("Input the word");
        setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
        setMaximumHeight(MIN_HEIGHT);
	to_center_screen();
	initialize_arm();
	initialize_eng();
	initialize_type();
	initialize_insert();
	layouts();
	setup_connections();
}

input::
input(QWidget* p)
	: QWidget(p)
	, m_arm_title(0)
	, m_arm(0)
	, m_eng_title(0)
	, m_eng(0)
	, m_insert(0)
	, m_cancel(0)
	, m_vbox(0)
        , m_type_title(0)
        , m_type(0)
        , m_is_topic(false)
{
	initialize_input();
        set_topic_type(false);
}

input::
~input()
{}
