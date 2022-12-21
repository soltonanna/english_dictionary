// Headers from this Project
#include "main_menu.hpp"

// Headers from standard library
#include <cassert>
#include <iostream>

// Headers from third party library
#include <QPixmap>
#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QRect>
#include <QDebug>

// Defined macros
#define MIN_WIDTH 640
#define MIN_HEIGHT 400
#define MAX_WIDTH 900
#define MAX_HEIGHT 500
#define ENTER 16777220
#define G_WIDTH 300
#define G_HEIGHT 320

void main_menu::
draw_A()
{
        double x1 =    0; double y1 = 300; double z1 =    0;
        double x2 =    0; double y2 = 900; double z2 =    0;
        double x3 =  150; double y3 = 600; double z3 =  150;
        double x4 =  150; double y4 = 600; double z4 = -150;
        double x5 = -150; double y5 = 600; double z5 =  150;
        double x6 = -150; double y6 = 600; double z6 = -150;
        double xx1 = m_project->get_x_projection(x1, y1, z1);
        double xx2 = m_project->get_x_projection(x2, y2, z2);
        double xx3 = m_project->get_x_projection(x3, y3, z3);
        double xx4 = m_project->get_x_projection(x4, y4, z4);
        double xx5 = m_project->get_x_projection(x5, y5, z5);
        double xx6 = m_project->get_x_projection(x6, y6, z6);
        double yy1 = m_project->get_y_projection(x1, y1, z1);
        double yy2 = m_project->get_y_projection(x2, y2, z2);
        double yy3 = m_project->get_y_projection(x3, y3, z3);
        double yy4 = m_project->get_y_projection(x4, y4, z4);
        double yy5 = m_project->get_y_projection(x5, y5, z5);
        double yy6 = m_project->get_y_projection(x6, y6, z6);
        set_line(xx1, yy1, xx3, yy3);
        set_line(xx1, yy1, xx4, yy4);
        set_line(xx1, yy1, xx5, yy5);
        set_line(xx1, yy1, xx6, yy6);
        set_line(xx2, yy2, xx3, yy3);
        set_line(xx2, yy2, xx4, yy4);
        set_line(xx2, yy2, xx5, yy5);
        set_line(xx2, yy2, xx6, yy6);
        set_line(xx3, yy3, xx4, yy4);
        set_line(xx6, yy6, xx4, yy4);
        set_line(xx5, yy5, xx6, yy6);
        set_line(xx3, yy3, xx5, yy5);
}

void main_menu::
set_display_parameters()
{
        double w = G_WIDTH;
        double h = G_HEIGHT;
        w = (w < h ? w : h);
        m_gscene->setSceneRect(0, 0, h - 30, h - 30);
        m_project->set_display_size(w, h);
        m_project->set_distance(700);
        m_project->set_canvas_size(500, 500);
}

void main_menu::
set_line(double a, double b, double c, double d)
{
        a = m_gscene->width()  / 2 - a;
        b = m_gscene->height() / 2 - b;
        c = m_gscene->width()  / 2 - c;
        d = m_gscene->height() / 2 - d;
        QGraphicsLineItem* l = new QGraphicsLineItem(a, b, c, d);
        l->setPen(QPen(QColor(m_style->get_button_def_color())));
        m_ggroup->addToGroup(l);
}

void main_menu::
draw_three_lines(double x0, double y0, double z0, double x00, double y00, double z00)
{
        double xx1 = m_project->get_x_projection(x0,  y0,  z0);
        double yy1 = m_project->get_y_projection(x0,  y0,  z0);
        double xx2 = m_project->get_x_projection(x0,  y00, z0);
        double yy2 = m_project->get_y_projection(x0,  y00, z0);
        double xx3 = m_project->get_x_projection(x00, y0,  z00);
        double yy3 = m_project->get_y_projection(x00, y0,  z00);
        double xx4 = m_project->get_x_projection(x00, y00, z00);
        double yy4 = m_project->get_y_projection(x00, y00, z00);
        set_line(xx1, yy1, xx2, yy2);
        set_line(xx2, yy2, xx4, yy4);
        set_line(xx1, yy1, xx3, yy3);
}


