#ifndef HELPER_HPP
#define HELPER_HPP

// Headers from third party library
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class helper : public QWidget
{
	Q_OBJECT
private:
	QLabel* m_image;
	QPushButton* m_next;
	QVBoxLayout* m_vbox;
        int m_current_index;
        size_t m_previous_width;
        size_t m_previous_height;
private:
	void initialize_menu();
	void initialize_image();
        void initialize_next();
        void to_center_screen();
	void layouts();
	void setup_connections();
	void resize_image();
        void resize_min_max();
private:
private slots:
	void show_next_table();
protected:
        void resizeEvent(QResizeEvent*);
        void keyPressEvent(QKeyEvent*);
public:
	explicit helper(QWidget* p = 0);
	~helper();
};

#endif
