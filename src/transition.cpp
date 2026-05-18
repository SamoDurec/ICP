/**
 * @file transition.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie prechodu.
 */

#include "transition.hpp"
/**
 * @brief Konstruktor prechodu.
 */
Transition::Transition(const QString &id)
    : m_id(id)
{}

/**
 * @brief Nastavi novy identifikator prechodu.
 * @param id Novy identifikator.
 */
void Transition::setId(const QString &id) {
    m_id = id;
}