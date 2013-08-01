/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file EngineProject.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "EngineProject.hpp"

#include <libxml2/libxml/xmlerror.h>
#include <boost/date_time.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/guicommon-gtk/RunDialogMachineListener.hpp>
#include <openfluid/guicommon-gtk/DialogBoxFactory.hpp>
#include <openfluid/guicommon-gtk/PreferencesManager.hpp>
#include <openfluid/guicommon-gtk/SimulationRunDialog.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Generator.hpp>

#include <openfluid/ware/GeneratorSignature.hpp>
#include "EngineHelper.hpp"
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include "WaresHelper.hpp"

#include "ProjectChecker.hpp"

// =====================================================================
// =====================================================================

Glib::ustring Msg = "";

void MyErrorHandler(void* /*userData*/, xmlErrorPtr error)
{
  Msg = Glib::ustring::compose(
      _("Xml error:\n%1\n\nduring parse of file:\n%2\n\n"
          "Project can't be opened."),
      error->message, error->file);
}
;

// =====================================================================
// =====================================================================

EngineProject::EngineProject(Glib::ustring FolderIn, bool WithProjectManager) :
    m_WithProjectManager(WithProjectManager), mp_IOListener(0), m_HasChangesAtStart(
        false), mp_FXDesc(0)
{
  std::string Now = boost::posix_time::to_iso_extended_string(
      boost::posix_time::microsec_clock::local_time());
  Now[10] = ' ';
  m_DefaultBeginDT.setFromISOString(Now);

  m_DefaultDeltaT = openfluid::core::DateTime::Minutes(5);

  if (m_WithProjectManager)
    openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();

  mp_IOListener = new openfluid::base::IOListener();

  mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(mp_IOListener);

  if (FolderIn == "")
  {
    setDefaultRunDesc();
  }
  else
  {
    //reset the generic error handler
    initGenericErrorDefaultFunc(NULL);
    //supply custom error handler
    xmlSetStructuredErrorFunc(NULL, MyErrorHandler);

    try
    {
      mp_FXDesc->loadFromDirectory(
          WithProjectManager ? openfluid::base::ProjectManager::getInstance()->getInputDir() :
                               std::string(FolderIn));
    }
    catch (openfluid::base::Exception& e)
    {
      if (Msg.empty())
        Msg = EngineHelper::minimiseInfoString(e.what());

      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(Msg);

      //because we're in a constructor catch, so destructor isn't called
      deleteEngineObjects();
      throw;
    }

    checkAndSetDefaultRunValues();
  }

  try
  {
    mp_AdvancedDesc = new openfluid::fluidx::AdvancedFluidXDescriptor(
        *mp_FXDesc);
  }
  catch (openfluid::base::Exception& e)
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
        EngineHelper::minimiseInfoString(e.what()));

    //because we're in a constructor catch, so destructor isn't called
    deleteEngineObjects();
    throw;
  }

  checkAndAdaptModel();
  checkAndAdaptMonitoring();

  mp_Checker = new ProjectChecker(*mp_AdvancedDesc);
}

// =====================================================================
// =====================================================================

void EngineProject::checkAndAdaptModel()
{
  std::list<std::string> MissingSimulators;

  std::list<openfluid::fluidx::ModelItemDescriptor*> ModifiedSimulators =
      WaresHelper::checkAndGetModifiedModel(mp_AdvancedDesc->getModel(),
                                            MissingSimulators);

  if (!MissingSimulators.empty())
  {
    std::string MissingSimulatorsStr = "";
    for (std::list<std::string>::iterator it = MissingSimulators.begin();
        it != MissingSimulators.end(); ++it)
      MissingSimulatorsStr += "- " + *it + "\n";

    Glib::ustring Msg = Glib::ustring::compose(
        _("Unable to find plugin file(s):\n%1\n\n"
            "Corresponding simulator(s) will be removed from the model.\n"
            "Do you want to continue?"),
        MissingSimulatorsStr);

    if (!openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
        Msg))
    {
      deleteEngineObjects();
      delete mp_AdvancedDesc;
      throw openfluid::base::ApplicationException("openfluid-builder","unknown error");
    }
    else
    {
      mp_AdvancedDesc->getModel().setItems(ModifiedSimulators);
      m_HasChangesAtStart = true;
    }
  }

}

