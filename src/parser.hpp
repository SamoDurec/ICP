/**
 * @file parser.hpp
 * @authors xdurecs00, xpertod00
 * @brief Parser pre nacitanie Petriho siete zo suboru .pn
 */

#pragma once
#include "petrinet.hpp"
#include <QString>
#include <memory>

/**
 * @brief Nacita Petriho siet z textoveho suboru.
 */
class Parser {
public:
    /**
     * @brief Nacita siet zo suboru.
     * @param filePath Cesta k suboru.
     * @param error Chybova sprava (prazdna ak uspech).
     * @return Pointer na siet alebo nullptr pri chybe.
     */
    static std::shared_ptr<PetriNet> load(const QString &filePath, QString &error);
};