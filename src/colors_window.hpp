#ifndef COLORS_WINDOW
#define COLORS_WINDOW

// Headers from standard library
#include <cassert>

// Headers from third party library
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QApplication>
#include <QLineEdit>

// Headers from this project
#include "style.hpp"

class colors_window : public QWidget
{
        Q_OBJECT
private:
        QLabel* m_title[10];
        QPushButton* m_color[10];
        QPushButton* m_save;
        QHBoxLayout* m_hbox;
        css::style* m_style;
        QLineEdit* m_name;
signals:
        void accepted();
private slots:
        void get_color()
        {
                QWidget* p = QApplication::focusWidget();
                QPushButton* b = static_cast<QPushButton*>(p);
                QColorDialog d(this);
                QColor c = d.getColor(QColor(b->text()), 0);
                d.setStyleSheet("");
                b->setStyleSheet("background-color:" + c.name() + ";");
                b->setText(c.name());
                accept();
        }

        void create()
        {
                QString t;
                if ("" != m_name->text()) {
                        t = "themes/" + m_name->text() + ".dtm";
                } else {
                        t = "themes/New Theme.dtm";
                }
                std::ofstream f;
                f.open(t.toUtf8().constData());
                for (int i = 0; i < 10; ++i) {
                        f << m_color[i]->text().toUtf8().constData()    
                                << std::endl;
                }
                f.close();
        }

        void accept()
        {
                m_style->set_colors(
                        m_color[0]->text(), 
                        m_color[1]->text(),
                        m_color[2]->text(),
                        m_color[3]->text(),
                        m_color[4]->text(),
                        m_color[5]->text(),
                        m_color[6]->text(),
                        m_color[7]->text(),
                        m_color[8]->text(),
                        m_color[9]->text()
                );
                assert(0 != m_style);
                m_style->initialize_css_styles();
                emit accepted();
        }
public:
        colors_window(QWidget* p = 0)
                : QWidget(p)
        {
                setWindowTitle("Dictionary | Theme creator" );
                for (int i = 0; i < 10; ++i) {
                        m_color[i] = new QPushButton(this);
                        m_color[i]->setObjectName("button");
	                m_color[i]->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
                        connect(m_color[i], SIGNAL(clicked()), 
                                this, SLOT(get_color())); 
                }
                m_name = new QLineEdit(this);
                m_save = new QPushButton("Save", this);
                connect(m_save, SIGNAL(clicked()), this, SLOT(create())); 
                m_title[0] = new QLabel("back_color", this); 
                m_title[1] = new QLabel("button_default", this); 
                m_title[2] = new QLabel("button_disabled", this); 
                m_title[3] = new QLabel("button_hover", this); 
                m_title[4] = new QLabel("button_pressed", this); 
                m_title[5] = new QLabel("button_text", this); 
                m_title[6] = new QLabel("border", this); 
                m_title[7] = new QLabel("text", this); 
                m_title[8] = new QLabel("text_back", this); 
                m_title[9] = new QLabel("info_text", this); 
                m_hbox = new QHBoxLayout(this);
                QVBoxLayout* v1 = new QVBoxLayout;
                QVBoxLayout* v2 = new QVBoxLayout;
                for (int i = 0; i < 10; ++i) {
                        v1->addWidget(m_title[i]); 
                        v2->addWidget(m_color[i]); 
                }
                v1->addWidget(m_name);
                v2->addWidget(m_save);
                m_hbox->addLayout(v1);
                m_hbox->addLayout(v2);
        }

        void set_colors() {
                m_color[0]->setText(m_style->BACK_COLOR);      // 1
                m_color[1]->setText(m_style->BUTTON_DEFAULT);  // 2
                m_color[2]->setText(m_style->BUTTON_DISABLED); // 3
                m_color[3]->setText(m_style->BUTTON_HOVER);    // 4
                m_color[4]->setText(m_style->BUTTON_PRESSED);  // 5
                m_color[5]->setText(m_style->BUTTON_TEXT);     // 6
                m_color[6]->setText(m_style->BORDER);          // 7
                m_color[7]->setText(m_style->TEXT);            // 8
                m_color[8]->setText(m_style->TEXT_BACK);       // 9
                m_color[9]->setText(m_style->INFO_TEXT);       // 10
                for (int i = 0; i < 10; ++i) {
                        m_color[i]->setStyleSheet(
                                "background-color:" + m_color[i]->text() + ";"
                                "border-color: 1px solid black;"
                                "color: black;");
                }
                QString s = QString::fromUtf8(m_style->m_theme.c_str());
                s.remove(".dtm");
                s.replace('_', ' ');
                m_name->setText(s);
        }

        void set_style(css::style* s)
        {
                assert(0 != s);
                m_style = s;
                set_colors();
        }
        
        ~colors_window()
        {

        }
};

#endif
