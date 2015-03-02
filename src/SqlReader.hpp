#ifndef SQLREADER_HPP_
#define SQLREADER_HPP_

#include <QtSql/QSqlQuery>

class SqlReader {
    QSqlQuery &m_query;
    int m_index;

    SqlReader();
    SqlReader(SqlReader const&);
    SqlReader& operator = (SqlReader const&);
public:
    SqlReader(QSqlQuery &query) : m_query(query), m_index(0) {
    }
    inline SqlReader& operator >> (int &value) {
        if(m_query.value(m_index).isValid() &&
           !m_query.value(m_index).isNull()) {
            bool bOk = false;
            value = m_query.value(m_index).toInt(&bOk);
            if(!bOk) {
                value = 0;
            }
        } else {
            value = 0;
        }
        m_index++;
        return *this;
    }
    inline SqlReader& operator >> (double &value) {
        if(m_query.value(m_index).isValid() &&
           !m_query.value(m_index).isNull()) {
            bool bOk = false;
            value = m_query.value(m_index).toDouble(&bOk);
            if(!bOk) {
                value = 0.0;
            }
        } else {
            value = 0.0;
        }
        m_index++;
        return *this;
    }
    inline SqlReader& operator >> (QString &value) {
        if(m_query.value(m_index).isValid() &&
           !m_query.value(m_index).isNull()) {
            value = m_query.value(m_index).toString();
        } else {
            value = "";
        }
        m_index++;
        return *this;
    }
};

#endif
