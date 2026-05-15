/**
 * @file transition.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie prechodu.
 */

#include "transition.hpp"

Transition::Transition(const QString &id)
    : m_id(id)
{}

void Transition::setId(const QString &id)
{
    m_id = id;
}