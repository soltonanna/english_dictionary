// Headers from this Project
#include "words.hpp"
#include "utils/string.hpp"

// Headers from standard library
#include <cassert>
#include <iostream>

// Headers from third party library
#include <QPixmap>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QVector>
#include <QMessageBox>
#include <QScrollBar>
#include <QKeyEvent>
#include <QTableWidget>
#include <QHeaderView>

// Defined macros
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32)&&!defined(__CYGWIN__)
        #define WORDS_DATABASE_PATH "\\data\\words.db"
#else
        #define WORDS_DATABASE_PATH "./data/words.db"
#endif

#define MIN_WIDTH 1150
#define MIN_HEIGHT 600
#define DATABASE_TABLE_NAME "vocabulary"
#define ENTER 16777220
#define F11   16777274

void words::
reset_random_status()
{
        m_is_random = !m_is_random;
        QString s = m_is_random ? "enabled" : "disabled";
        QMessageBox m;
        m.setText("Random checking is " + s);
        m.exec();
}

void words::
search_words(QString s)
{
        m_random_arm.clear();
        m_random_eng.clear();
        m_random_id.clear();
        m_helper->select_by_word(s, m_random_arm, m_random_eng, m_random_id);
        set_zero_values();
        m_input->setEnabled(false);
        m_insert->setEnabled(true);
        m_current_view->setText("");
        m_input->setText("");
        m_in_process = false;
        show_virtual_model(m_random_arm.size());
        QItemSelectionModel* m = m_view->selectionModel();
        connect(m, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                        this, SLOT(selected_rows(QModelIndex, QModelIndex)));
        //m_search->close();
}

void words::
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

void words::
recv_updates(QString a, QString e)
{
        m_helper->update_by_id(m_selected_id[0], a, e);
        initialize_variables();
}

void words::
show_results()
{
        m_remains->setText(QString::number(m_remains_size));
        m_checked->setText(QString::number(m_checked_size));
        m_true->setText(QString::number(m_true_size));
        m_false->setText(QString::number(m_false_size));
}

void words::
set_zero_values()
{
        m_in_process = true;
        m_input->setEnabled(true); 
        m_insert->setEnabled(false); 
        m_current_index = 0;
        m_current_size = m_random_arm.size();
        m_remains_size = m_current_size;
        m_checked_size = 0;
        m_true_size = 0;
        m_false_size = 0;
}

void words::
check_word()
{
        std::string tmp_i = m_input->text().toUtf8().constData();
        StringUtils::eraseFirstAndLastProbels(tmp_i);
        QString i = QString::fromStdString(tmp_i);

        QString w = m_current_view->text();
        QString g = m_random_eng[m_current_index];
        ++m_checked_size;
        --m_remains_size;
        g.replace('`', '\'');

        if (g == i) {
                ++m_true_size;
        } else {
                QStandardItem* a = new QStandardItem(w);
                QStandardItem* b = new QStandardItem(g);
                QStandardItem* c = new QStandardItem(i);
                m_virtual_model->setItem(m_false_size, 1, a); 
                m_virtual_model->setItem(m_false_size, 2, b); 
                m_virtual_model->setItem(m_false_size, 3, c); 
                m_view->setModel(m_virtual_model);
                m_false_id.push_back(m_random_id[m_current_index]);
                m_false_arm.push_back(m_random_arm[m_current_index]);
                m_false_eng.push_back(m_random_eng[m_current_index]);
                ++m_false_size;
                resize_view();
                m_view->scrollToBottom();
        }
        show_results();
        if (0 == m_remains_size) {
                m_in_process = false;
                m_input->setEnabled(false);
                if (m_false_id.size() > 0) {
                        QMessageBox::StandardButton r;
                        r = QMessageBox::question(this, "End",
                                        "Continue ?",
                                        QMessageBox::Yes | QMessageBox::No);
                        if (QMessageBox::Yes == r) {
                                m_new = false;
                                start();
                                return;
                        }
                }
                m_new = true;
                m_current_view->setText("End...");
                m_input->setText("");
                return;
        }
        ++m_current_index;
        show_current_word();
}

