// Autori: xdurecs00, xpertod00
// place v Petriho sieti

#pragma once
#include <QString>
#include <QPointF>

class Place {
public:
    Place(const QString &id, int initialTokens = 0);

    QString id()           const { return m_id; }
    int     tokens()       const { return m_tokens; }
    int     initialTokens() const { return m_initialTokens; }

    void    reset()              { m_tokens = m_initialTokens; }
    void    addTokens(int n)     { m_tokens += n; }
    bool    removeTokens(int n);

    QPointF pos() const {return m_pos; }
    void setPos(const QPointF &p)
    {
        m_pos = p;
    }

    void setId(const QString &id);

private:
    QString m_id;
    int     m_initialTokens;
    int     m_tokens;
    QPointF m_pos;
};