void main_menu::
clear()
{
        foreach( QGraphicsItem *item, 
                m_gscene->items(m_ggroup->boundingRect())) {
                if( item->group() == m_ggroup ) {
                        delete item;
                }
        }
}

void main_menu::
draw_3d(double hex_height, double hex_angles, double hex_r, double x, double z)
{
        double hex_angle_step = 2 * PI / hex_angles;
        double hex_i = 0;
        double hex_x = 0;
        double hex_z = 0;
        double hex_xx = 0;
        double hex_zz = 0;
        while (hex_i < hex_angles) {
                hex_z  = hex_r * cos(hex_i * hex_angle_step);
                hex_x  = hex_r * sin(hex_i * hex_angle_step);
                hex_zz = hex_r * cos((hex_i + 1) * hex_angle_step);
                hex_xx = hex_r * sin((hex_i + 1) * hex_angle_step);
                draw_three_lines(hex_x + x,  hex_height, hex_z + z,
                                 hex_xx + x, 0,          hex_zz + z);
                ++hex_i;
        }
}

void main_menu::
draw()
{
        set_display_parameters();
        static double r = 0;
        r += 0.005;
        m_project->set_camera_position(cos(r) * 4000, 2000, sin(r)*4000);
        m_project->set_camera_angles(PI / 6, 1.5 * PI - r, 0);
        clear();
        const int c = 40;
        static double r_x[c];
        static double r_z[c];
        static double r_h[c];
        static bool r_t[c];
        static bool t = true;
        if (t) {
                double r = 0; 
                for (size_t i = 0; i < c; ++i) {
                        r_x[i] = 1000 * cos(r);
                        r_z[i] = 1000 * sin(r);
                        r += double(2 * PI) / c;
                        r_h[i] = -20 * (rand() % 60) - 20;
                        r_t[i] = (bool)(rand() % 2);
                }
                t = !t;
        }

        for (size_t i = 0; i < c; ++i) {
                if (r_t[i]) {
                        r_h[i] += 20;
                        if (0 == r_h[i]) {
                                r_t[i] = false;
                        }
                } else {
                        r_h[i] -= 20;
                        if (-1200 == r_h[i]) {
                                r_t[i] = true;
                        }
                }
                draw_3d(r_h[i], 1, 30, r_x[i], r_z[i]);
                //      H       C  R   X       Z
        }
        draw_A();
}

//***************************************************

void main_menu::
keyPressEvent(QKeyEvent* e) {
        if (Qt::Key_Escape == e->key()) {
                this->close();
        }
        if (e->key() == Qt::Key_T) {
                m_theme->animateClick();
        }
        if (e->key() == Qt::Key_R) {
                static bool t = m_style->is_enabled_radius();
                t = !t;
                m_style->radius_enabled(t);
                m_style->set_theme(m_style->get_style());
                show_new_style();
                m_style->save();
        }
        if (e->key() != ENTER) {
                return;
        }       
        QWidget* p = QApplication::focusWidget();
        QPushButton* b = static_cast<QPushButton*>(p);
        b->animateClick();
}

void main_menu::
show_new_style()
{
        setStyleSheet(m_style->get_all());
        m_gscene->setBackgroundBrush(QBrush(QColor(m_style->get_back_color()),
                Qt::SolidPattern));
}

void main_menu::
show_helper_window()
{
	m_helper_window= new helper(this);
        m_helper_window->setObjectName("widget");
	m_helper_window->setAttribute(Qt::WA_DeleteOnClose);
	m_helper_window->setWindowFlags(Qt::Dialog);
	m_helper_window->setWindowModality(Qt::WindowModal);
	m_helper_window->show();
}

void main_menu::
show_irregular_verbs_window()
{
	m_irregular_verbs_window = new irregular_verbs(this);
        m_irregular_verbs_window->setObjectName("widget");
	m_irregular_verbs_window->setAttribute(Qt::WA_DeleteOnClose);
	m_irregular_verbs_window->setWindowFlags(Qt::Dialog);
	m_irregular_verbs_window->setWindowModality(Qt::WindowModal);
	m_irregular_verbs_window->show();
}

