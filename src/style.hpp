#ifndef STYLE_HPP
#define STYLE_HPP

// Headers from third party library
#include <QString>
#include <QDebug>

// Header from standard library
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <dirent.h>

// Defined macros
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32)&&!defined(__CYGWIN__)
        #define THEME_FILE "\\data\\theme"
#else
        #define THEME_FILE "data/theme"
#endif


namespace css {
        class style;
};

class css::
style 
{
public:
        QString BACK_COLOR;             // 1
        QString BUTTON_DEFAULT;         // 2        
        QString BUTTON_DISABLED;        // 3
        QString BUTTON_HOVER;           // 4
        QString BUTTON_PRESSED;         // 5
        QString BUTTON_TEXT;            // 6
        QString BORDER;                 // 7
        QString TEXT;                   // 8
        QString TEXT_BACK;              // 9
        QString INFO_TEXT;              // 10
        QString radius;
        QString radius_scroll;
        std::set<std::string> m_themes;
        std::string m_theme;
        //--style
        QString widget;
        QString check_box;
        QString button;
        QString theme_button;
        QString line_edit;
        QString spin_box;
        QString label;
        QString scroll;
        QString table;
        QString combo_box;
public:
        void set_colors( const QString c1, const QString c2, const QString c3, 
                        const QString c4, const QString c5, const QString c6, 
                        const QString c7, const QString c8, const QString c9,
                        const QString c10)
        {
                BACK_COLOR = c1;     
                BUTTON_DEFAULT = c2;        
                BUTTON_DISABLED = c3;
                BUTTON_HOVER = c4;   
                BUTTON_PRESSED = c5; 
                BUTTON_TEXT = c6;    
                BORDER = c7;         
                TEXT = c8;           
                TEXT_BACK = c9;      
                INFO_TEXT = c10;     
        }
        void initialize_colors()
        {
                std::string p = "themes/" + m_theme;
                std::ifstream f(p.c_str());
                std::string a;
                f >> a; BACK_COLOR = QString::fromUtf8(a.c_str());
                f >> a; BUTTON_DEFAULT = QString::fromUtf8(a.c_str());
                f >> a; BUTTON_DISABLED = QString::fromUtf8(a.c_str());
                f >> a; BUTTON_HOVER = QString::fromUtf8(a.c_str());
                f >> a; BUTTON_PRESSED = QString::fromUtf8(a.c_str());
                f >> a; BUTTON_TEXT = QString::fromUtf8(a.c_str());
                f >> a; BORDER = QString::fromUtf8(a.c_str());
                f >> a; TEXT = QString::fromUtf8(a.c_str());
                f >> a; TEXT_BACK = QString::fromUtf8(a.c_str());
                f >> a; INFO_TEXT = QString::fromUtf8(a.c_str());
        }
public:
        
        bool is_enabled_radius() 
        {
                return ("0" != radius);
        }
        void radius_enabled(bool t) 
        {
                if (t) {
                        radius = "8";
                        radius_scroll = "5";
                } else {
                        radius = radius_scroll = "0";
                }
        }
        QString get_button_def_color()
        {
                return BUTTON_DEFAULT;
        }
        QString get_back_color()
        {
                return BACK_COLOR;
        }
        
public:
        const std::string& get_style()
        {
                return m_theme;
        }

        void save()
        {
                std::ofstream f;
                std::remove(THEME_FILE);
                f.open(THEME_FILE);
                QString t = QString::fromUtf8(m_theme.c_str());
                t.replace(' ', '_');
                f << t.toUtf8().constData() << std::endl;
                f << is_enabled_radius() << std::endl;
                f.close();
        }

        void set_next_theme()
        {
                std::set<std::string>::iterator t = m_themes.find(m_theme);
                ++t;
                if (t == m_themes.end()) {
                        t = m_themes.begin(); 
                }
                m_theme = *t;
                initialize_colors();
                initialize_css_styles();
        }

        void set_theme(std::string t) 
        {
                m_theme = t;
                initialize_colors();
                initialize_css_styles();
        }

        const QString get_all()
        {
                const QString s = widget + check_box + button + 
                        line_edit + spin_box + label + theme_button +
                        table + scroll + combo_box;
                return s; 
        }

        void initialize_themes()
        {
                DIR *d;
                struct dirent *e;
                m_themes.clear();
                if ((d = opendir("themes")) != NULL) {
                        while ((e = readdir(d)) != NULL) {
                                QString a = QString(e->d_name);
                                if ("dtm" == a.section(".", -1, -1)) {
                                       m_themes.insert(a.toUtf8().constData());
                                }
                        }
                        closedir(d);
                }
        }
public:
        style()
        {
                initialize_themes();
                std::ifstream f;
                f.open(THEME_FILE);
                if (f.good()) {
                        std::string i;
                        bool t;
                        f >> i; 
                        f >> t;
                        radius_enabled(t);
                        QString m = QString::fromUtf8(i.c_str());
                        m.replace('_', ' ');
                        set_theme(m.toUtf8().constData());
                        f.close();
                } else {
                        std::ofstream f;
                        f.open(THEME_FILE);
                        f << "Sky_blue.dtm" << std::endl;
                        f << 0 << std::endl;
                        set_theme("Sky blue.dtm");
                        f.close();
                        return;
                }
        }

        void initialize_css_styles() {
                check_box = 
                        "QCheckBox"
                        "{"
                        "background-color: none;"
                        "outline: none;"
                        "border-radius: " + radius + "px;"
                        "font-size: 16px;"
                        "color: " + BUTTON_DEFAULT + ";"
                        "margin:2px;"
                        "}"
                        "QCheckBox:focus"
                        "{"
                        "background-color: " + BUTTON_DEFAULT + ";"
                        "color: " + BUTTON_TEXT + ";"
                        "margin:2px;"
                        "margin:1px;"
                        "}"
                        "QCheckBox:hover"
                        "{"
                        "background-color: " + BUTTON_DEFAULT + ";"
                        "color: " + BUTTON_TEXT + ";"
                        "margin:2px;"
                        "margin:1px;"
                        "}"
                        "QCheckBox:pressed"
                        "{"
                        "background-color: " + BUTTON_PRESSED + ";"
                        "color: " + BUTTON_TEXT + ";"
                        "margin:2px;"
                        "margin:2px 0px 0px 2px;"
                        "}";
                line_edit = 
                        "QLineEdit"
                        "{"
                        "border: 1px solid " + BORDER + ";"
                        "border-radius: " + radius + "px;"
                        "color: black;"
                        "}";
                theme_button = 
                        "QPushButton#theme"
                        "{"
                        "outline: none;"
                        "border-radius: 11px;"
                        "border: 3px solid #DADADA;"
                        "background-color: QLinearGradient(x1:0,y1:0,x2:1,y2:1,"
                        "stop:0 #FF0000, stop:0.5 #FFBF00, stop:1 #00FFFF);" 
                        "}"
                        "QPushButton#theme:hover"
                        "{"
                        "border: 2px solid #DADADA;"
                        "}"
                        "QPushButton#theme:focus"
                        "{"
                        "border: 2px solid #DADADA;"
                        "}"
                        "QPushButton#theme:pressed"
                        "{"
                        "border: 3px solid #DADADA;"
                        "}";
                button = 
                        "QPushButton#button"
                        "{"
                        "background-color: " + BUTTON_DEFAULT + ";"
                        "outline: none;"
                        "border-radius: " + radius + "px;"
                        "font-size: 16px;"
                        "color: " + BUTTON_TEXT + ";"
                        "margin:2px;"
                        "}"
                        "QPushButton#button:hover"
                        "{"
                        "background-color: " + BUTTON_HOVER + ";"
                        "margin:1px;"
                        "}"
                        "QPushButton#button:focus"
                        "{"
                        "background-color: " + BUTTON_HOVER + ";"
                        "margin:1px;"
                        "}"
                        "QPushButton#button:pressed"
                        "{"
                        "background-color: " + BUTTON_PRESSED + ";"
                        "margin:2px 0px 0px 2px;"
                        "}"
                        "QPushButton#button:disabled"
                        "{"
                        "background-color: " + BUTTON_DISABLED + ";"
                        "}";
                spin_box =
                        "QSpinBox "
                        "{"
                        "selection-background-color: #444;"
                        "border: 1px solid " + BORDER + ";"
                        "border-radius: " + radius + "px;"
                        "font-size: 16px;"
                        "margin-top: 3px;"
                        "padding: 5px 10px;"
                        "}"
                        "QSpinBox::up-button"
                        "{"
                        "width: 22px;"
                        "height: 12px;"
                        "background-color:" + BUTTON_DEFAULT + ";"
                        "border: 1px solid " + BORDER + ";"
                        "border-top-right-radius: " + radius + "px;"
                        "margin: 1px; "
                        "}"
                        "QSpinBox::down-button"
                        "{"
                        "width: 22px;"
                        "height: 12px;"
                        "background-color:" + BUTTON_DEFAULT + ";"
                        "border: 1px solid " + BORDER + ";" 
                        "border-bottom-right-radius: " + radius + "px;"
                        "margin: 1px; "
                        "}"
                        "QSpinBox::up-button:hover"
                        "{"
                        "background-color:" + BUTTON_HOVER + ";"
                        "}"
                        "QSpinBox::down-button:hover"
                        "{"
                        "background-color:" + BUTTON_HOVER + ";"
                        "}"
                        "QSpinBox::up-button:pressed"
                        "{"
                        "background-color:" + BUTTON_PRESSED + ";"
                        "}"
                        "QSpinBox::down-button:pressed"
                        "{"
                        "background-color:" + BUTTON_PRESSED + ";"
                        "}"; 
                label = 
                        "QLabel#text_region"
                        "{"
                        "border: 1px solid " + BORDER + ";"
                        "border-radius: " + radius + "px;"
                        "color: " + TEXT + ";"
                        "background-color: " + TEXT_BACK + ";"
                        "}"
                        "QLabel#info_text"
                        "{"
                        "background-color: " + BACK_COLOR + ";"
                        "color: " + INFO_TEXT + ";"
                        "}";
                widget = 
                        "QWidget#widget"
                        "{"
                        "background-color: " + BACK_COLOR + ";"
                        "}";
                scroll =
                        "QScrollBar:vertical "
                        "{"
                        "    border: 1px solid " + BORDER + ";"
                        "    background: " + BACK_COLOR + ";"
                        "    width:  17px;    "
                        "    margin: 15px 0px;"
                        "    position: absolute;"
                        "    padding: 1px 1px;"
                        "}"
                        "QScrollBar::handle:vertical {"
                        "    background-color: " + BUTTON_DEFAULT + ";"
                        "    min-height: 0px;"
                        "    border-radius: " + radius_scroll + "px;"
                        "    border: 1px solid " + BORDER + ";"
                        "    margin: 1px 0px;"
                        "}"
                        "QScrollBar::handle:vertical:hover {"
                        "    background-color: " + BUTTON_HOVER + ";"
                        "}"
                        "QScrollBar::handle:vertical:pressed {"
                        "    background-color: " + BUTTON_PRESSED + ";"
                        "}"
                        "QScrollBar::add-line:vertical {"
                        "    border: 1px solid " + BORDER + ";"
                        "    background: " + BUTTON_DEFAULT + ";"
                        "    height: 15px;"
                        "    subcontrol-position: bottom;"
                        "    subcontrol-origin: margin;"
                        "    border-bottom-right-radius: " + radius + "px;"
                        "    border-bottom-left-radius: " + radius + "px;"
                        "}"
                        "QScrollBar::add-line:vertical:hover {"
                        "    background: " + BUTTON_HOVER + ";"
                        "}"
                        "QScrollBar::add-line:vertical:pressed {"
                        "    background: " + BUTTON_PRESSED + ";"
                        "}"
                        "QScrollBar::sub-line:vertical {"
                        "    border: 1px solid " + BORDER + ";"
                        "    background: " + BUTTON_DEFAULT + ";"
                        "    height: 15px;"
                        "    subcontrol-position: top;"
                        "    subcontrol-origin: margin;"
                        "    border-top-right-radius: " + radius + "px;"
                        "    border-top-left-radius: " + radius + "px;"
                        "}"
                        "QScrollBar::sub-line:vertical:hover {"
                        "    background: " + BUTTON_HOVER + ";"
                        "}"
                        "QScrollBar::sub-line:vertical:pressed {"
                        "    background: " + BUTTON_PRESSED + ";"
                        "}";
                combo_box = 
                        "QComboBox {"
                        "border: 1px solid" + BACK_COLOR + ";"
                        "border-radius: " + radius + "px;"
                        "color: " + "f000" + ";"
                        "}"
                        "QComboBox:!editable," 
                        "QComboBox::drop-down:editable {"
                        "background: " + BUTTON_DEFAULT + ";"
                        "}"
                        "QComboBox:!editable:on," 
                        "QComboBox::drop-down:editable:on"
                        "{"
                        "background:" + BUTTON_PRESSED + ";"
                        "}"
                        "QComboBox:on { "
                        "padding-top: 3px;"
                        "padding-left: 4px;"
                        "}"
                        "QComboBox::drop-down {"
                        "subcontrol-origin: padding;"
                        "subcontrol-position: top right;"
                        "width: 50px;"
                        "border-left-width: 5px;"
                        "border-left-color: " + BACK_COLOR + ";"
                        "border-left-style: solid;" 
                        "}"
                        "QComboBox::down-arrow:on {"
                        "top: 1px;"
                        "left: 1px;"
                        "}"
                        "QComboBox QAbstractItemView {"
                        "border: none;"
                        "border-radius: " + radius + "px;"
                        "}";
                table =
                        "QTableView"
                        "{"
                        "selection-background-color:" + TEXT_BACK + ";"
                        "border: 3px solid " + BUTTON_DEFAULT + ";"
                        "border-radius: " + radius + "px;"
                        "margin: 0px 8px 0px 0px;"                      
                        "}"
                        "QTableView QTableCornerButton::section"
                        "{"
                        "background: " + BUTTON_DEFAULT + ";"
                        "}"
                        "QHeaderView::section"
                        "{"
                        "background-color:" + BUTTON_DEFAULT + ";" 
                        "color: " + BUTTON_TEXT + ";" 
                        "border: 0px solid " + BUTTON_DEFAULT + ";"
                        "}"
                        "QTableView::item"
                        "{"
                        "background-color: white;"
                        "color: black;"
                        "}"
                        "QTableView::item:selected"
                        "{"
                        "background-color: #DDD;"
                        "selection-color: #444;"
                        "}"
                        ;
        }
};

#endif
