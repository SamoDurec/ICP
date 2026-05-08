// Autori: xdurecs00, x
// Implementacia transition

#include "transition.hpp"

Transition::Transition(const QString &id)
    : m_id(id)
{}

void Transition::setId(const QString &id)
{
    m_id = id;
}