void main_menu::
show_verbs_window()
{
	m_verbs_window = new verbs(this);
        m_verbs_window->setObjectName("widget");
        m_verbs_window->setAttribute(Qt::WA_DeleteOnClose);
	m_verbs_window->setWindowFlags(Qt::Dialog);
	m_verbs_window->setWindowModality(Qt::WindowModal);
	m_verbs_window->show();
}

void main_menu::
show_tobe_window()
{
	m_tobe_window = new tobe(this);
        m_tobe_window->setObjectName("widget");
	m_tobe_window->setAttribute(Qt::WA_DeleteOnClose);
	m_tobe_window->setWindowFlags(Qt::Dialog);
	m_tobe_window->setWindowModality(Qt::WindowModal);
	m_tobe_window->show();
}

void main_menu::
hide_name() 
{
        m_theme_name->setText("");
        m_name_timer->stop();
}

void main_menu::
change_theme()
{
        m_style->set_next_theme();
        setStyleSheet(m_style->get_all());
        m_style->save();
        if (0 != m_colors) {
                m_colors->set_style(m_style);
        }
        m_gscene->setBackgroundBrush(QBrush(QColor(m_style->get_back_color()),
                Qt::SolidPattern));
        QString n = QString::fromUtf8(m_style->get_style().c_str());
        n.remove(".dtm");
        m_theme_name->setText(n);
        m_name_timer->start(3000);
}

void main_menu::
show_topic_words_window()
{
	m_topic_words_window = new topic_words(this);
        m_topic_words_window->setObjectName("widget");
	m_topic_words_window->setAttribute(Qt::WA_DeleteOnClose);
	m_topic_words_window->setWindowFlags(Qt::Dialog);
	m_topic_words_window->setWindowModality(Qt::WindowModal);
	m_topic_words_window->show();
}

void main_menu::
show_words_window()
{
	m_words_window = new words(this);
        m_words_window->setObjectName("widget");
	m_words_window->setAttribute(Qt::WA_DeleteOnClose);
	m_words_window->setWindowFlags(Qt::Dialog);
	m_words_window->setWindowModality(Qt::WindowModal);
	m_words_window->show();
}

void main_menu::
setup_connections()
{
	connect(m_help, SIGNAL(clicked()), this, SLOT(show_helper_window()));	
	connect(m_words, SIGNAL(clicked()), this, SLOT(show_words_window()));	
	connect(m_topic_words, SIGNAL(clicked()), 
                this, SLOT(show_topic_words_window()));	
	connect(m_verbs, SIGNAL(clicked()), this, SLOT(show_verbs_window()));	
	connect(m_tobe, SIGNAL(clicked()), this, SLOT(show_tobe_window()));	
	connect(m_theme, SIGNAL(clicked()), this, SLOT(change_theme()));	
	connect(m_name_timer, SIGNAL(timeout()), this, SLOT(hide_name()));	
	connect(m_irregular_verbs, SIGNAL(clicked()), 
                this, SLOT(show_irregular_verbs_window()));	
}

void main_menu::
layouts()
{
	QVBoxLayout* v = new QVBoxLayout;
	v->addWidget(m_verbs);
	v->addWidget(m_tobe);
	v->addWidget(m_words);
	v->addWidget(m_topic_words);
	v->addWidget(m_irregular_verbs);
	v->addWidget(m_help);
	assert(0 == m_hbox);	
	m_hbox = new QHBoxLayout(this);
	m_hbox->addWidget(m_gview);
	m_hbox->addLayout(v);	
}

