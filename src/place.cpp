/**
 * @file place.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie miesta.
 */

#include "place.hpp"
/**
 * @brief Konstruktor miesta.
 */
Place::Place(const QString &id, int initialTokens)
    : m_id(id)
    , m_initialTokens(initialTokens)
    , m_tokens(initialTokens)
{}

/**
 * @brief Odstrani tokeny z miesta, pokial to ide.
 * Pri uspechu vrati true, inak false.
 */
bool Place::removeTokens(int n) {
    if (m_tokens < n) return false;
    m_tokens -= n;
    return true;
}

/**
 * @brief Nastavi novy identifikator miesta.
 * @param id Novy identifikator.
 */
void Place::setId(const QString &id)
{
    m_id = id;
}

/**
 * @brief Nastavi pocet tokenov.
 * Lubovolne cele cislo zadane uzivatelom.
 */
void Place::setTokens(int t)
{
    m_tokens = t;
}