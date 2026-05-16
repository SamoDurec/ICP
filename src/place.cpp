/**
 * @file place.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie miesta.
 */

#include "place.hpp"
/**
 * @brief Konštruktor miesta.
 */
Place::Place(const QString &id, int initialTokens)
    : m_id(id)
    , m_initialTokens(initialTokens)
    , m_tokens(initialTokens)
{}

/**
 * @brief Odstráni tokeny z miesta, pokiaľ to ide.
 * Pri úspechu vráti true, inak false.
 */
bool Place::removeTokens(int n) {
    if (m_tokens < n) return false;
    m_tokens -= n;
    return true;
}

/**
 * @brief Nastaví nový identifikátor miesta.
 * @param id Nový identifikátor.
 */
void Place::setId(const QString &id)
{
    m_id = id;
}

/**
 * @brief Nastaví počet tokenov.
 * Ľubovoľné celé číslo zadané užívateľom.
 */
void Place::setTokens(int t)
{
    m_tokens = t;
}