void words::
keyPressEvent(QKeyEvent* e)
{
        if (Qt::Key_Escape == e->key()) {
            if (m_in_process) {
                QMessageBox::StandardButton r;
                r = QMessageBox::question(this,
                        "Warning", "The process will"
                        " be rejected!",
                        QMessageBox::Yes | QMessageBox::No);
                if (QMessageBox::No == r) {
                    return;
                }
            }
            this->close();
        }
        if ((e->key() == Qt::Key_R) && 
                        (e->modifiers().testFlag(Qt::ControlModifier))) {
                reset_random_status();
        }
	if ((e->key() == Qt::Key_F) && 
                        (e->modifiers().testFlag(Qt::ControlModifier))) {
                m_search->show();
        }
        if (F11 == e->key()) {
                resize_min_max();
        }
        if (ENTER != e->key()) {
                return;
        }
        bool is_start = (m_begin_control->hasFocus() || 
                        m_end_control->hasFocus() ||
                        m_start->hasFocus());
        if (is_start) {
                m_start->animateClick();
                return;
        }
        if (m_input->hasFocus()) {
                check_word();
                return;
        }
        QWidget* p = QApplication::focusWidget();
        QPushButton* b = static_cast<QPushButton*>(p);
        b->animateClick();
}

void words::
show_current_word()
{
        m_current_view->setText(m_random_arm[m_current_index].replace('`', '\''));
        resize_font();
        m_input->setText("");
        m_input->setFocus();
}

void words::
initialize_empty_errors()
{
        m_virtual_model->clear();
        QStringList h;
        h.append("id");
        h.append("arm");
        h.append("eng");
        h.append("err");
        m_virtual_model->setHorizontalHeaderLabels(h);
        m_view->setModel(m_virtual_model);
        m_view->setColumnHidden(0, true);
        resize_view();
}

void words::
show_virtual_model(int s)
{
        m_virtual_model->clear();
        m_virtual_model->setRowCount(s);
        QStringList h;
        QStringList v;
        h.append("id");
        h.append("arm");
        h.append("eng");
        m_virtual_model->setHorizontalHeaderLabels(h);
        for (int i = 0; i < s; ++i) {
               QStandardItem* j = new QStandardItem(m_random_id[i]);
               QStandardItem* a = new QStandardItem(m_random_arm[i]);
               QStandardItem* b = new QStandardItem(m_random_eng[i]);
               m_virtual_model->setItem(i, 0, j); 
               m_virtual_model->setItem(i, 1, a); 
               m_virtual_model->setItem(i, 2, b); 
               v.append(QString::number(i + m_begin_control->value()));
        }
        m_virtual_model->setVerticalHeaderLabels(v);
        m_view->setModel(m_virtual_model);
        m_view->setColumnHidden(0, true);
        resize_view();
}

void words::
show_values_of_interval() 
{
        if (m_in_process) {
                QMessageBox::StandardButton r;
                r = QMessageBox::question(this,
                                "Warning", "The process will"
                                " be rejected!",
                                QMessageBox::Yes | QMessageBox::No);
                if (QMessageBox::No == r) {
                        return;
                }
        }
        if (0 == m_helper->get_row_count()) {
	        QFont f = m_current_view->font();
                f.setPointSize(46);
                m_current_view->setFont(f);
                m_current_view->setText("No words!");
                return;
        }
        set_zero_values();
        m_input->setEnabled(false);
        m_insert->setEnabled(true);
        m_current_view->setText("");
        m_input->setText("");
        m_in_process = false;
        int s = get_values_of_interval(m_begin_control->value(),
                m_end_control->value());
        show_virtual_model(s);
        QItemSelectionModel* m = m_view->selectionModel();
        connect(m, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                this, SLOT(selected_rows(QModelIndex, QModelIndex)));
}

