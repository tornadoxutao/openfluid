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
  \file WFSSourceAddDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_SourceAddDialog.h"
#include "WFSSourceAddDialog.hpp"

#include <QPushButton>
#include <QMessageBox>
#include <iostream>


WFSSourceAddDialog::WFSSourceAddDialog(QWidget* Parent):
  SourceAddDialog(Parent)
{
  setWindowTitle("Spatial data import (OGR/GDAL) - Add WFS source");

  ui->FileSelectWidget->setVisible(false);

  connect(ui->ConnectButton,SIGNAL(clicked()),this,SLOT(connectToWFS()));
}


// =====================================================================
// =====================================================================


WFSSourceAddDialog::~WFSSourceAddDialog()
{

}


// =====================================================================
// =====================================================================


void WFSSourceAddDialog::connectToWFS()
{
  ui->URLLineEdit->setText(ui->URLLineEdit->text().trimmed());

  if (!ui->URLLineEdit->text().isEmpty())
  {
    m_CurrentSourceURI = "WFS:"+ui->URLLineEdit->text();

    openSource();

    if (mp_DataSource == NULL)
    {
      m_CurrentSourceURI.clear();
      QMessageBox::critical(this,tr("WFS error"),tr("Error connecting to WFS service\n")+m_CurrentSourceURI+tr("\n\nAborting."));
    }
  }

  globalCheck();
}


// =====================================================================
// =====================================================================


void WFSSourceAddDialog::prepareToImport()
{
  std::cout << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;
  m_SrcInfos.SourceURI = m_CurrentSourceURI;
}
