
// Header from this project
#include "data_helper.hpp"

// Headers from standard library
#include <cassert>
#include <iostream>

// Headers from third party library
#include <QDebug>
#include <QFileInfo>
#include <QHeaderView>

// Defined macros
#define DATABASE_TYPE "QSQLITE"

void data_helper::
select_by_word(QString s, QVector<QString>& a,
		QVector<QString>& b, QVector<QString>& c)
{
	m_query->exec("SELECT * FROM " + m_table_name +
			" WHERE arm LIKE '%" + s + "%' OR eng LIKE '%" + s + "%'");
	m_model->select();
	m_table_view->setModel(m_model);
	while(m_query->next()) {
		a.push_back(m_query->value(1).toString());
		b.push_back(m_query->value(2).toString());
		c.push_back(m_query->value(0).toString());
	}
}

void data_helper::
update_topic_by_id(QString i, QString a, QString e, QString t)
{
	m_query->exec("UPDATE " + m_table_name + " SET arm = '" +
			a + "', eng = '" + e + "', type = '" + t +
			"'  WHERE id = '" + i + "'");
	m_model->select();
	m_table_view->setModel(m_model);
}

void data_helper::
update_by_id(QString i, QString a, QString e)
{
	m_query->exec("UPDATE " + m_table_name + " SET arm = '" +
			a + "', eng = '" + e + "'  WHERE id = '" + i + "'");
	m_model->select();
	m_table_view->setModel(m_model);
}

QString data_helper::
get_type(QString i)
{
	m_query->exec("SELECT * FROM " + m_table_name +
			" WHERE id = '" + i + "'");
	m_query->next();
	QString a = m_query->value(3).toString();
	return a;
}

QString data_helper::
get_arm(QString i)
{
	m_query->exec("SELECT * FROM " + m_table_name +
			" WHERE id = '" + i + "'");
	m_query->next();
	QString a = m_query->value(1).toString();
	return a;
}

QString data_helper::
get_eng(QString i)
{
	m_query->exec("SELECT * FROM " + m_table_name +
			" WHERE id = '" + i + "'");
	m_query->next();
	QString a = m_query->value(2).toString();
	return a;
}


int data_helper::
get_id()
{
	int m = 0;
	m_query->exec("SELECT * FROM " + m_table_name);
	while (m_query->next()) {
		int a = m_query->value(0).toInt();
		if(a > m) {
			m = a;
		}
	}
	return ++m;	
}

void data_helper::
delete_by_id(QVector<QString> d)
{
	assert("" != m_table_name);
	QString s = d[0];
	for (int i = 1; i < d.size(); ++i) {
		s += ", " + d[i];
	}
	m_query->exec("DELETE FROM " + m_table_name +
			" WHERE id IN (" + s + ")");
	m_model->select();
	m_table_view->setModel(m_model);
}

QVector<QString> data_helper::
get_column(int i)
{
	QVector<QString> v;
	m_query->exec("SELECT * FROM " + m_table_name);
	while (m_query->next()) {
		QString s = m_query->value(i).toString();
		v.push_back(s);
	}
	return v;	
}

int data_helper::
get_row_count()
{
	assert(0 != m_table_view);
	while(m_model->canFetchMore())
		m_model->fetchMore();
	int c =	m_model->rowCount();
	return c;
}

int data_helper::
get_col_count()
{
	assert(0 != m_table_view);
	int c =	m_table_view->horizontalHeader()->count();
	return c;
}

QString data_helper::
get_insert_command(QString a,
		QString b,
		QString c,
		QString d,
		QString e,
		QString f,
		QString g)
{
	assert("" != a);
	assert(0 != m_table_name);
	QString id = QString::number(get_id());
	QString i = "insert into " + m_table_name + " ";
	i += "values('" + id + "', '" + a + "'";
	do {
		if("" == b) break;
		i += ", '" + b.replace("'", "`") + "'";
		if("" == c) break;
		i += ", '" + c.replace("'", "`") + "'";
		if("" == d) break;
		i += ", '" + d.replace("'", "`") + "'";
		if("" == e) break;
		i += ", '" + e.replace("'", "`") + "'";
		if("" == f) break;
		i += ", '" + f.replace("'", "`") + "'";
		if("" == g) break;
		i += ", '" + g.replace("'", "`") + "'";
	} while (0);
	return i + ")";
}

void data_helper::
insert(QString a,
		QString b, 
		QString c, 
		QString d,
		QString e,
		QString f,
		QString g)
{
	assert("" != m_table_name);
	bool t = m_query->exec(get_insert_command(a, b, c, d, e, f, g));
	if(!t) {
		qDebug() << "Error:   " << m_query->lastError().text();
		return;
	}
	m_model->select();
	assert(0 != m_table_view);
	assert(0 != m_model);
	m_table_view->setModel(m_model);
}

void data_helper::
connect_to_table_view(QTableView* v)
{
	assert(0 != m_model);
	assert(0 != v);
	m_table_view = v;
	m_table_view->setModel(m_model);
}

void data_helper::
setup_database(QString t, QString p)
{
	assert(0 != t);
	assert(0 != p);
	m_table_name = t;
	m_path = p;
	assert(0 == m_data);
	assert(0 == m_query);
	assert(0 != m_widget);
	m_data = new QSqlDatabase();
	*m_data = QSqlDatabase::addDatabase(DATABASE_TYPE);
	m_data->setDatabaseName(m_path);
	if (!m_data->open()) {
		qDebug() << "Error:   " << m_data->lastError().text();
		return;
	}
	m_query = new QSqlQuery(*m_data);
	assert(0 == m_model);
	m_model = new QSqlTableModel(m_widget, *m_data);
	m_model->setTable(m_table_name);
	m_model->select();
	assert(0 != m_model);
}

data_helper::
data_helper(QWidget* w)
	: m_data(0)
	, m_query(0)
	  , m_model(0)
	  , m_path("")
	, m_table_name("")
	  , m_table_view(0)
{
	assert(0 != w);
	m_widget = w;
}

data_helper::
~data_helper()
{
	m_data->close();
	delete m_data;
	delete m_model;
	delete m_query;
	m_data->removeDatabase("qt_sql_default_connection"); 
}