int words::
get_false_values()
{
        m_random_id.clear();
        m_random_arm.clear();
        m_random_eng.clear();
        for (int i = 0; i < m_false_id.size(); ++i) {
                assert(i < m_false_id.size());
                assert(i < m_false_arm.size());
                assert(i < m_false_eng.size());
                m_random_id.push_back(m_false_id[i]);
                m_random_arm.push_back(m_false_arm[i]);
                m_random_eng.push_back(m_false_eng[i]);
        }
        return m_false_id.size();
}

int words::
get_values_of_interval(int b, int e)
{
        m_random_id.clear();
        m_random_arm.clear();
        m_random_eng.clear();
        for (int i = b - 1; i < e; ++i) {
                assert(i < m_id.size());
                assert(i < m_arm.size());
                assert(i < m_eng.size());
                m_random_id.push_back(m_id[i]);
                m_random_arm.push_back(m_arm[i]);
                m_random_eng.push_back(m_eng[i]);
        }
        return e - b + 1;
}

void words::
initialize_random_words()
{
        int s = 0;
        if (m_new) {
                s = get_values_of_interval(m_begin_control->value(),
                        m_end_control->value());
        } else {
                s = get_false_values();
        }
        m_false_id.clear();
        m_false_arm.clear();
        m_false_eng.clear();
        int f = s;
        if (!m_is_random) {
                return;
        }
        for (int i = 0; i < s; ++i) {
                int  j = rand() % f;
                std::swap(m_random_arm[j], m_random_arm[f - 1]);
                std::swap(m_random_eng[j], m_random_eng[f - 1]);
                --f;
        }
}

void words::
start()
{
        if (0 == m_helper->get_row_count()) {
	        QFont f = m_current_view->font();
                f.setPointSize(46);
                m_current_view->setFont(f);
                m_current_view->setText("No words!");
                return;
        }
        if (0 != set_begin_values()) {
                return;
        }
        if (m_arm.size() < m_helper->get_row_count()) {
                initialize_variables();       
        }
        m_delete->setEnabled(false);
        m_update->setEnabled(false);
        m_input->setEnabled(false);
        initialize_random_words();
        initialize_empty_errors();
        set_zero_values();
        show_results();
        show_current_word();
        resize_view();
}

int words::
set_begin_values()
{
        int e = m_end_control->value();
        int b = m_begin_control->value();
        if (b > e) {
                QMessageBox::StandardButton r;
                r = QMessageBox::question(this, "Invalid interval",
                                "Initial index is bigger than end index!\n So this initial index will be '1'",
                                QMessageBox::Yes | QMessageBox::No);
                if (QMessageBox::No == r) {
                        return -1;
                }
                m_begin_control->setValue(1);
        }
        return 0;
}

void words::
set_interval_default_values() 
{
        m_view->scrollToBottom();
        int n = m_helper->get_row_count();
        m_begin_control->setRange(1, n);
        m_begin_control->setValue(1);
        m_end_control->setRange(1, n);
        m_end_control->setValue(n);
}

void words::
update_selected_row()
{
        initialize_selected_id();
        QMessageBox::StandardButton r;
        r = QMessageBox::question(this, "Update",
                "Do you want to edit this row",
                QMessageBox::Yes | QMessageBox::No);
        if (QMessageBox::No == r) {
                return;
        }
        m_delete->setEnabled(false);
        m_update->setEnabled(false);
        m_insert_window = new input(this);
        m_insert_window->setObjectName("widget");
	m_insert_window->setAttribute(Qt::WA_DeleteOnClose);
	m_insert_window->setWindowFlags(Qt::Dialog);
	m_insert_window->setWindowModality(Qt::WindowModal);
	connect(m_insert_window, SIGNAL(words_updated(QString, QString)), 
                this, SLOT(recv_updates(QString, QString)));	
        m_insert_window->set_values(m_helper->get_arm(m_selected_id[0]),
                m_helper->get_eng(m_selected_id[0]));
        m_insert_window->set_insert_type(false);        
        m_insert_window->set_topic_type(false);        
        m_insert_window->show();

}