// =====================================================================
// =====================================================================

void EngineProject::checkAndAdaptMonitoring()
{
  std::list<std::string> MissingObservers;

  std::list<openfluid::fluidx::ObserverDescriptor*> ModifiedObservers =
      WaresHelper::checkAndGetModifiedMonitoring(
          mp_AdvancedDesc->getMonitoring(), MissingObservers);

  if (!MissingObservers.empty())
  {
    std::string MissingObserversStr = "";
    for (std::list<std::string>::iterator it = MissingObservers.begin();
        it != MissingObservers.end(); ++it)
      MissingObserversStr += "- " + *it + "\n";

    Glib::ustring Msg = Glib::ustring::compose(
        _("Unable to find plugin file(s):\n%1\n\n"
            "Corresponding observers will be removed from the monitoring.\n"
            "Do you want to continue?"),
        MissingObserversStr);

    if (!openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
        Msg))
    {
      deleteEngineObjects();
      delete mp_AdvancedDesc;
      throw openfluid::base::ApplicationException("openfluid-builder","unknown error");
    }
    else
    {
      mp_AdvancedDesc->getMonitoring().setItems(ModifiedObservers);
      m_HasChangesAtStart = true;
    }
  }

}

// =====================================================================
// =====================================================================

sigc::signal<void> EngineProject::signal_RunStarted()
{
  return m_signal_RunStarted;
}

// =====================================================================
// =====================================================================

sigc::signal<void> EngineProject::signal_RunStopped()
{
  return m_signal_RunStopped;
}

// =====================================================================
// =====================================================================

sigc::signal<void> EngineProject::signal_SaveHappened()
{
  return m_signal_SaveHappened;
}

// =====================================================================
// =====================================================================

void EngineProject::setDefaultRunDesc()
{
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  openfluid::core::DateTime DefaultBeginDT;
  openfluid::core::DateTime DefaultEndDT;
  int DefaultDeltaT;

  if (PrefMgr->getBegin() != "")
    DefaultBeginDT.setFromISOString(PrefMgr->getBegin());
  else
    DefaultBeginDT = m_DefaultBeginDT;

  if (PrefMgr->getEnd() != "")
    DefaultEndDT.setFromISOString(PrefMgr->getEnd());
  else
    DefaultEndDT = DefaultBeginDT + openfluid::core::DateTime::Day();

  if (PrefMgr->getDeltaT() != -1)
    DefaultDeltaT = PrefMgr->getDeltaT();
  else
    DefaultDeltaT = m_DefaultDeltaT;

  openfluid::fluidx::RunDescriptor RunDesc(DefaultDeltaT, DefaultBeginDT,
                                           DefaultEndDT);

  RunDesc.setFilled(true);

  mp_FXDesc->getRunDescriptor() = RunDesc;
}

// =====================================================================
// =====================================================================

void EngineProject::checkAndSetDefaultRunValues()
{
  openfluid::fluidx::RunDescriptor& RunDesc = mp_FXDesc->getRunDescriptor();

  openfluid::core::DateTime DT;
  openfluid::core::DateTime BeginDT;

  if (!DT.setFromISOString(RunDesc.getBeginDate().getAsISOString()))
  {
    RunDesc.setBeginDate(m_DefaultBeginDT);

    BeginDT = m_DefaultBeginDT;
  }
  else
    BeginDT = DT;

  if (!DT.setFromISOString(RunDesc.getEndDate().getAsISOString()))
  {
    openfluid::core::DateTime EndDT = BeginDT
        + openfluid::core::DateTime::Day();

    RunDesc.setEndDate(EndDT);
  }

  if (RunDesc.getDeltaT() < 1)
    RunDesc.setDeltaT(m_DefaultDeltaT);

  RunDesc.setFilled(true);
}

