// Autori: xdurec00
// nacitanie .pn suboru do petrinet

#pragma once
#include "petrinet.hpp"
#include <QString>
#include <memory>

class Parser {
public:
    static std::shared_ptr<PetriNet> load(const QString &filePath, QString &error);
};