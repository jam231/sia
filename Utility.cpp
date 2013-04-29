#include "Utility.h"

#include <QRegExp>
#include <QStringList>
#include <QDebug>

/*
 *  I assume .ini file have following syntax:
 *   - one line == one statement
 *   - Statement is in one of the two following forms:
 *      #A comment which will be ignored in parsing process
 *      [whitespaces]constant_name[whitespaces]=[whitespaces]value[whitespaces]
 *   - The '=' and '#' signs reserved and should be present in a noncomment
 *     statement only once!
 *   - statements first part (the key) of the statement is not case sensitive
 *     and by default they will be perceived as being lowercase.
 *     Second part (the value) is case sensitive.
 *
 *  Everything which is not a equality statement parseINI ignores,
 *  giving a debug informations about nonstatement (when in debug mode).
 */
QVector<QPair<QString, QString> > parseINI(const QStringList& lines)
{
    QVector<QPair<QString, QString> > keyValuePairs;

    /*
     * Filter out comments:
     *      \s* <=> zero or more white space characters
     *      #   <=> a hash sign
     *      .*  <=> match zero or more times any character.
     */
    foreach(QString statement, lines)
    {
        statement = statement.trimmed();
        if(QRegExp("#.*").exactMatch(statement) || statement.isEmpty())
        {
        }
        else if(QRegExp("[^#=]+=[^#=]+").exactMatch(statement))
        {
            QVector<QString> keyValue =
                    QVector<QString>::fromList(statement.split(QRegExp("=")));
            keyValuePairs.push_back(
                        qMakePair(keyValue[0].trimmed().toLower(),
                                  keyValue[1].trimmed()));
        }
        else
            qDebug() << "[parseINI]: Not a statement " << statement;
    }
    return keyValuePairs;
}