void words::
initialize_selected_id()
{
        QModelIndexList s = m_view->selectionModel()->selectedRows();
        m_selected_id.clear();
        if (s.count() > 0) {
                for (int i = 0; i < s.count(); ++i) {
                        int r = s.at(i).row();
                        QModelIndex m = m_view->model()->index(r, 0);
                        m_selected_id.push_back(m.data().toString());
                }
        }
}

void words::
delete_selected_row()
{
        initialize_selected_id();
        QString m = "Do you want to delete " + 
                QString::number(m_selected_id.size()) + " rows?";
        QMessageBox::StandardButton r;
        r = QMessageBox::question(this, "Delete", m,
                QMessageBox::Yes | QMessageBox::No);
        if (QMessageBox::No == r) {
                m_view->setFocus();
                return;
        }
        m_delete->setEnabled(false);
        m_update->setEnabled(false);
        m_helper->delete_by_id(m_selected_id);        
        initialize_variables();
        set_interval_default_values();
}

void words::
selected_rows(QModelIndex , QModelIndex)
{
        m_delete->setEnabled(true);
        m_update->setEnabled(true);
}

void words::
recv_inserts(QString a, QString b)
{
        m_helper->insert(a, b);
        set_interval_default_values();
        m_view->scrollToBottom();
        initialize_variables();
}

void words::
show_insert_window()
{
        m_delete->setEnabled(false);
        m_update->setEnabled(false);
	m_insert_window = new input(this);
        m_insert_window->setObjectName("widget");
	m_insert_window->setAttribute(Qt::WA_DeleteOnClose);
	m_insert_window->setWindowFlags(Qt::Dialog);
	m_insert_window->setWindowModality(Qt::WindowModal);
	connect(m_insert_window, SIGNAL(words_inserted(QString, QString)), 
                this, SLOT(recv_inserts(QString, QString)));	
        m_insert_window->set_insert_type(true);        
        m_insert_window->set_topic_type(false);        
	m_insert_window->show();
}

void words::
initialize_variables()
{
        m_helper->connect_to_table_view(m_view);
	m_id.clear();
	m_arm.clear();
	m_eng.clear();
	m_id = m_helper->get_column(0);
	m_arm = m_helper->get_column(1);
	m_eng = m_helper->get_column(2);
}

void words::
resize_font()
{
        QString t = "";
        if (m_in_process) {
                t = m_random_arm[m_current_index];
        }
        m_current_view->setText(t);
	QFont f = m_current_view->font();
	QFontMetrics m(f);
	QSize s = m.size(0, m_current_view->text());
	float w = m_current_view->width() / (float)s.width(); 
	float h = m_current_view->height() / (float)s.height();
	float c = qMin(w, h);
	f.setPointSizeF(std::min(int(f.pointSizeF() * c), 30));
	m_current_view->setFont(f);
}

void words::
resize_view() 
{
        for (int i = 0; i < m_view->horizontalHeader()->count(); ++i)
        {
                m_view->horizontalHeader()->setSectionResizeMode(
                                i, QHeaderView::Stretch);
        }
}

void words::
resizeEvent(QResizeEvent*)
{
	resize_font();
        resize_view();
}

void words::
setup_connections()
{
        QItemSelectionModel* m = m_view->selectionModel();
        connect(m, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                this, SLOT(selected_rows(QModelIndex, QModelIndex)));
	connect(m_select, SIGNAL(clicked()), 
                this, SLOT(show_values_of_interval()));	
	connect(m_start, SIGNAL(clicked()), this, SLOT(start()));	
	connect(m_insert, SIGNAL(clicked()), this, SLOT(show_insert_window()));	
	connect(m_delete, SIGNAL(clicked()), 
                this, SLOT(delete_selected_row()));	
	connect(m_update, SIGNAL(clicked()), 
                this, SLOT(update_selected_row()));	
	connect(m_search, SIGNAL(searched(QString)),
                this, SLOT(search_words(QString)));	
}

