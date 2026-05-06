// Autori: xdurec00
// implementacia Parsera pre .pn subory

#include "parser.hpp"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

static QString nextLine(QStringList &lines) {
    while (!lines.isEmpty()) {
        QString l = lines.takeFirst().trimmed();
        if (!l.isEmpty()) return l;
    }
    return {};
}

std::shared_ptr<PetriNet> Parser::load(const QString &filePath, QString &error) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = "Nepodarilo sa otvorit subor: " + filePath;
        return nullptr;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd())
        lines << in.readLine();

    auto net = std::make_shared<PetriNet>();

    // meno siete
    nextLine(lines);
    net->setName(nextLine(lines));

    // komentar
    nextLine(lines); 
    net->setComment(nextLine(lines));

    // vstupy
    nextLine(lines);
    QString inputLine = nextLine(lines);
    for (const QString &s : inputLine.split(" ", Qt::SkipEmptyParts))
        net->addInput(s);

    // vystupy
    nextLine(lines); 
    QString outputLine = nextLine(lines);
    for (const QString &s : outputLine.split(" ", Qt::SkipEmptyParts))
        net->addOutput(s);

    // premenne
    nextLine(lines);
    while (!lines.isEmpty()) {
        QString l = lines.first().trimmed();
        if (l.startsWith("Místa")) break;
        lines.takeFirst();
        if (!l.isEmpty()) net->addVariable(l);
    }

    // miesta
    nextLine(lines);
    while (!lines.isEmpty()) {
        QString l = lines.first().trimmed();
        if (l.startsWith("Přechody")) break;
        lines.takeFirst();
        if (l.isEmpty()) continue;

        // IDLE (1)
        QRegularExpression rx("(\\w+)\\s+\\((\\d+)\\)");
        QRegularExpressionMatch m = rx.match(l);
        if (m.hasMatch())
            net->addPlace(m.captured(1), m.captured(2).toInt());
    }

    // prechody
    nextLine(lines);
    std::shared_ptr<Transition> current;
    while (!lines.isEmpty()) {
        QString l = lines.takeFirst().trimmed();
        if (l.isEmpty()) continue;

        if (l.endsWith(':') && !l.startsWith("in:") && !l.startsWith("out:")
                            && !l.startsWith("when:") && !l.startsWith("do:")) {
            current = net->addTransition(l.chopped(1).trimmed());
            continue;
        }
        if (!current) continue;

        if (l.startsWith("in:")) {
            for (const QString &part : l.mid(3).trimmed().split(",", Qt::SkipEmptyParts)) {
                QStringList pw = part.trimmed().split("*");
                Arc a; a.placeId = pw[0].trimmed();
                a.weight = pw.size() > 1 ? pw[1].toInt() : 1;
                current->addInputArc(a);
            }
        } else if (l.startsWith("out:")) {
            for (const QString &part : l.mid(4).trimmed().split(",", Qt::SkipEmptyParts)) {
                QStringList pw = part.trimmed().split("*");
                Arc a; a.placeId = pw[0].trimmed();
                a.weight = pw.size() > 1 ? pw[1].toInt() : 1;
                current->addOutputArc(a);
            }
        } else if (l.startsWith("when:")) {
            QString w = l.mid(5).trimmed();
            int at = w.indexOf('@');
            if (at != -1) {
                current->setDelayMs(w.mid(at+1).trimmed().toInt());
                w = w.left(at).trimmed();
            }
            int lb = w.indexOf('['), rb = w.indexOf(']');
            if (lb != -1 && rb != -1)
                current->setGuard(w.mid(lb+1, rb-lb-1).trimmed());
            current->setEventName(w.left(lb == -1 ? w.size() : lb).trimmed());
        } else if (l.startsWith("do:")) {
            int lb = l.indexOf('{'), rb = l.lastIndexOf('}');
            if (lb != -1 && rb != -1)
                current->setAction(l.mid(lb+1, rb-lb-1).trimmed());
        }
    }

    return net;
}