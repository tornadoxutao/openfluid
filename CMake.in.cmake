#
# Configuration file for CMakeLists.txt files
#
# Author : Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
#
# This file is included by the main CMakeLists.txt file, and defines variables
# to configure the build and install 
#
# The variables in this file can also be overriden through the  
# CMake.in.local.config file 
#


################### general ###################

SET(OPENFLUID_MAIN_NAME "openfluid")

SET(OPENFLUID_RELATIVEDIR "${OPENFLUID_MAIN_NAME}")

SET(OPENFLUID_DEFAULT_CONFIGFILE "openfluid.conf")

SET(OPENFLUID_INPUTDIR "OPENFLUID.IN")
SET(OPENFLUID_OUTPUTDIR "OPENFLUID.OUT")
SET(OPENFLUID_WORKSPACEDIR "workspace")
SET(OPENFLUID_WARESBINDIR "wares")
SET(OPENFLUID_WARESDEVDIR "wares-dev")
SET(OPENFLUID_SIMPLUGSDIR "simulators")
SET(OPENFLUID_OBSPLUGSDIR "observers")
SET(OPENFLUID_BEXTPLUGSDIR "builderexts")
SET(OPENFLUID_EXAMPLESDIR "examples")
SET(OPENFLUID_PROJECTSDIR "projects")

SET(OPENFLUID_SIMSMAXNUMTHREADS "4")

SET(OPENFLUID_PROJECT_FILE "openfluid-project.conf")
SET(OPENFLUID_PROJECT_INPUTDIR "IN")
SET(OPENFLUID_PROJECT_OUTPUTDIRPREFIX "OUT")


################### versions ###################

SET(CUSTOM_CMAKE_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

SET(VERSION_MAJOR 2)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 1)
SET(VERSION_STATUS "alpha3") # example: SET(VERSION_STATUS "rc1")

