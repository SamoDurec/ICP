// Autori: xdurecs00, x
// Implementacia petrinet

#include "petrinet.hpp"
#include <algorithm>

std::shared_ptr<Place> PetriNet::addPlace(const QString &id, int tokens) {
    auto p = std::make_shared<Place>(id, tokens);
    m_places.append(p);
    return p;
}

std::shared_ptr<Place> PetriNet::findPlace(const QString &id) const {
    for (auto &p : m_places)
        if (p->id() == id) return p;
    return nullptr;
}

std::shared_ptr<Transition> PetriNet::addTransition(const QString &id) {
    auto t = std::make_shared<Transition>(id);
    m_transitions.append(t);
    return t;
}

std::shared_ptr<Transition> PetriNet::findTransition(const QString &id) const {
    for (auto &t : m_transitions)
        if (t->id() == id) return t;
    return nullptr;
}

void PetriNet::resetMarking() {
    for (auto &p : m_places) p->reset();
}

bool PetriNet::isEnabled(const std::shared_ptr<Transition> &t) const {
    for (const auto &arc : t->inputArcs()) {
        auto p = findPlace(arc.placeId);
        if (!p || p->tokens() < arc.weight) return false;
    }
    return true;
}