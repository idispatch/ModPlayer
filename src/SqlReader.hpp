#ifndef SQLREADER_HPP_
#define SQLREADER_HPP_

#include <QtSql/QSqlQuery>

class SqlReader {
    QSqlQuery &m_query;
    int m_index;

    SqlReader(SqlReader const&);
    SqlReader& operator = (SqlReader const&);
public:
    SqlReader(QSqlQuery &query) : m_query(query), m_index(0) {
    }
    inline SqlReader& operator >> (int &value) {
        value = m_query.value(m_index++).toInt();
        return *this;
    }
    inline SqlReader& operator >> (double &value) {
        value = m_query.value(m_index++).toDouble();
        return *this;
    }
    inline SqlReader& operator >> (QString &value) {
        value = m_query.value(m_index++).toString();
        return *this;
    }
};

#endif
