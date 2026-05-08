// Autori: xdurecs00, x
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