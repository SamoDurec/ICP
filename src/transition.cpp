/**
 * @file transition.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie prechodu.
 */

#include "transition.hpp"
/**
 * @brief Konštruktor prechodu.
 */
Transition::Transition(const QString &id)
    : m_id(id)
{}

/**
 * @brief Nastaví nový identifikátor prechodu.
 * @param id Nový identifikátor.
 */
void Transition::setId(const QString &id)
{
    m_id = id;
}