// =====================================================================
// =====================================================================

bool EngineProject::hasChangesAtStart()
{
  return m_HasChangesAtStart;
}

// =====================================================================
// =====================================================================

void EngineProject::run()
{
  openfluid::base::ProjectManager::getInstance()->updateOutputDir();

  if (m_WithProjectManager)
    openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();

  openfluid::machine::SimulationBlob SimBlob;

  openfluid::guicommon::RunDialogMachineListener Listener;

  openfluid::machine::ModelInstance ModelInstance(SimBlob, &Listener);

  openfluid::machine::MonitoringInstance MonitInstance(SimBlob);

  openfluid::machine::Engine Engine(SimBlob, ModelInstance, MonitInstance,
                                    &Listener);

  openfluid::machine::Factory::buildSimulationBlobFromDescriptors(*mp_FXDesc,
                                                                  SimBlob);

  openfluid::machine::Factory::buildModelInstanceFromDescriptor(
      mp_FXDesc->getModelDescriptor(), ModelInstance);

  openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(
      mp_FXDesc->getMonitoringDescriptor(), MonitInstance);

  openfluid::machine::Factory::fillRunEnvironmentFromDescriptor(
      mp_FXDesc->getRunDescriptor());

  SimBlob.getCoreRepository().sortUnitsByProcessOrder();

  openfluid::guicommon::SimulationRunDialog RunDialog(&Engine);

  RunDialog.signal_SimulationStarted().connect(
      sigc::mem_fun(*this, &EngineProject::whenSimulationStarted));
  RunDialog.signal_SimulationStopped().connect(
      sigc::mem_fun(*this, &EngineProject::whenSimulationStopped));

  Gtk::Main::run(RunDialog);
}

// =====================================================================
// =====================================================================

void EngineProject::whenSimulationStarted()
{
  m_signal_RunStarted.emit();
}

// =====================================================================
// =====================================================================

void EngineProject::whenSimulationStopped()
{
  m_signal_RunStopped.emit();
}

// =====================================================================
// =====================================================================

void EngineProject::save()
{
  std::string InputDir =
      openfluid::base::RuntimeEnvironment::getInstance()->getInputDir();

  openfluid::base::ProjectManager::getInstance()->save();

  boost::filesystem::path InputPath(InputDir);

  boost::filesystem::directory_iterator end_it;
  for (boost::filesystem::directory_iterator it(InputPath); it != end_it; ++it)
  {
    if ((boost::filesystem::extension(it->path()) == ".fluidx"))
      boost::filesystem::remove(it->path());
  }

  mp_FXDesc->writeToManyFiles(InputDir);

  m_signal_SaveHappened.emit();
}

// =====================================================================
// =====================================================================

const ProjectChecker& EngineProject::check(bool& GlobalState)
{
  GlobalState = mp_Checker->check();

  return *mp_Checker;
}

// =====================================================================
// =====================================================================

openfluid::fluidx::AdvancedFluidXDescriptor& EngineProject::getAdvancedDesc()
{
  return *mp_AdvancedDesc;
}

// =====================================================================
// =====================================================================

EngineProject::~EngineProject()
{
  deleteEngineObjects();
  delete mp_AdvancedDesc;
  delete mp_Checker;
}

// =====================================================================
// =====================================================================

void EngineProject::deleteEngineObjects()
{
  delete mp_IOListener;
  delete mp_FXDesc;
  //don't delete mp_RunEnv, which is singleton
  if (m_WithProjectManager)
    openfluid::base::RuntimeEnvironment::getInstance()->detachFromProject();
}

// =====================================================================
// =====================================================================
