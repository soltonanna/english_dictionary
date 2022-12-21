#ifndef WORDS_HPP
#define WORDS_HPP

// Headers from this project
#include "data_helper.hpp"
#include "input.hpp"
#include "search.hpp"
	
// Headers from third party library
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QStandardItem>

class words : public QWidget
{
	Q_OBJECT
private:
        search* m_search;
	// data
	data_helper* m_helper;
	// table view
	QTableView* m_view;
	// input
	QLabel* m_current_view;
	QLineEdit* m_input;
	// results
	QLabel* m_remains_title;
	QLabel* m_remains;
	QLabel* m_checked_title;
	QLabel* m_checked;
	QLabel* m_true_title;
	QLabel* m_true;
	QLabel* m_false_title;
	QLabel* m_false;
	// controls
	QPushButton* m_select;
	QPushButton* m_insert;
	QPushButton* m_delete;
	QPushButton* m_update;
	input* m_insert_window;
	QSpinBox* m_begin_control;
	QSpinBox* m_end_control;
	QPushButton* m_start;
	// main layout
	QHBoxLayout* m_hbox;
	// Var
	QVector<QString> m_id;
	QVector<QString> m_arm;
	QVector<QString> m_eng;
        QVector<QString> m_selected_id;
	QVector<QString> m_random_id;
	QVector<QString> m_random_arm;
	QVector<QString> m_random_eng;
	QVector<QString> m_false_id;
	QVector<QString> m_false_arm;
	QVector<QString> m_false_eng;
private:
	void initialize_window();
	void to_center_screen();
	QPoint get_center_point();
	void initialize_table_view();
	void initialize_input();
	void initialize_results();
	void initialize_controls();
	void layouts();
	void setup_connections();
	void resize_font();
	void resize_view();
        void resize_min_max();
        void initialize_variables();
private slots:
        void selected_rows(QModelIndex, QModelIndex);
	void recv_inserts(QString, QString);
	void recv_updates(QString, QString);
	void start();
	void show_insert_window();
	void delete_selected_row();
	void update_selected_row();
        void show_values_of_interval();
        void search_words(QString);
//--------------------------------------------------
private:
        QStandardItemModel* m_virtual_model;
        bool m_in_process;
        int m_current_index;
        int m_current_size;
        int m_remains_size;
        int m_checked_size;
        int m_true_size;
        int m_false_size;
        bool m_new;
        bool m_is_random;
private:
        void reset_random_status();
        void set_interval_default_values();
        void initialize_random_words();
        int get_values_of_interval(int, int);
        int get_false_values();
        void show_virtual_model(int);
        void initialize_selected_id();
        void initialize_empty_errors();
        void show_current_word();
        void check_word();
        void set_zero_values();
        void show_results();
        int set_begin_values();
//--------------------------------------------------
protected:
	virtual void resizeEvent(QResizeEvent*);
	virtual void keyPressEvent(QKeyEvent*);
public:
	explicit words(QWidget* p = 0);
	~words();
};

#endif
