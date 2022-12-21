#ifndef SEARCH_HPP
#define SEARCH_HPP

// Headers from third party library
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>

class search : public QWidget
{
	Q_OBJECT
private:
	QLabel* m_title;
        QLineEdit* m_word;
	QPushButton* m_search;
	QVBoxLayout* m_vbox;
private:
	void initialize_widgets();
	void to_center_screen();
	void layouts();
	void setup_connections();
signals:
        void searched(QString);
private slots:
        void search_by_word();
protected:
        virtual void keyPressEvent(QKeyEvent*);
public:
	explicit search(QWidget* p = 0);
	~search();
};

#endif
