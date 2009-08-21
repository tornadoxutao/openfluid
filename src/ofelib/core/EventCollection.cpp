/**
  \file EventCollection.cpp
  \brief Implements distributed event collection management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include "EventCollection.h"
#include <iostream>

namespace openfluid { namespace core
{



EventCollection::EventCollection()
{
}


// =====================================================================
// =====================================================================


EventCollection::~EventCollection()
{
}


// =====================================================================
// =====================================================================


bool EventCollection::addEvent(DistributedEvent* Ev)
{


  // empty list
  if (m_Events.size() == 0)
  {
    m_Events.push_back(Ev);
  }
  else
  {
    // event date is before first collection item
    if (m_Events.front()->getDateTime() >= Ev->getDateTime())
    {
      m_Events.push_front(Ev);
    }
    else
    {
      // event date is after last collection item
      if (m_Events.back()->getDateTime() <= Ev->getDateTime())
      {
        m_Events.push_back(Ev);
      }
      else
      {
        // event has to be inserted somewhere in the collection
        std::list<DistributedEvent*>::iterator DEiter;

        for(DEiter=m_Events.begin(); DEiter != m_Events.end(); ++DEiter)
        {
          if ((*DEiter)->getDateTime() >= Ev->getDateTime())
          {
            m_Events.insert(--DEiter,Ev);
            return true;
          }
        }

      }
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


bool EventCollection::getEventsBetween(const DateTime BeginDate, const DateTime EndDate,
    EventCollection *Events)
{

  std::list<DistributedEvent*>::iterator DEiter;

  for(DEiter=m_Events.begin(); DEiter != m_Events.end(); ++DEiter)
  {
    if ((*DEiter)->getDateTime().isBetween(BeginDate,EndDate))
    {
      Events->addEvent(*DEiter);
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


void EventCollection::println()
{
  std::list<DistributedEvent*>::iterator DEiter;

    for(DEiter=m_Events.begin(); DEiter != m_Events.end(); ++DEiter)
    {
      (*DEiter)->println();
    }
}


} }  // namespaces

