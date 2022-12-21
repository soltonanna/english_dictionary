#ifndef VERBS_HPP
#define VERBS_HPP

// Headers from third party library
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class verbs : public QWidget
{
	Q_OBJECT
private:
	QLabel* m_words;
	QPushButton* m_random;
	QVBoxLayout* m_vbox;
private:
	void initialize_menu();
	void to_center_screen();
	void initialize_random();
	void initialize_words();
	void layouts();
	void setup_connections();
	void resize_font();
	void resize_min_max();
        QString he_or_she(const int);
private:
        QString m_current_text;
        static const QString m_pronouns[7];
        static const QString m_pron[63];
        static const QString m_verbs[63][2];
private slots:
	void show_random();
protected:
        void resizeEvent(QResizeEvent*);
	virtual void keyPressEvent(QKeyEvent*);
public:
	explicit verbs(QWidget* p = 0);
	~verbs();
};

#endif
