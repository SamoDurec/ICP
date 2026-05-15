/**
 * @file petrinet.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia petrinet.
 */

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

void PetriNet::removePlace(const QString &id)
{
    // prejde miesta a odstrani pozadovane
    for (int i = 0; i < m_places.size(); i++)
    {
        if (m_places[i]->id() == id)
        {
            m_places.removeAt(i);
            break;
        }
    }

    // odstrani hrany z prechodov
    for (auto &t : m_transitions)
    {
        QVector<Arc> newInputs;
        QVector<Arc>newOutputs;

        for(const auto &a : t->inputArcs())
        {
            if (a.placeId != id) newInputs.append(a);
        }

        for(const auto &a : t->outputArcs())
        {
            if (a.placeId != id) newOutputs.append(a);
        }

        t->setInputArcs(newInputs);
        t->setOutputArcs(newOutputs);
    }
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

void PetriNet::removeTransition(const QString &id)
{
    // prejde prechody a odstrani pozadovane
    for (int i = 0; i < m_transitions.size(); i++)
    {
        if (m_transitions[i]->id() == id)
        {
            m_transitions.removeAt(i);
            break;
        }
    }
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