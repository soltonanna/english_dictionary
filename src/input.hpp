#ifndef INPUT_HPP
#define INPUT_HPP

// Headers from third party library
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>

class input : public QWidget
{
	Q_OBJECT
private:
	QLabel* m_arm_title;
        QLineEdit* m_arm;
	QLabel* m_eng_title;
        QLineEdit* m_eng;
	QPushButton* m_insert;
	QPushButton* m_cancel;
	QVBoxLayout* m_vbox;
	QLabel* m_type_title;
	QComboBox* m_type;
        bool m_is_insert;
        bool m_is_topic;
private:
	void initialize_input();
	void to_center_screen();
	void initialize_arm();
	void initialize_eng();
	void initialize_type();
	void initialize_insert();
	void layouts();
	void setup_connections();
public:	
        void set_values(QString, QString);
        void set_topic_values(QString, QString, int);
        void set_insert_type(const bool);
        void set_topic_type(const bool);
signals:
        void words_inserted(QString, QString); 
        void words_updated(QString, QString); 
        void words_topic_inserted(QString, QString, QString); 
        void words_topic_updated(QString, QString, QString); 
private slots:
	void send_input();
	void send_topic_input();
	void cancel();
protected:
        virtual void keyPressEvent(QKeyEvent*);
public:
	explicit input(QWidget* p = 0);
	~input();
};

#endif
