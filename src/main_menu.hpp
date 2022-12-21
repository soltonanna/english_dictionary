#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

// Headers from this project
#include "words.hpp"
#include "topic_words.hpp"
#include "verbs.hpp"
#include "tobe.hpp"
#include "irregular_verbs.hpp"
#include "style.hpp"
#include "colors_window.hpp"
#include "helper.hpp"
#include "projection.hpp"

// Headers from third party library
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>

class main_menu : public QWidget
{
	Q_OBJECT
private:
	QLabel* m_image;
	QPushButton* m_verbs;
	QPushButton* m_tobe;
	QPushButton* m_words;
	QPushButton* m_topic_words;
	QPushButton* m_irregular_verbs;
	QPushButton* m_help;
	QPushButton* m_theme;
	QHBoxLayout* m_hbox;
	words* m_words_window;
	topic_words* m_topic_words_window;
	verbs* m_verbs_window;
	irregular_verbs* m_irregular_verbs_window;
	tobe* m_tobe_window;
        helper* m_helper_window;
        css::style* m_style;
        colors_window* m_colors;
        QLabel* m_theme_name;
        QTimer* m_name_timer;
private:
        QGraphicsView* m_gview;
        QGraphicsScene* m_gscene;
        QGraphicsItemGroup* m_ggroup;
        QTimer* m_timer;
        projection* m_project;
        void draw_3d(double, double, double, double, double);   
        void clear();
        void draw_three_lines(double, double, double, double, double, double);
        void set_line(double, double, double, double);
        void set_display_parameters();
private:
	void initialize_menu();
	void to_center_screen();
	QPoint get_center_point();
	void initialize_buttons();
	void set_size_policy_for_buttons();
	void initialize_graphics();
	void layouts();
	void setup_connections();
private slots:
        void hide_name();
        void draw();
        void draw_A();
        void show_words_window();
	void show_verbs_window();
	void show_tobe_window();
	void show_irregular_verbs_window();
	void show_topic_words_window();
	void show_helper_window();
	void change_theme();
        void show_new_style();
	virtual void keyPressEvent(QKeyEvent*);
public:
public:
	explicit main_menu(QWidget* p = 0, bool t = 0);
	~main_menu();
};

#endif
