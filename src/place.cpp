// Autori: xdurecs00, xpertod00
// Implementacia place

#include "place.hpp"

Place::Place(const QString &id, int initialTokens)
    : m_id(id)
    , m_initialTokens(initialTokens)
    , m_tokens(initialTokens)
{}

bool Place::removeTokens(int n) {
    if (m_tokens < n) return false;
    m_tokens -= n;
    return true;
}

void Place::setId(const QString &id)
{
    m_id = id;
}

void Place::setTokens(int t)
{
    m_tokens = t;
}