void words::
layouts()
{
	assert(0 == m_hbox);
	m_hbox = new QHBoxLayout(this);
	QVBoxLayout* v1 = new QVBoxLayout;
	QVBoxLayout* v2 = new QVBoxLayout;
	QVBoxLayout* v3 = new QVBoxLayout;
	QVBoxLayout* v4 = new QVBoxLayout;
	QHBoxLayout* h1 = new QHBoxLayout;
	QHBoxLayout* h2 = new QHBoxLayout;
	QHBoxLayout* h3 = new QHBoxLayout;
	v1->addWidget(m_remains_title);
	v1->addWidget(m_checked_title);
	v1->addWidget(m_true_title);
	v1->addWidget(m_false_title);		
	v1->addStretch(1);
	v2->addWidget(m_remains);
	v2->addWidget(m_checked);
	v2->addWidget(m_true);
	v2->addWidget(m_false);
	v2->addStretch(1);
	h1->addLayout(v1);
	h1->addStretch(1);
	h1->addLayout(v2);
	h1->addStretch(4);
	v3->addWidget(m_current_view);
	v3->addWidget(m_input);
	v3->addLayout(h1);
	h2->addWidget(m_select);
	h2->addWidget(m_insert);
	h2->addWidget(m_update);
	h2->addWidget(m_delete);
	h2->addStretch(1);
	h3->addWidget(m_begin_control);
	h3->addWidget(m_end_control);
	v4->addLayout(h3);
	v4->addStretch(1);
	QWidget* w = new QWidget;
	w->setMaximumHeight(70);
	w->setLayout(h2);
	h2->addLayout(v4);
	h2->addWidget(m_start);
	v3->addWidget(w);
        QVBoxLayout* vl = new QVBoxLayout;
        vl->addWidget(m_view);
	m_hbox->addLayout(vl);
	m_hbox->addLayout(v3);
}