void main_menu::
initialize_graphics()
{
        m_project = new projection; 
        m_gview = new QGraphicsView(this);
        m_gview->setFixedSize(G_WIDTH, G_HEIGHT); 
        m_gscene = new QGraphicsScene(this); 
        m_gscene->setSceneRect(0, 0, G_WIDTH - 30, G_HEIGHT - 30); 
        m_gscene->setBackgroundBrush(QBrush(QColor(m_style->get_back_color()),
                Qt::SolidPattern));
        m_gview->setScene(m_gscene); 
        m_gview->setStyleSheet("border-style: none;");
        m_gview->setRenderHint(QPainter::Antialiasing, true);
        m_ggroup = m_gscene->createItemGroup(m_gscene->selectedItems()); 
        m_timer = new QTimer(this); 
        connect(m_timer, SIGNAL(timeout()), this, SLOT(draw()));
}

void main_menu::
set_size_policy_for_buttons()
{
	m_verbs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_tobe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_words->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_topic_words->setSizePolicy(QSizePolicy::Expanding,
		 QSizePolicy::Expanding);
	m_irregular_verbs->setSizePolicy(QSizePolicy::Expanding,
		 QSizePolicy::Expanding);
	m_help->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void main_menu::
initialize_buttons()
{
	assert(0 == m_verbs);
	assert(0 == m_tobe);
	assert(0 == m_words);
	assert(0 == m_topic_words);
	assert(0 == m_irregular_verbs);
	assert(0 == m_help);
	assert(0 == m_theme);
    m_verbs = new QPushButton("Random sentences (Verbs)", this);
    m_verbs->setObjectName("button");
    m_tobe = new QPushButton("Random sentences, (ToBe)", this);
    m_tobe->setObjectName("button");
    m_words = new QPushButton("My vocabulary", this);
    m_words->setObjectName("button");
    m_topic_words = new QPushButton("Topic vocabulary", this);
    m_topic_words->setObjectName("button");
    m_irregular_verbs = new QPushButton("Irregular verbs", this);
    m_irregular_verbs->setObjectName("button");
    m_help = new QPushButton("Helper tables", this);
    m_help->setObjectName("button");
    m_theme = new QPushButton(this);
    m_theme->setGeometry(5, 5, 22, 22);
    m_theme->setObjectName("theme");
    m_theme->setToolTip("Press 'T' to change theme\nPress 'R' to change radius");
    m_theme_name = new QLabel("", this);
    m_theme_name->setGeometry(40, 5, 250, 18);
    m_theme_name->setObjectName("info_text");
    set_size_policy_for_buttons();
    m_name_timer = new QTimer(this);
}

QPoint main_menu::
get_center_point() {
	QRect d = QApplication::desktop()->screenGeometry();
    int h = (d.height() - MIN_HEIGHT) / 2;
    int w = (d.width() - MIN_WIDTH) / 2;	
    return QPoint(w, h);
}

void main_menu::
to_center_screen()
{
	move(get_center_point());
	setWindowTitle("Dictionary | Main menu");
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    setMaximumSize(MAX_WIDTH, MAX_HEIGHT);
}

void main_menu::
initialize_menu()
{
    setObjectName("widget");
    setStyleSheet(m_style->get_all()); 
    initialize_graphics();
    initialize_buttons();
    layouts();
    to_center_screen();
    m_timer->start(20);
}

main_menu::
main_menu(QWidget* p, bool t)
    : QWidget(p)
	, m_verbs(0)
	, m_tobe(0)
    , m_words(0)
    , m_topic_words(0)
    , m_irregular_verbs(0)
    , m_help(0)
    , m_theme(0)
    , m_hbox(0)
    , m_words_window(0)
    , m_topic_words_window(0)
    , m_verbs_window(0)
    , m_irregular_verbs_window(0)
    , m_tobe_window(0)
    , m_helper_window(0)
    , m_style(0)
    , m_colors(0)
{
    QElapsedTimer tm;
    tm.start();
    QThread::msleep(1200);
    m_style = new css::style();
    initialize_menu();
    setup_connections();
    if (t) {
        m_colors = new colors_window(this); 
        m_colors->setWindowFlags(Qt::Dialog);
        m_colors->set_style(m_style);
        m_colors->show();
        connect(m_colors, SIGNAL(accepted()), 
                this, SLOT(show_new_style()));	
    }
    m_gview->setFocusPolicy(Qt::NoFocus);
}

main_menu::
~main_menu()
{}
