/**
 * @file place.hpp
 * @authors xdurecs00, xpertod00
 * @brief Reprezentacia place v Petriho sieti.
 */

#pragma once
#include <QString>
#include <QPointF>

/**
 * @brief Miesto v Petriho sieti.
 *
 * Obsahuje identifikator, pocet tokenov a poziciu v GUI.
 */
class Place {
public:
    /** @brief Konstruktor miesta. 
     * @param id Identifikator. 
     * @param initialTokens Pociatocny pocet tokenov. 
     * */
    Place(const QString &id, int initialTokens = 0);

    /** @brief Vrati identifikator miesta. */
    QString id()            const { return m_id; }
    /** @brief Vrati aktualny pocet tokenov. */
    int     tokens()        const { return m_tokens; }
    /** @brief Vrati pociatocny pocet tokenov. */
    int     initialTokens() const { return m_initialTokens; }
    /** @brief Resetuje tokeny na pociatocnu hodnotu. */
    void    reset()               { m_tokens = m_initialTokens; }
    /** @brief Prida n tokenov. */
    void    addTokens(int n)      { m_tokens += n; }
    /** @brief Odoberie n tokenov. @return false ak nie je dostatok tokenov. */
    bool    removeTokens(int n);
    /** @brief Vrati poziciu v GUI. */
    QPointF pos() const { return m_pos; }
    /** @brief Nastavi poziciu v GUI. */
    void    setPos(const QPointF &p) { m_pos = p; }
    /** @brief Nastavi identifikator. */
    void    setId(const QString &id);
    /** @brief Nastavi pocet tokenov. */
    void    setTokens(int t);

private:
    QString m_id;           ///< Identifikator miesta
    int     m_initialTokens; ///< Pociatocny pocet tokenov
    int     m_tokens;        ///< Aktualny pocet tokenov
    QPointF m_pos {0, 0};    ///< Pozicia v GUI
};