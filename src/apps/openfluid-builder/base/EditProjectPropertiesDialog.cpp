/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file EditProjectPropertiesDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/base/RunContextManager.hpp>

#include <openfluid/ui/config.hpp>

#include "ui_EditProjectPropertiesDialog.h"
#include "EditProjectPropertiesDialog.hpp"


#include <QPushButton>
#include <QDateTime>


EditProjectPropertiesDialog::EditProjectPropertiesDialog(QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),ui(new Ui::EditProjectPropertiesDialog)
{
  openfluid::base::RunContextManager* PrjMan =
      openfluid::base::RunContextManager::instance();

  ui->setupUi(this);

  setMessage();

  ui->ProjectNameLabel->setText(QString::fromStdString(PrjMan->getProjectName()));
  ui->DescriptionTextEdit->setPlainText(QString::fromStdString(PrjMan->getProjectDescription()));
  ui->AuthorsLineEdit->setText(QString::fromStdString(PrjMan->getProjectAuthors()));

  QDateTime DT = QDateTime::fromString(QString::fromStdString(PrjMan->getProjectCreationDate()),"yyyyMMdd'T'hhmmss");
  if (DT.isValid())
    ui->CreationDateLabel->setText(DT.toString("yyyy-MM-dd, hh:mm:ss"));
  else
    ui->CreationDateLabel->setText(tr("unknown"));

  DT = QDateTime::fromString(QString::fromStdString(PrjMan->getProjectLastModDate()),"yyyyMMdd'T'hhmmss");
  if (DT.isValid())
    ui->LastModDateLabel->setText(DT.toString("yyyy-MM-dd, hh:mm:ss"));
  else
    ui->LastModDateLabel->setText(tr("unknown"));


  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


EditProjectPropertiesDialog::~EditProjectPropertiesDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditProjectPropertiesDialog::setMessage()
{
  ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                  .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
  ui->MessageLabel->setText(tr("Edit project properties"));
  ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}


// =====================================================================
// =====================================================================


QString EditProjectPropertiesDialog::getDescription() const
{
  return ui->DescriptionTextEdit->toPlainText();
}


// =====================================================================
// =====================================================================


QString EditProjectPropertiesDialog::getAuthors() const
{
  return ui->AuthorsLineEdit->text();
}
