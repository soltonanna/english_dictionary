#ifndef DATA_HELPER
#define DATA_HELPER

// Headers from standard library
#include <cassert>

// Headers from third party library
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>

class data_helper
{
    private:
        QSqlDatabase* m_data;
        QSqlQuery* m_query;
        QSqlTableModel* m_model;
        QString m_path;
        QString m_table_name;
        QWidget* m_widget;
        QTableView* m_table_view;
    private:
        int get_id();
        QString get_insert_command(QString a = "", QString b = "",
                QString c = "", QString d = "",
                QString e = "", QString f = "", 
                QString g = "");
    public:
        void select_by_word(QString, QVector<QString>&, QVector<QString>&, QVector<QString>&);
        void update_by_id(QString, QString, QString);
        void update_topic_by_id(QString, QString, QString, QString);
        QString get_arm(QString);
        QString get_eng(QString);
        QString get_type(QString);
        QVector<QString> get_column(int);
        int get_col_count();
        int get_row_count();
        void delete_by_id(QVector<QString>);
        void insert(QString a = "", QString b = "",
                QString c = "", QString d = "",
                QString e = "", QString f = "", 
                QString g = "");
        void connect_to_table_view(QTableView*);	
        void setup_database(QString, QString);
    public:
        explicit data_helper(QWidget*);
        ~data_helper();
};

#endif // DATA_HELPER
