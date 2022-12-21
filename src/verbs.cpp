// Headers from this Project
#include "verbs.hpp"

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
#define MIN_WIDTH 640
#define MIN_HEIGHT 370
#define ENTER 16777220
#define F11   16777274

void verbs::
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

void verbs::
keyPressEvent(QKeyEvent* e)
{
        if (Qt::Key_Escape == e->key()) {
                this->close();
        }
        if (e->key() == ENTER) {
                show_random();
        }
        if (e->key() == F11) {
                resize_min_max();
        }
}

void verbs::
resizeEvent(QResizeEvent*)
{
        resize_font();
}

void verbs::
show_random() {
        size_t r = rand() % 63;
        size_t rv = rand() % 2;
        QString j = he_or_she(r % 7);
        QString p = m_pronouns[r % 7];
        QString e = m_pron[r];
        QString v = m_verbs[r][rv];
        QString s = "";
        if (r > 13 && r < 21) {
                s += p + " " + e + " " + v + j;
        } else if (r > 34 && r < 42) {
                s += p + " " + e + " " + v + j;
        } else if (r > 55) {
                s += p + " " + e + " " + v + j;
        } else {
                s += p + " " + v + " " + e + j;
        }
        m_words->setText(s);
        m_current_text = s;
        resize_font();
}

QString verbs::
he_or_she(const int i) 
{
        if (i == 2) {
                return "  ♂ ";
        }
        if (i == 3) {
                return "  ♀";
        }
        return "    ";
}

void verbs::
resize_font()
{
        QString t = "";
        t = "  " + m_current_text + "  ";
        int d = 25 - t.length();
        if (d > 0) {
                for (int i = 0; i < d; ++i) {
                        t += "  ";
                }
        }
        m_words->setText(t);
	QFont f = m_words->font();
	QFontMetrics m(f);
	QSize s = m.size(0, m_words->text());
	float w = m_words->width() / (float)s.width(); 
	float h = m_words->height() / (float)s.height();
	float c = qMin(w, h);
	f.setPointSizeF(std::min(int(f.pointSizeF() * c), 60));
        m_words->setFont(f);
}

void verbs::
setup_connections()
{
        connect(m_random, SIGNAL(clicked()), this, SLOT(show_random()));
}

void verbs::
layouts()
{
	QHBoxLayout* h = new QHBoxLayout;
	h->addStretch(1);
	h->addWidget(m_random);
	assert(0 == m_vbox);	
	m_vbox = new QVBoxLayout(this);
	m_vbox->addWidget(m_words);
	m_vbox->addLayout(h);	
}

void verbs::
initialize_words()
{
	assert(0 == m_words);
	m_words = new QLabel(this);
	m_words->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_words->setObjectName("text_region");
	QFont f("Times", 35, QFont::Bold, true);
        m_words->setFont(f);
        m_words->setAlignment(Qt::AlignCenter);
}

void verbs::
initialize_random()
{
	assert(0 == m_random);
	m_random = new QPushButton("New", this);
	m_random->setFixedSize(120, 60);
	m_random->setObjectName("button");
}

void verbs::
to_center_screen()
{
	QRect d = QApplication::desktop()->screenGeometry();
        int h = (d.height() - MIN_WIDTH) / 2;
        int w = (d.width() - MIN_HEIGHT) / 2;	
        move(w, h);
}

void verbs::
initialize_menu()
{
	setWindowTitle("Dictionary | Random sentences");
        setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	to_center_screen();
	initialize_words();
	initialize_random();
	layouts();
	setup_connections();
}

verbs::
verbs(QWidget* p)
	: QWidget(p)
	, m_words(0)
	, m_random(0)
	, m_vbox(0)
{
        srand(time(0));
	initialize_menu();
}

verbs::
~verbs()
{}
