/**
  \file InterpDTSerie.h
  \brief 
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DISTRIBINTERP_H__
#define __DISTRIBINTERP_H__

#include <wx/string.h>
#include <wx/hashmap.h>
#include <wx/datetime.h>

#include "mhydasdk-core.h"
#include "DTSerie.h"
#include "DataSrcFile.h"
#include "Tools.h"


namespace mhydasdk { namespace tools {

WX_DECLARE_HASH_MAP(int,DateTimeSerie*,wxIntegerHash,wxIntegerEqual,DTSeriesMap);
WX_DECLARE_HASH_MAP(int,DateTimeSerie*,wxIntegerHash,wxIntegerEqual,UnitsDTSeriesMap);
WX_DECLARE_HASH_MAP(int,IndexedSerie*,wxIntegerHash,wxIntegerEqual,IndexedSeriesMap);
WX_DECLARE_HASH_MAP(int,IndexedSerie*,wxIntegerHash,wxIntegerEqual,UnitsIndexedSeriesMap);

enum SeriePreprocess
{
  SERIEPREPCS_NONE,
  SERIEPREPCS_CUMULATE
};


class DistributeInterpolate : public mhydasdk::tools::Tools
{

  private:

    DTSeriesMap m_InterpData;
    IndexedSeriesMap m_InterpIndexedData;
    UnitsDTSeriesMap m_UnitsData;
    UnitsIndexedSeriesMap m_UnitsIndexedData;
    
    
    wxString m_DataSourcesFilename;
    wxString m_DataDir;
    wxString m_DistributionFilename;
    SeriePreprocess m_SPpcs;
    wxDateTime m_Begin;
    wxDateTime m_End;
    int m_TimeStep;
    
    bool m_Configured;   
    
    bool loadDataAsSerie(wxString FilePath, SeriePreprocess SPpcs, DateTimeSerie *Serie);
    
    bool loadDistributionAndDistribute(wxString FilePath);
    
  public:
    DistributeInterpolate();
     
    void setConfig(wxString DataDir, wxString DataSourcesFilename, wxString DistributionFilename, SeriePreprocess SPpcs, wxDateTime Begin,wxDateTime End, int TimeStep);
    
    bool loadAndPrepareData();
    
    bool getValue(int ID, wxDateTime DT, mhydasdk::core::MHYDASScalarValue *Value);
    
    bool getValue(int ID, int Index, mhydasdk::core::MHYDASScalarValue *Value);
    
    virtual ~DistributeInterpolate();
};

} }


#endif /*__DISTRIBINTERP_H__*/