SET(FULL_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

IF(VERSION_STATUS)
  SET(FULL_VERSION "${FULL_VERSION}~${VERSION_STATUS}")
ENDIF(VERSION_STATUS)


################### install path ###################

SET(BIN_INSTALL_PATH "bin")
SET(LIB_INSTALL_PATH "lib${OF_LIBDIR_SUFFIX}")
SET(INCLUDE_INSTALL_PATH "include")
SET(SHARE_INSTALL_PATH "share")
SET(INCLUDE_OPENFLUID_INSTALL_PATH "${INCLUDE_INSTALL_PATH}/openfluid")
SET(SHARE_OPENFLUID_INSTALL_PATH "${SHARE_INSTALL_PATH}/openfluid")
SET(SHARE_DESKTOPENTRY_INSTALL_PATH "${SHARE_INSTALL_PATH}/applications")

SET(SHARE_COMMON_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/common")
SET(SHARE_APPS_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/apps")
SET(SHARE_WARESDEV_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/waresdev")
SET(SIMULATORS_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/${OPENFLUID_SIMPLUGSDIR}")
SET(OBSERVERS_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/${OPENFLUID_OBSPLUGSDIR}")
SET(BUILDEREXTS_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/${OPENFLUID_BEXTPLUGSDIR}")

IF (WIN32)
  SET(CMAKE_MODULES_INSTALL_PATH "${LIB_INSTALL_PATH}/cmake")
  SET(CMAKE_HELPERSMODULES_INSTALL_PATH "${LIB_INSTALL_PATH}/cmake")
ELSE()
  SET(CMAKE_MODULES_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/cmake")
  SET(CMAKE_HELPERSMODULES_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluidhelpers/cmake")
ENDIF()  

SET(DOC_INSTALL_PATH "${SHARE_INSTALL_PATH}/doc/openfluid")
SET(MAIN_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/main")
SET(SIMULATORS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/${OPENFLUID_SIMPLUGSDIR}")
SET(OBSERVERS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/${OPENFLUID_OBSPLUGSDIR}")
SET(EXAMPLES_INSTALL_PATH "${DOC_INSTALL_PATH}/examples")
SET(EXAMPLES_SRC_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/src")
SET(EXAMPLES_DATA_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/datasets")
SET(EXAMPLES_PROJECTS_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/${OPENFLUID_PROJECTSDIR}")
SET(KERNEL_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}")
SET(MANUALS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/manuals")
SET(MANUALS_DOCPDF_INSTALL_PATH "${MANUALS_DOC_INSTALL_PATH}/pdf")
SET(MANUALS_DOCHTML_INSTALL_PATH "${MANUALS_DOC_INSTALL_PATH}/html")
SET(EXAMPLES_DOCPDF_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}")


################### source and output path ###################

SET(BUILD_OUTPUT_PATH "${CMAKE_BINARY_DIR}/dist")

SET(LIB_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${LIB_INSTALL_PATH}")
SET(SIMULATORS_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${SIMULATORS_INSTALL_PATH}")
SET(OBSERVERS_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${OBSERVERS_INSTALL_PATH}")
SET(BUILDEREXTS_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${BUILDEREXTS_INSTALL_PATH}")
SET(BIN_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${BIN_INSTALL_PATH}")
SET(CMAKE_MODULES_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${CMAKE_MODULES_INSTALL_PATH}")

SET(DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${DOC_INSTALL_PATH}")
SET(MAIN_DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MAIN_DOC_INSTALL_PATH}")
SET(DOC_LAYOUT_DIR "${CMAKE_SOURCE_DIR}/doc/layout")
SET(DOC_CONTENTS_DIR "${CMAKE_SOURCE_DIR}/doc/contents")
SET(DOC_SOURCES_DIR "${CMAKE_SOURCE_DIR}/src/openfluid")


SET(DOCFORDEV_OUTPUT_PATH "${CMAKE_BINARY_DIR}/docfordev")
SET(DOCFORDEV_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

SET(SIMULATORS_DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${SIMULATORS_DOC_INSTALL_PATH}")

#SET(DOC_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources/doc")
SET(DOC_BUILD_DIR "${CMAKE_BINARY_DIR}/docdir-for-build")
#SET(MANUALS_DOC_BUILD_DIR "${DOC_BUILD_DIR}/manuals")
SET(MANUALS_DOCPDF_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MANUALS_DOCPDF_INSTALL_PATH}")
SET(MANUALS_DOCHTML_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MANUALS_DOCHTML_INSTALL_PATH}")

#SET(EXAMPLES_DOC_BUILD_DIR "${DOC_BUILD_DIR}/examples")
#SET(EXAMPLES_DOCPDF_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${EXAMPLES_DOCPDF_INSTALL_PATH}")
SET(EXAMPLES_PATH "${CMAKE_SOURCE_DIR}/${OPENFLUID_EXAMPLESDIR}")
SET(EXAMPLES_PROJECTS_PATH "${EXAMPLES_PATH}/projects")


SET(SHARE_TRANSLATIONS_PATH "${SHARE_INSTALL_PATH}/locale")
SET(TRANSLATIONS_BUILD_DIR "${BUILD_OUTPUT_PATH}/${SHARE_TRANSLATIONS_PATH}")


################### test paths ###################

SET(TEST_OUTPUT_PATH "${CMAKE_BINARY_DIR}/tests-bin")
SET(TESTS_DATASETS_PATH "${CMAKE_SOURCE_DIR}/resources/tests/datasets")
SET(TESTS_MISCDATA_PATH "${CMAKE_SOURCE_DIR}/resources/tests/miscdata")
SET(TESTS_OUTPUTDATA_PATH "${CMAKE_BINARY_DIR}/tests-output")

SET(OPENFLUID_TESTS_USERDATA_PATH "${CMAKE_BINARY_DIR}/tests-userdata")
SET(OPENFLUID_TESTS_TEMP_PATH "${CMAKE_BINARY_DIR}/tests-temp")


################### applications ###################

SET(OPENFLUID_CMD_APP "openfluid")
SET(OPENFLUID_BUILDER_APP "openfluid-builder")
SET(OPENFLUID_DEVSTUDIO_APP "openfluid-devstudio")
SET(OPENFLUID_LOGEXPLORER_APP "openfluid-logexplorer")

IF(APPLE)
  SET(OPENFLUID_BUILDER_APP "OpenFLUID-Builder")
  SET(OPENFLUID_DEVSTUDIO_APP "OpenFLUID-Devstudio")
  SET(OPENFLUID_LOGEXPLORER_APP "OpenFLUID-Logexplorer")
ENDIF()


################### compilation and build ###################

IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC) 
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
  SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")  
ENDIF()

SET(PLUGINS_BINARY_EXTENSION "${CMAKE_SHARED_LIBRARY_SUFFIX}")
SET(PLUGINS_GHOST_EXTENSION ".xml")

SET(OPENFLUID_SIMULATORS_SUFFIX "_ofware-sim")
SET(OPENFLUID_OBSERVERS_SUFFIX "_ofware-obs")
SET(OPENFLUID_BUILDEREXTS_SUFFIX "_ofware-bext")

SET(OPENFLUID_GHOSTSIMULATORS_SUFFIX "_ofghost-sim")


SET(DEBUG_PREFIX "[OpenFLUID debug]")


################### 3rd party headers and libraries ###################

SET(RapidJSON_GIT_URL "https://github.com/miloyip/rapidjson.git")


################### parts and libraries build ###################

SET(OPENFLUID_ENABLE_GUI 1)

SET(OPENFLUID_ENABLE_LANDR 1)

SET(OPENFLUID_ENABLE_MARKET 0)


################### applications build ###################

# set this to 1 to build openfluid command line program
SET(BUILD_APP_CMD 1)

# set this to 1 to build openfluid-minimal
SET(BUILD_APP_MINIMAL 1)

# set this to 1 to build openfluid-builder
SET(BUILD_APP_BUILDER 1)

# set this to 1 to build openfluid-devstudio
SET(BUILD_APP_DEVSTUDIO 1)

# set this to 1 enable waresdev integration in openfluid-builder
SET(ENABLE_WARESDEV_BUILDER_INTEGRATION 1)

# set this to 1 to build openfluid-market-client
SET(BUILD_APP_MARKETCLIENT 0)

# set this to 1 enable market integration in openfluid-builder
SET(ENABLE_MARKET_BUILDER_INTEGRATION 0)

# set this to 1 to build openfluid-logexplorer
SET(BUILD_APP_LOGEXPLORER 1)


################### simulators build ###################

# uncomment this to build simulators mixing C++ and fortran source codes (in this source tree)
#SET(BUILD_FORTRAN_SIMULATORS 1)


################### doc build ###################

# uncomment this to build latex docs
SET(BUILD_DOCS 1)


################### logfiles ###################

SET(OPENFLUID_MESSAGES_LOG_FILE "openfluid-messages.log")

SET(OPENFLUID_CUMULATIVE_PROFILE_FILE "openfluid-profile-cumulative.log")
SET(OPENFLUID_SCHEDULE_PROFILE_FILE "openfluid-profile-schedule.log")
SET(OPENFLUID_TIMEINDEX_PROFILE_FILE "openfluid-profile-timeindex.log")


################### waresdev ###################

SET(OPENFLUID_WARESDEV_CMAKE_USERFILE "CMake.in.config")
SET(OPENFLUID_WARESDEV_CMAKE_SIMCPPVAR "SIM_CPP")
SET(OPENFLUID_WARESDEV_CMAKE_OBSCPPVAR "OBS_CPP")
SET(OPENFLUID_WARESDEV_CMAKE_BEXTCPPVAR "BEXT_CPP")


################### market ###################

SET(OPENFLUID_MARKETBAGDIR "market-bag")
SET(OPENFLUID_MARKETPLACE_SITEFILE "OpenFLUID-Marketplace")
SET(OPENFLUID_MARKETPLACE_CATALOGFILE "Catalog")

SET(OPENFLUID_MARKET_COMMONBUILDOPTS "-DSIM_SIM2DOC_DISABLED=1")

IF(WIN32)
  SET(OPENFLUID_MARKET_COMMONBUILDOPTS "-G \\\"${CMAKE_GENERATOR}\\\" ${OPENFLUID_MARKET_COMMONBUILDOPTS}")
ENDIF()


################### i18n ###################

SET(OPENFLUID_TRANSLATIONS_FILEROOT openfluid)
SET(OPENFLUID_TRANSLATIONS_TSDIR "${CMAKE_SOURCE_DIR}/resources/translations")
SET(OPENFLUID_TRANSLATIONS_LANGS fr_FR)
SET(OPENFLUID_TRANSLATIONS_DIRSTOSCAN "${CMAKE_SOURCE_DIR}/src/openfluid/ui"
                                      "${CMAKE_SOURCE_DIR}/src/openfluid/ui/config.hpp.in"
                                      "${CMAKE_SOURCE_DIR}/src/openfluid/waresdev"                                      
                                      "${CMAKE_SOURCE_DIR}/src/apps/openfluid-builder"
                                      "${CMAKE_SOURCE_DIR}/src/apps/openfluid-devstudio")


################### tests configuration ###################

SET(TESTS_RESTSERVICE_URL "http://jsonplaceholder.typicode.com")
SET(TESTS_FLUIDHUB_URL "http://www.openfluid-project.org/resources/fluidhub-api/testing")


################### cppcheck ###################

SET(CPPCHECK_EXTRA_OPTIONS "-q" 
                           "--enable=style,information,performance,portability,missingInclude" 
                           "--suppress=variableScope"
                           "--force")

