/****************************************************************************
**
** Copyright (C) 2013, HicknHack Software
** All rights reserved.
** Contact: http://www.hicknhack-software.com/contact
**
** This file is part of the QSqlMigrator
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/
#include "SqliteDropIndexService.h"
#include "SqliteCreateIndexService.h"
#include "Commands/CreateIndex.h"
#include "Commands/DropIndex.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteDropIndexService::SqliteDropIndexService()
{
}

const QString &SqliteDropIndexService::commandType() const
{
    return Commands::DropIndex::typeName();
}

bool SqliteDropIndexService::up(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropIndex> dropIndex(command.staticCast<const Commands::DropIndex>());

    QString sDropQuery = QString("DROP INDEX %1").arg(dropIndex->name());

    return CommandExecution::BaseCommandExecutionService::executeQuery(sDropQuery, context);
}

bool SqliteDropIndexService::isUpValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(command);
    Q_UNUSED(context);

    //TODO check for index existence?!

    return true;
}

bool SqliteDropIndexService::down(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropIndex> dropIndex(command.staticCast<const Commands::DropIndex>());
    SqliteCreateIndexService createIndexService;
    return createIndexService.up(Commands::CommandPtr(new Commands::CreateIndex(dropIndex->index()))
                                 , context);
}

bool SqliteDropIndexService::isDownValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(context);

    QSharedPointer<const Commands::DropIndex> dropIndex(command.staticCast<const Commands::DropIndex>());
    if (!dropIndex->hasIndex()) {
        ::qWarning() << "Index not completely defined. Migrationg down is not possible!";
        return false;
    }
    return true;
}

} // namespace CommandExecution