void words::
initialize_table_view()
{
	assert(0 == m_view);
	m_view = new QTableView(this);
	m_view->setMinimumWidth(550);
	m_view->setMaximumWidth(750);
        m_virtual_model = new QStandardItemModel;
	m_helper = new data_helper(this);
	m_helper->setup_database(DATABASE_TABLE_NAME, WORDS_DATABASE_PATH);
	m_helper->connect_to_table_view(m_view);
        m_view->setColumnHidden(0, true);
        m_view->setEditTriggers(QTableWidget::NoEditTriggers);
        m_view->setFocusPolicy(Qt::NoFocus);
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void words::
initialize_input()
{
        assert(0 == m_current_view);
	assert(0 == m_input);
	QFont f("Times", 35, QFont::Bold, true);
	m_current_view = new QLabel("", this);
	m_current_view->setMinimumHeight(200);
        m_current_view->setAlignment(Qt::AlignCenter);
        m_current_view->setSizePolicy(QSizePolicy::Expanding, 
                QSizePolicy::Expanding);
	m_current_view->setFont(f);
	m_current_view->setObjectName("text_region");
	m_input = new QLineEdit("", this);
	m_input->setMinimumHeight(70);
	m_input->setFont(f);
        m_input->setEnabled(false);
}

void words::
initialize_results()
{
	assert(0 == m_remains_title);
	assert(0 == m_remains);
	assert(0 == m_checked_title);
	assert(0 == m_checked);
	assert(0 == m_true_title);
	assert(0 == m_true);
	assert(0 == m_false_title);
	assert(0 == m_false);
	m_remains_title = new QLabel("  Remains: ", this);
	m_remains = new QLabel("....", this);
	m_checked_title = new QLabel("  Checked: ", this);
	m_checked = new QLabel("....", this);
	m_true_title = new QLabel("  True: ", this);
	m_true = new QLabel("....", this);
	m_false_title = new QLabel("  False: ", this);
	m_false = new QLabel("....", this);
	QFont f("Times", 15, QFont::Bold, false);
        m_checked_title->setFont(f);
        m_checked->setFont(f);
        m_remains_title->setFont(f);
        m_remains->setFont(f);
        m_true_title->setFont(f);
        m_true->setFont(f);
        m_false_title->setFont(f);
        m_false->setFont(f);
	m_remains_title->setObjectName("info_text");
	m_checked_title->setObjectName("info_text");
	m_true_title->setObjectName("info_text");
	m_false_title->setObjectName("info_text");
	m_remains->setObjectName("info_text");
	m_checked->setObjectName("info_text");
	m_true->setObjectName("info_text");
	m_false->setObjectName("info_text");
}

void words::
initialize_controls() {
	assert(0 == m_select);
	assert(0 == m_insert);
	assert(0 == m_delete);
	assert(0 == m_begin_control);
	assert(0 == m_end_control);
	assert(0 == m_start);
        m_select = new QPushButton("Show", this);
	m_select->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_select->setObjectName("button");
        m_insert = new QPushButton("Add", this);
	m_insert->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_insert->setObjectName("button");
	m_delete = new QPushButton("Delete", this);
        m_delete->setEnabled(false);
	m_delete->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_delete->setObjectName("button");
        m_update = new QPushButton("Edit", this);
	m_update->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_update->setObjectName("button");
        m_update->setEnabled(false);
	m_delete->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_begin_control = new QSpinBox(this);
	m_end_control = new QSpinBox(this);
        m_start = new QPushButton("Start", this);
	m_start->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_start->setMinimumWidth(80);
        m_start->setObjectName("button");
        m_delete->setMinimumWidth(60);
        m_select->setMinimumWidth(80);
        m_update->setMinimumWidth(60);
        m_insert->setMinimumWidth(60);
        m_begin_control->setMinimumHeight(30);
	m_end_control->setMinimumHeight(30);
}

QPoint words::
get_center_point() {
	QRect d = QApplication::desktop()->screenGeometry();
        int h = (d.height() - MIN_HEIGHT) / 2;
        int w = (d.width() - MIN_WIDTH) / 2;	
	return QPoint(w, h);
}

void words::
to_center_screen()
{
        move(get_center_point());
	setWindowTitle("Dictionary | My vocabulary");
        setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
}

void words::
initialize_window()
{
	to_center_screen();
	initialize_table_view();
	initialize_input();
	initialize_results();
	initialize_controls();
}

words::
words(QWidget* p)
	: QWidget(p)
        , m_search(0)
	, m_helper(0)
	, m_view(0)
	, m_current_view(0)
	, m_input(0)
	, m_remains_title(0)
	, m_remains(0)
	, m_checked_title(0)
	, m_checked(0)
	, m_true_title(0)
	, m_true(0)
	, m_false_title(0)
	, m_false(0)
	, m_select(0)
	, m_insert(0)
	, m_delete(0)
	, m_insert_window(0)
	, m_begin_control(0)
	, m_end_control(0)
	, m_start(0)
	, m_hbox(0)
        , m_in_process(false)
        , m_new(true)
        , m_is_random(true)
{
        srand(time(0));
	initialize_window();
	layouts();
        
        m_search = new search(this);
        m_search->setObjectName("widget");
	m_search->setWindowFlags(Qt::Dialog);
	m_search->setWindowModality(Qt::WindowModal);
	
        setup_connections();
        set_interval_default_values();
        initialize_variables();
        m_view->scrollToTop();
        
}

words::
~words()
{
        delete m_helper;
}
