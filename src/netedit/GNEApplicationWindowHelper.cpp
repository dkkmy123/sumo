/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2020 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    GNEApplicationWindow.cpp
/// @author  Jakob Erdmann
/// @date    Feb 2011
///
// The main window of Netedit (adapted from GUIApplicationWindow)
/****************************************************************************/
#include <netbuild/NBFrame.h>
#include <netedit/elements/additional/GNEAdditionalHandler.h>
#include <netedit/elements/data/GNEDataHandler.h>
#include <netedit/elements/demand/GNERouteHandler.h>
#include <netedit/dialogs/GNEAbout.h>
#include <netedit/frames/network/GNETAZFrame.h>
#include <netedit/frames/network/GNETLSEditorFrame.h>
#include <netedit/frames/common/GNEInspectorFrame.h>
#include <netimport/NIFrame.h>
#include <netwrite/NWFrame.h>
#include <utils/common/SysUtils.h>
#include <utils/common/SystemFrame.h>
#include <utils/foxtools/FXLinkLabel.h>
#include <utils/gui/cursors/GUICursorSubSys.h>
#include <utils/gui/shortcuts/GUIShortcutsSubSys.h>
#include <utils/gui/div/GLHelper.h>
#include <utils/gui/div/GUIDesigns.h>
#include <utils/gui/div/GUIDialog_GLChosenEditor.h>
#include <utils/gui/div/GUIGlobalSelection.h>
#include <utils/gui/div/GUIUserIO.h>
#include <utils/gui/events/GUIEvent_Message.h>
#include <utils/gui/images/GUITextureSubSys.h>
#include <utils/gui/settings/GUICompleteSchemeStorage.h>
#include <utils/gui/settings/GUISettingsHandler.h>
#include <utils/gui/windows/GUIAppEnum.h>
#include <utils/gui/windows/GUIDialog_Options.h>
#include <utils/gui/windows/GUIPerspectiveChanger.h>
#include <utils/options/OptionsCont.h>
#include <utils/xml/XMLSubSys.h>

#include "GNEApplicationWindow.h"
#include "GNEEvent_NetworkLoaded.h"
#include "GNELoadThread.h"
#include "GNENet.h"
#include "GNEViewNet.h"
#include "GNEUndoList.h"
#include "GNEViewParent.h"

#ifdef HAVE_VERSION_H
#include <version.h>
#endif

// ===========================================================================
// GNEApplicationWindowHelper::ToolbarsGrip method definitions
// ===========================================================================

GNEApplicationWindowHelper::ToolbarsGrip::ToolbarsGrip(GNEApplicationWindow* GNEApp) :
    menu(nullptr),
    superModes(nullptr),
    saveElements(nullptr),
    navigation(nullptr),
    modes(nullptr),
    modeOptions(nullptr),
    intervalBar(nullptr),
    myGNEApp(GNEApp),
    myToolBarShellMenu(nullptr),
    myToolBarShellSuperModes(nullptr),
    myToolBarShellSaveElements(nullptr),
    myToolBarShellNavigation(nullptr),
    myToolBarShellModes(nullptr),
    myToolBarShellModeOptions(nullptr),
    myToolBarShellIntervalBar(nullptr) {
}


void
GNEApplicationWindowHelper::ToolbarsGrip::buildMenuToolbarsGrip() {
    // build menu bar (for File, edit, processing...) using specify design
    myToolBarShellMenu = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    menu = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellMenu, GUIDesignToolbarMenuBarNetedit);
    // declare toolbar grip for menu bar
    new FXToolBarGrip(menu, menu, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
}


void
GNEApplicationWindowHelper::ToolbarsGrip::buildViewParentToolbarsGrips() {
    // build menu bar for supermodes (next to menu bar)
    myToolBarShellSuperModes = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    superModes = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellSuperModes, GUIDesignToolBarRaisedSame);
    // declare toolbar grip for menu bar superModes
    new FXToolBarGrip(superModes, superModes, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
    // build menu bar for save elements (bot to menu bar)
    myToolBarShellSaveElements = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    saveElements = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellSaveElements, GUIDesignToolBarRaisedNext);
    // declare toolbar grip for menu bar saveElements
    new FXToolBarGrip(saveElements, saveElements, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
    // build menu bar for navigation
    myToolBarShellNavigation = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    navigation = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellNavigation, GUIDesignToolBarRaisedSame);
    // declare toolbar grip for menu bar navigation
    new FXToolBarGrip(navigation, navigation, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
    // build menu bar for modes
    myToolBarShellModes = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    modes = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellModes, GUIDesignToolBarRaisedSame);
    // declare toolbar grip for menu bar modes
    new FXToolBarGrip(modes, modes, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
    // build menu bar for mode Options
    myToolBarShellModeOptions = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    modeOptions = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellModeOptions, GUIDesignToolBarRaisedSame);
    // declare toolbar grip for menu bar modes
    new FXToolBarGrip(modeOptions, modeOptions, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
    // build menu bar for interal
    myToolBarShellIntervalBar = new FXToolBarShell(myGNEApp, GUIDesignToolBar);
    intervalBar = new FXMenuBar(myGNEApp->myTopDock, myToolBarShellModeOptions, GUIDesignToolBarRaisedNext);
    // declare toolbar grip for menu bar modes
    new FXToolBarGrip(intervalBar, intervalBar, FXMenuBar::ID_TOOLBARGRIP, GUIDesignToolBarGrip);
    // create menu bars
    superModes->create();
    saveElements->create();
    navigation->create();
    modes->create();
    modeOptions->create();
    intervalBar->create();
    // create toolbar shells
    myToolBarShellSuperModes->create();
    myToolBarShellSaveElements->create();
    myToolBarShellNavigation->create();
    myToolBarShellModes->create();
    myToolBarShellModeOptions->create();
    myToolBarShellIntervalBar->create();
    // recalc top dop after creating elements
    myGNEApp->myTopDock->recalc();
}


void
GNEApplicationWindowHelper::ToolbarsGrip::destroyParentToolbarsGrips() {
    // delete Menu bars
    delete superModes;
    delete saveElements;
    delete navigation;
    delete modes;
    delete modeOptions;
    delete intervalBar;
    // also delete toolbar shells to avoid floating windows
    delete myToolBarShellSuperModes;
    delete myToolBarShellSaveElements;
    delete myToolBarShellNavigation;
    delete myToolBarShellModes;
    delete myToolBarShellModeOptions;
    delete myToolBarShellIntervalBar;
    // recalc top dop after deleting elements
    myGNEApp->myTopDock->recalc();
}

// ===========================================================================
// GNEApplicationWindowHelper::MenuBarFile method definitions
// ===========================================================================

GNEApplicationWindowHelper::MenuBarFile::MenuBarFile(GNEApplicationWindow* GNEApp) :
    myRecentNets(GNEApp->getApp(), "nets"),
    myGNEApp(GNEApp)
{ }


void
GNEApplicationWindowHelper::MenuBarFile::buildRecentFiles(FXMenuPane* fileMenu) {
    FXMenuSeparator* sep1 = new FXMenuSeparator(fileMenu);
    sep1->setTarget(&myRecentConfigs);
    sep1->setSelector(FXRecentFiles::ID_ANYFILES);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_1);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_2);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_3);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_4);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_5);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_6);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_7);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_8);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_9);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentConfigs, FXRecentFiles::ID_FILE_10);
    new FXMenuCommand(fileMenu, "Clear Recent Configurat&ions", nullptr, &myRecentConfigs, FXRecentFiles::ID_CLEAR);
    myRecentConfigs.setTarget(myGNEApp);
    myRecentConfigs.setSelector(MID_RECENTFILE);
    FXMenuSeparator* sep2 = new FXMenuSeparator(fileMenu);
    sep2->setTarget(&myRecentNets);
    sep2->setSelector(FXRecentFiles::ID_ANYFILES);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_1);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_2);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_3);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_4);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_5);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_6);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_7);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_8);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_9);
    new FXMenuCommand(fileMenu, "", nullptr, &myRecentNets, FXRecentFiles::ID_FILE_10);
    new FXMenuCommand(fileMenu, "Cl&ear Recent Networks", nullptr, &myRecentNets, FXRecentFiles::ID_CLEAR);
    myRecentNets.setTarget(myGNEApp);
    myRecentNets.setSelector(MID_RECENTFILE);
}

// ---------------------------------------------------------------------------
// GNEViewNet::FileMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::FileMenuCommands::FileMenuCommands(GNEApplicationWindow* GNEApp) :
    saveAdditionals(nullptr),
    saveAdditionalsAs(nullptr),
    saveTLSPrograms(nullptr),
    saveDemandElements(nullptr),
    saveDemandElementsAs(nullptr),
    saveDataElements(nullptr),
    saveDataElementsAs(nullptr),
    myGNEApp(GNEApp) {
}


void
GNEApplicationWindowHelper::FileMenuCommands::buildFileMenuCommands(FXMenuPane* fileMenu) {
    new FXMenuCommand(fileMenu,
        "&New Network...\tCtrl+N\tCreate a new network.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_NET), myGNEApp, MID_HOTKEY_CTRL_N_NEWNETWORK);
    new FXMenuCommand(fileMenu,
        "&Open Network...\tCtrl+O\tOpen a SUMO network.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_NET), myGNEApp, MID_OPEN_NETWORK);
    new FXMenuCommand(fileMenu,
        "Open Netconvert Configura&tion...\tCtrl+Shift+O\tOpen a configuration file with NETCONVERT options.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_CONFIG), myGNEApp, MID_OPEN_CONFIG);
    new FXMenuCommand(fileMenu,
        "Import &Foreign Network...\t\tImport a foreign network such as OSM.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_NET), myGNEApp, MID_GNE_TOOLBARFILE_OPENFOREIGN);
    new FXMenuCommand(fileMenu,
        "&Reload\tCtrl+R\tReloads the network.",
        GUIIconSubSys::getIcon(GUIIcon::RELOAD), myGNEApp, MID_HOTKEY_CTRL_R_RELOAD);
    new FXMenuCommand(fileMenu,
        "&Save Network...\tCtrl+S\tSave the network.",
        GUIIconSubSys::getIcon(GUIIcon::SAVE), myGNEApp, MID_HOTKEY_CTRL_S_STOPSIMULATION_SAVENETWORK);
    new FXMenuCommand(fileMenu,
        "Save Net&work As...\tCtrl+Shift+S\tSave the network in another file.",
        GUIIconSubSys::getIcon(GUIIcon::SAVE), myGNEApp, MID_HOTKEY_CTRL_SHIFT_S_SAVENETWORK_AS);
    new FXMenuCommand(fileMenu,
        "Save plain XM&L...\tCtrl+L\tSave plain xml representation the network.",
        GUIIconSubSys::getIcon(GUIIcon::SAVE), myGNEApp, MID_HOTKEY_CTRL_L_SAVEASPLAINXML);
    new FXMenuCommand(fileMenu,
        "Save &joined junctions...\tCtrl+J\tSave log of joined junctions (allows reproduction of joins).",
        GUIIconSubSys::getIcon(GUIIcon::SAVE), myGNEApp, MID_HOTKEY_CTRL_J_SAVEJOINEDJUNCTIONS);
    // create TLS menu options
    myGNEApp->myFileMenuTLS = new FXMenuPane(myGNEApp);
    new FXMenuCommand(myGNEApp->myFileMenuTLS,
        "load TLS Programs...\tCtrl+K\tload TLS Programs in all Traffic Lights of the net.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_TLSPROGRAMS), myGNEApp, MID_HOTKEY_CTRL_K_OPENTLSPROGRAMS);
    saveTLSPrograms = new FXMenuCommand(myGNEApp->myFileMenuTLS,
        "Save TLS Programs \tCtrl+Shift+K\tSave TLS Programs of all Traffic Lights of the current net.",
        GUIIconSubSys::getIcon(GUIIcon::SAVE), myGNEApp, MID_HOTKEY_CTRL_SHIFT_K_SAVETLS);
    saveTLSPrograms->disable();
    new FXMenuCommand(myGNEApp->myFileMenuTLS,
        "Save TLS Programs As...\t\tSave TLS Programs of all Traffic Lights of the current net in another file.",
        GUIIconSubSys::getIcon(GUIIcon::SAVE), myGNEApp, MID_GNE_TOOLBARFILE_SAVETLSPROGRAMS_AS);
    new FXMenuCascade(fileMenu,
        "Traffic Lights",
        GUIIconSubSys::getIcon(GUIIcon::MODETLS), myGNEApp->myFileMenuTLS);
    // create Additionals menu options
    myGNEApp->myFileMenuAdditionals = new FXMenuPane(myGNEApp);
    new FXMenuCommand(myGNEApp->myFileMenuAdditionals,
        "Load A&dditionals...\tCtrl+A\tLoad additionals and shapes.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_ADDITIONALS), myGNEApp, MID_HOTKEY_CTRL_A_STARTSIMULATION_OPENADDITIONALS);
    saveAdditionals = new FXMenuCommand(myGNEApp->myFileMenuAdditionals,
        "Save Additionals\tCtrl+Shift+A\tSave additionals and shapes.",
        GUIIconSubSys::getIcon(GUIIcon::SAVEADDITIONALELEMENTS), myGNEApp, MID_HOTKEY_CTRL_SHIFT_A_SAVEADDITIONALS);
    saveAdditionals->disable();
    saveAdditionalsAs = new FXMenuCommand(myGNEApp->myFileMenuAdditionals,
        "Save Additionals As...\t\tSave additional elements in another file.",
        GUIIconSubSys::getIcon(GUIIcon::SAVEADDITIONALELEMENTS), myGNEApp, MID_GNE_TOOLBARFILE_SAVEADDITIONALS_AS);
    saveAdditionalsAs->disable();
    new FXMenuCascade(fileMenu, "Additionals and shapes", GUIIconSubSys::getIcon(GUIIcon::MODEADDITIONAL), myGNEApp->myFileMenuAdditionals);
    // create DemandElements menu options
    myGNEApp->myFileMenuDemandElements = new FXMenuPane(myGNEApp);
    new FXMenuCommand(myGNEApp->myFileMenuDemandElements,
        "Load demand elements...\tCtrl+D\tLoad demand elements.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_ADDITIONALS), myGNEApp, MID_HOTKEY_CTRL_D_SINGLESIMULATIONSTEP_OPENDEMANDELEMENTS);
    saveDemandElements = new FXMenuCommand(myGNEApp->myFileMenuDemandElements,
        "Save demand elements\tCtrl+Shift+D\tSave demand elements.",
        GUIIconSubSys::getIcon(GUIIcon::SAVEDEMANDELEMENTS), myGNEApp, MID_HOTKEY_CTRL_SHIFT_D_SAVEDEMANDELEMENTS);
    saveDemandElements->disable();
    saveDemandElementsAs = new FXMenuCommand(myGNEApp->myFileMenuDemandElements,
        "Save demand elements as...\t\tSave demand elements in another file.",
        GUIIconSubSys::getIcon(GUIIcon::SAVEDEMANDELEMENTS), myGNEApp, MID_GNE_TOOLBARFILE_SAVEDEMAND_AS);
    saveDemandElementsAs->disable();
    new FXMenuCascade(fileMenu,
        "Demand elements",
        GUIIconSubSys::getIcon(GUIIcon::SUPERMODEDEMAND), myGNEApp->myFileMenuDemandElements);
    // create DataElements menu options
    myGNEApp->myFileMenuDataElements = new FXMenuPane(myGNEApp);
    new FXMenuCommand(myGNEApp->myFileMenuDataElements,
        "Load data elements...\tCtrl+B\tLoad data elements.",
        GUIIconSubSys::getIcon(GUIIcon::OPEN_ADDITIONALS), myGNEApp, MID_HOTKEY_CTRL_B_EDITBREAKPOINT_OPENDATAELEMENTS);
    saveDataElements = new FXMenuCommand(myGNEApp->myFileMenuDataElements,
        "Save data elements\tCtrl+Shift+B\tSave data elements.",
        GUIIconSubSys::getIcon(GUIIcon::SAVEDATAELEMENTS), myGNEApp, MID_HOTKEY_CTRL_SHIFT_B_SAVEDATAELEMENTS);
    saveDataElements->disable();
    saveDataElementsAs = new FXMenuCommand(myGNEApp->myFileMenuDataElements,
        "Save data elements as...\t\tSave data elements in another file.",
        GUIIconSubSys::getIcon(GUIIcon::SAVEDATAELEMENTS), myGNEApp, MID_GNE_TOOLBARFILE_SAVEDATA_AS);
    saveDataElementsAs->disable();
    new FXMenuCascade(fileMenu,
        "Data elements",
        GUIIconSubSys::getIcon(GUIIcon::SUPERMODEDATA), myGNEApp->myFileMenuDataElements);
    // close network
    new FXMenuSeparator(fileMenu);
    new FXMenuCommand(fileMenu,
        "Close\tCtrl+W\tClose the net&work.",
        GUIIconSubSys::getIcon(GUIIcon::CLOSE), myGNEApp, MID_HOTKEY_CTRL_W_CLOSESIMULATION);
    // build recent files
    myGNEApp->myMenuBarFile.buildRecentFiles(fileMenu);
    new FXMenuSeparator(fileMenu);
    new FXMenuCommand(fileMenu,
        "&Quit\tCtrl+Q\tQuit the Application.",
        nullptr, myGNEApp, MID_HOTKEY_CTRL_Q_CLOSE, 0);
}

// ---------------------------------------------------------------------------
// GNEApplicationWindowHelper::EditMenuCommands::NetworkMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::EditMenuCommands::NetworkMenuCommands::NetworkMenuCommands(const EditMenuCommands* editMenuCommandsParent) :
    createEdgeMode(nullptr),
    moveMode(nullptr),
    deleteMode(nullptr),
    inspectMode(nullptr),
    selectMode(nullptr),
    connectMode(nullptr),
    prohibitionMode(nullptr),
    TLSMode(nullptr),
    additionalMode(nullptr),
    crossingMode(nullptr),
    TAZMode(nullptr),
    shapeMode(nullptr),
    myEditMenuCommandsParent(editMenuCommandsParent),
    myHorizontalSeparator(nullptr) {
}


void
GNEApplicationWindowHelper::EditMenuCommands::NetworkMenuCommands::showNetworkMenuCommands() {
    createEdgeMode->show();
    moveMode->show();
    deleteMode->show();
    inspectMode->show();
    selectMode->show();
    connectMode->show();
    prohibitionMode->show();
    TLSMode->show();
    additionalMode->show();
    crossingMode->show();
    TAZMode->show();
    shapeMode->show();
    // also show separator
    myHorizontalSeparator->show();
}


void
GNEApplicationWindowHelper::EditMenuCommands::NetworkMenuCommands::hideNetworkMenuCommands() {
    createEdgeMode->hide();
    moveMode->hide();
    deleteMode->hide();
    inspectMode->hide();
    selectMode->hide();
    connectMode->hide();
    prohibitionMode->hide();
    TLSMode->hide();
    additionalMode->hide();
    crossingMode->hide();
    TAZMode->hide();
    shapeMode->hide();
    // also hide separator
    myHorizontalSeparator->hide();
}


void
GNEApplicationWindowHelper::EditMenuCommands::NetworkMenuCommands::buildNetworkMenuCommands(FXMenuPane* editMenu) {
    // build every FXMenuCommand giving it a shortcut
    createEdgeMode = new FXMenuCommand(editMenu,
        "&Edge mode\tE\tCreate junction and edges.",
        GUIIconSubSys::getIcon(GUIIcon::MODECREATEEDGE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_E_MODES_EDGE_EDGEDATA);
    moveMode = new FXMenuCommand(editMenu,
        "&Move mode\tM\tMove elements.",
        GUIIconSubSys::getIcon(GUIIcon::MODEMOVE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_M_MODES_MOVE);
    deleteMode = new FXMenuCommand(editMenu,
        "&Delete mode\tD\tDelete elements.",
        GUIIconSubSys::getIcon(GUIIcon::MODEDELETE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_D_MODES_DELETE);
    inspectMode = new FXMenuCommand(editMenu,
        "&Inspect mode\tI\tInspect elements and change their attributes.",
        GUIIconSubSys::getIcon(GUIIcon::MODEINSPECT), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_I_MODES_INSPECT);
    selectMode = new FXMenuCommand(editMenu,
        "&Select mode\tS\tSelect elements.",
        GUIIconSubSys::getIcon(GUIIcon::MODESELECT), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_S_MODES_SELECT);
    connectMode = new FXMenuCommand(editMenu,
        "&Connection mode\tC\tEdit connections between lanes.",
        GUIIconSubSys::getIcon(GUIIcon::MODECONNECTION), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_C_MODES_CONNECT_PERSONPLAN);
    prohibitionMode = new FXMenuCommand(editMenu,
        "Pro&hibition mode\tW\tEdit connection prohibitions.",
        GUIIconSubSys::getIcon(GUIIcon::MODEPROHIBITION), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_W_MODES_PROHIBITION_PERSONTYPE);
    TLSMode = new FXMenuCommand(editMenu,
        "&Traffic light mode\tT\tEdit traffic lights over junctions.",
        GUIIconSubSys::getIcon(GUIIcon::MODETLS), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_T_MODES_TLS_VTYPE);
    additionalMode = new FXMenuCommand(editMenu,
        "&Additional mode\tA\tCreate additional elements.",
        GUIIconSubSys::getIcon(GUIIcon::MODEADDITIONAL), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_A_MODES_ADDITIONAL_STOP);
    crossingMode = new FXMenuCommand(editMenu,
        "C&rossing mode\tR\tCreate crossings between edges.",
        GUIIconSubSys::getIcon(GUIIcon::MODECROSSING), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_R_MODES_CROSSING_ROUTE_EDGERELDATA);
    TAZMode = new FXMenuCommand(editMenu,
        "TA&Z mode\tZ\tCreate Traffic Assignment Zones.",
        GUIIconSubSys::getIcon(GUIIcon::MODETAZ), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_Z_MODES_TAZ);
    shapeMode = new FXMenuCommand(editMenu,
        "&POI-Poly mode\tP\tCreate Points-Of-Interest and polygons.",
        GUIIconSubSys::getIcon(GUIIcon::MODEPOLYGON), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_P_MODES_POLYGON_PERSON);
    // build separator
    myHorizontalSeparator = new FXMenuSeparator(editMenu);
}

// ---------------------------------------------------------------------------
// GNEApplicationWindowHelper::EditMenuCommands::DemandMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::EditMenuCommands::DemandMenuCommands::DemandMenuCommands(const EditMenuCommands* editMenuCommandsParent) :
    routeMode(nullptr),
    vehicleMode(nullptr),
    vehicleTypeMode(nullptr),
    stopMode(nullptr),
    personTypeMode(nullptr),
    personMode(nullptr),
    personPlanMode(nullptr),
    myEditMenuCommandsParent(editMenuCommandsParent),
    myHorizontalSeparator(nullptr) {
}


void
GNEApplicationWindowHelper::EditMenuCommands::DemandMenuCommands::showDemandMenuCommands() {
    routeMode->show();
    vehicleMode->show();
    vehicleTypeMode->show();
    stopMode->show();
    personTypeMode->show();
    personMode->show();
    personPlanMode->show();
    // also show separator
    myHorizontalSeparator->show();
}


void
GNEApplicationWindowHelper::EditMenuCommands::DemandMenuCommands::hideDemandMenuCommands() {
    routeMode->hide();
    vehicleMode->hide();
    vehicleTypeMode->hide();
    stopMode->hide();
    personTypeMode->hide();
    personMode->hide();
    personPlanMode->hide();
    // also hide separator
    myHorizontalSeparator->hide();
}


void
GNEApplicationWindowHelper::EditMenuCommands::DemandMenuCommands::buildDemandMenuCommands(FXMenuPane* editMenu) {
    // build every FXMenuCommand giving it a shortcut
    routeMode = new FXMenuCommand(editMenu,
        "Route mode\tR\tCreate Routes.",
        GUIIconSubSys::getIcon(GUIIcon::MODEROUTE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_R_MODES_CROSSING_ROUTE_EDGERELDATA);
    vehicleMode = new FXMenuCommand(editMenu,
        "Vehicle mode\tV\tCreate vehicles.",
        GUIIconSubSys::getIcon(GUIIcon::MODEVEHICLE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_V_MODES_VEHICLE);
    vehicleTypeMode = new FXMenuCommand(editMenu,
        "Vehicle type mode\tT\tCreate vehicle types.",
        GUIIconSubSys::getIcon(GUIIcon::MODEVEHICLETYPE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_T_MODES_TLS_VTYPE);
    stopMode = new FXMenuCommand(editMenu,
        "Stop mode\tA\tCreate stops.",
        GUIIconSubSys::getIcon(GUIIcon::MODESTOP), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_A_MODES_ADDITIONAL_STOP);
    personTypeMode = new FXMenuCommand(editMenu,
        "Person type mode\tW\tCreate person types.",
        GUIIconSubSys::getIcon(GUIIcon::MODEPERSONTYPE), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_W_MODES_PROHIBITION_PERSONTYPE);
    personMode = new FXMenuCommand(editMenu,
        "Person mode\tP\tCreate persons.",
        GUIIconSubSys::getIcon(GUIIcon::MODEPERSON), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_P_MODES_POLYGON_PERSON);
    personPlanMode = new FXMenuCommand(editMenu,
        "Person plan mode\tC\tCreate person plans.",
        GUIIconSubSys::getIcon(GUIIcon::MODEPERSONPLAN), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_C_MODES_CONNECT_PERSONPLAN);
    // build separator
    myHorizontalSeparator = new FXMenuSeparator(editMenu);
}

// ---------------------------------------------------------------------------
// GNEApplicationWindowHelper::EditMenuCommands::DataMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::EditMenuCommands::DataMenuCommands::DataMenuCommands(const EditMenuCommands* editMenuCommandsParent) :
    edgeData(nullptr),
    edgeRelData(nullptr),
    myEditMenuCommandsParent(editMenuCommandsParent),
    myHorizontalSeparator(nullptr) {
}


void
GNEApplicationWindowHelper::EditMenuCommands::DataMenuCommands::showDataMenuCommands() {
    edgeData->show();
    edgeRelData->show();
    // also show separator
    myHorizontalSeparator->show();
}


void
GNEApplicationWindowHelper::EditMenuCommands::DataMenuCommands::hideDataMenuCommands() {
    edgeData->hide();
    edgeRelData->hide();
    // also hide separator
    myHorizontalSeparator->hide();
}


void
GNEApplicationWindowHelper::EditMenuCommands::DataMenuCommands::buildDataMenuCommands(FXMenuPane* editMenu) {
    // build every FXMenuCommand giving it a shortcut
    edgeData = new FXMenuCommand(editMenu,
        "Edge Data Mode\tE\tCreate edge datas.",
        GUIIconSubSys::getIcon(GUIIcon::MODEEDGEDATA), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_E_MODES_EDGE_EDGEDATA);
    edgeRelData  = new FXMenuCommand(editMenu,
        "Edge Rel Data Mode\tR\tCreate edge relation datas.",
        GUIIconSubSys::getIcon(GUIIcon::MODEEDGERELDATA), myEditMenuCommandsParent->myGNEApp, MID_HOTKEY_R_MODES_CROSSING_ROUTE_EDGERELDATA);
    // build separator
    myHorizontalSeparator = new FXMenuSeparator(editMenu);
}

// ---------------------------------------------------------------------------
// GNEApplicationWindowHelper::EditMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::EditMenuCommands::EditMenuCommands(GNEApplicationWindow* GNEApp) :
    myGNEApp(GNEApp),
    networkMenuCommands(this),
    demandMenuCommands(this),
    dataMenuCommands(this),
    undoLastChange(nullptr),
    redoLastChange(nullptr),
    editViewScheme(nullptr),
    editViewPort(nullptr),
    toogleGrid(nullptr),
    loadAdditionalsInSUMOGUI(nullptr),
    loadDemandInSUMOGUI(nullptr),
    openInSUMOGUI(nullptr) {
}


void
GNEApplicationWindowHelper::EditMenuCommands::buildEditMenuCommands(FXMenuPane* editMenu) {
    // build undo/redo command
    undoLastChange = new FXMenuCommand(editMenu,
        "&Undo\tCtrl+Z\tUndo the last change.",
        GUIIconSubSys::getIcon(GUIIcon::UNDO), myGNEApp, MID_HOTKEY_CTRL_Z_UNDO);
    redoLastChange = new FXMenuCommand(editMenu,
        "&Redo\tCtrl+Y\tRedo the last change.",
        GUIIconSubSys::getIcon(GUIIcon::REDO), myGNEApp, MID_HOTKEY_CTRL_Y_REDO);
    // build separator
    new FXMenuSeparator(editMenu);
    // build Supermode commands and hide it
    myGNEApp->mySupermodeCommands.buildSupermodeCommands(editMenu);
    myGNEApp->mySupermodeCommands.hideSupermodeCommands();
    // build Network modes commands and hide it
    networkMenuCommands.buildNetworkMenuCommands(editMenu);
    networkMenuCommands.hideNetworkMenuCommands();
    // build Demand Modes commands
    demandMenuCommands.buildDemandMenuCommands(editMenu);
    demandMenuCommands.hideDemandMenuCommands();
    // build Data Modes commands
    dataMenuCommands.buildDataMenuCommands(editMenu);
    dataMenuCommands.hideDataMenuCommands();
    // build rest of menu commands
    editViewScheme = new FXMenuCommand(editMenu,
        "Edit Visualisation\tF9\tOpens a dialog for editing visualization settings.",
        nullptr, myGNEApp, MID_HOTKEY_F9_EDIT_VIEWSCHEME);
    editViewPort = new FXMenuCommand(editMenu,
        "Edit Viewport\tCtrl+I\tOpens a dialog for editing viewing are, zoom and rotation.",
        nullptr, myGNEApp, MID_HOTKEY_CTRL_I_EDITVIEWPORT);
    toogleGrid = new FXMenuCommand(editMenu,
        "Toggle Grid\tCtrl+G\tToggles background grid (and snap-to-grid functionality).",
        nullptr, myGNEApp, MID_HOTKEY_CTRL_G_GAMINGMODE_TOOGLEGRID);
    // add open in sumo options
    new FXMenuSeparator(editMenu);
    loadAdditionalsInSUMOGUI = new FXMenuCheck(editMenu,
        "Load additionals in SUMO-GUI\t\tLoad additionals in SUMO-GUI.",
        myGNEApp, MID_GNE_TOOLBAREDIT_LOADADDITIONALS);
    loadAdditionalsInSUMOGUI->setCheck(TRUE);
    loadDemandInSUMOGUI = new FXMenuCheck(editMenu,
        "Load demand in SUMO-GUI\t\tLoad demand in SUMO-GUI.",
        myGNEApp, MID_GNE_TOOLBAREDIT_LOADDEMAND);
    loadDemandInSUMOGUI->setCheck(TRUE);
    openInSUMOGUI = new FXMenuCommand(editMenu,
        "Open in SUMO-GUI\tCtrl+T\tOpens the SUMO-GUI application with the current network.",
        GUIIconSubSys::getIcon(GUIIcon::SUMO_MINI), myGNEApp, MID_HOTKEY_CTRL_T_OPENSUMONETEDIT);
}

// ---------------------------------------------------------------------------
// GNEViewNet::ProcessingMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::ProcessingMenuCommands::ProcessingMenuCommands(GNEApplicationWindow* GNEApp) :
    computeNetwork(nullptr),
    computeNetworkVolatile(nullptr),
    cleanJunctions(nullptr),
    joinJunctions(nullptr),
    clearInvalidCrossings(nullptr),
    computeDemand(nullptr),
    cleanRoutes(nullptr),
    joinRoutes(nullptr),
    clearInvalidDemandElements(nullptr),
    optionMenus(nullptr),
    myGNEApp(GNEApp) {
}


void
GNEApplicationWindowHelper::ProcessingMenuCommands::buildProcessingMenuCommands(FXMenuPane* fileMenu) {
    // build network processing menu commands
    computeNetwork = new FXMenuCommand(fileMenu,
        "Compute Junctions\tF5\tComputes junction shape and logic.",
        GUIIconSubSys::getIcon(GUIIcon::COMPUTEJUNCTIONS), myGNEApp, MID_HOTKEY_F5_COMPUTE_NETWORK_DEMAND);
    computeNetworkVolatile = new FXMenuCommand(fileMenu,
        "Compute Junctions with volatile options\tShift+F5\tComputes junction shape and logic using volatile junctions.",
        GUIIconSubSys::getIcon(GUIIcon::COMPUTEJUNCTIONS), myGNEApp, MID_HOTKEY_SHIFT_F5_COMPUTEJUNCTIONS_VOLATILE);
    cleanJunctions = new FXMenuCommand(fileMenu,
        "Clean Junctions\tF6\tRemoves solitary junctions.",
        GUIIconSubSys::getIcon(GUIIcon::CLEANJUNCTIONS), myGNEApp, MID_HOTKEY_F6_CLEAN_SOLITARYJUNCTIONS_UNUSEDROUTES);
    joinJunctions = new FXMenuCommand(fileMenu,
        "Join Selected Junctions\tF7\tJoins selected junctions into a single junction.",
        GUIIconSubSys::getIcon(GUIIcon::JOINJUNCTIONS), myGNEApp, MID_HOTKEY_F7_JOIN_SELECTEDJUNCTIONS_ROUTES);
    clearInvalidCrossings = new FXMenuCommand(fileMenu,
        "Clean invalid crossings\tF8\tClear invalid crossings.",
        GUIIconSubSys::getIcon(GUIIcon::JOINJUNCTIONS), myGNEApp, MID_HOTKEY_F8_CLEANINVALID_CROSSINGS_DEMANDELEMENTS);
    // build demand  processing menu commands
    computeDemand = new FXMenuCommand(fileMenu,
        "Compute demand\tF5\tComputes demand elements.",
        GUIIconSubSys::getIcon(GUIIcon::COMPUTEDEMAND), myGNEApp, MID_HOTKEY_F5_COMPUTE_NETWORK_DEMAND);
    cleanRoutes = new FXMenuCommand(fileMenu,
        "Clean routes\tF6\tRemoves routes without vehicles.",
        GUIIconSubSys::getIcon(GUIIcon::CLEANROUTES), myGNEApp, MID_HOTKEY_F6_CLEAN_SOLITARYJUNCTIONS_UNUSEDROUTES);
    joinRoutes = new FXMenuCommand(fileMenu,
        "Join routes\tF7\tJoins routes with the same edges.",
        GUIIconSubSys::getIcon(GUIIcon::JOINROUTES), myGNEApp, MID_HOTKEY_F7_JOIN_SELECTEDJUNCTIONS_ROUTES);
    clearInvalidDemandElements = new FXMenuCommand(fileMenu,
        "Clean invalid route elements\tF8\tClear elements with an invalid path (routes, Trips, Flows...).",
        GUIIconSubSys::getIcon(GUIIcon::JOINJUNCTIONS), myGNEApp, MID_HOTKEY_F8_CLEANINVALID_CROSSINGS_DEMANDELEMENTS);
    // add separator (because optionsmenu is commmon for Network and Demand modes
    new FXMenuSeparator(fileMenu);
    optionMenus = new FXMenuCommand(fileMenu,
        "Options\tF10\t\tConfigure Processing Options.",
        GUIIconSubSys::getIcon(GUIIcon::OPTIONS), myGNEApp, MID_HOTKEY_F10_OPTIONSMENU);
}


void
GNEApplicationWindowHelper::ProcessingMenuCommands::showNetworkProcessingMenuCommands() {
    // first enable menu commands
    computeNetwork->enable();
    computeNetworkVolatile->enable();
    cleanJunctions->enable();
    joinJunctions->enable();
    clearInvalidCrossings->enable();
    // now show it
    computeNetwork->show();
    computeNetworkVolatile->show();
    cleanJunctions->show();
    joinJunctions->show();
    clearInvalidCrossings->show();
}


void
GNEApplicationWindowHelper::ProcessingMenuCommands::hideNetworkProcessingMenuCommands() {
    // first disable menu commands
    computeNetwork->disable();
    computeNetworkVolatile->disable();
    cleanJunctions->disable();
    joinJunctions->disable();
    clearInvalidCrossings->disable();
    // now hide it
    computeNetwork->hide();
    computeNetworkVolatile->hide();
    cleanJunctions->hide();
    joinJunctions->hide();
    clearInvalidCrossings->hide();
}


void
GNEApplicationWindowHelper::ProcessingMenuCommands::showDemandProcessingMenuCommands() {
    // first enable menu commands
    computeDemand->enable();
    cleanRoutes->enable();
    joinRoutes->enable();
    clearInvalidDemandElements->enable();
    // now show it
    computeDemand->show();
    cleanRoutes->show();
    joinRoutes->show();
    clearInvalidDemandElements->show();
}


void
GNEApplicationWindowHelper::ProcessingMenuCommands::hideDemandProcessingMenuCommands() {
    // first disable menu commands
    computeDemand->disable();
    cleanRoutes->disable();
    joinRoutes->disable();
    clearInvalidDemandElements->disable();
    // now hide it
    computeDemand->hide();
    cleanRoutes->hide();
    joinRoutes->hide();
    clearInvalidDemandElements->hide();
}


void 
GNEApplicationWindowHelper::ProcessingMenuCommands::showDataProcessingMenuCommands() {

}


void 
GNEApplicationWindowHelper::ProcessingMenuCommands::hideDataProcessingMenuCommands() {

}

// ---------------------------------------------------------------------------
// GNEViewNet::LocateMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::LocateMenuCommands::LocateMenuCommands(GNEApplicationWindow* GNEApp) :
    myGNEApp(GNEApp) {
}


void
GNEApplicationWindowHelper::LocateMenuCommands::buildLocateMenuCommands(FXMenuPane* locateMenu) {
    // build locate menu commands
    new FXMenuCommand(locateMenu,
        "Locate &Junctions\tShift+J\tOpen a dialog for locating a Junction.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEJUNCTION), myGNEApp, MID_LOCATEJUNCTION);
    new FXMenuCommand(locateMenu,
        "Locate &Edges\tShift+E\tOpen a dialog for locating an Edge.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEEDGE), myGNEApp, MID_LOCATEEDGE);
    new FXMenuCommand(locateMenu,
        "Locate &Vehicles\tShift+V\tOpen a dialog for locating a Vehicle.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEVEHICLE), myGNEApp, MID_LOCATEVEHICLE);
    new FXMenuCommand(locateMenu,
        "Locate &Person\tShift+P\tOpen a dialog for locating a Person.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEPERSON), myGNEApp, MID_LOCATEPERSON);
    new FXMenuCommand(locateMenu,
        "Locate &Route\tShift+R\tOpen a dialog for locating a Route.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEROUTE), myGNEApp, MID_LOCATEROUTE);
    new FXMenuCommand(locateMenu,
        "Locate &Stops\tShift+S\tOpen a dialog for locating a Stop.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATESTOP), myGNEApp, MID_LOCATESTOP);
    new FXMenuCommand(locateMenu,
        "Locate &TLS\tShift+T\tOpen a dialog for locating a Traffic Light.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATETLS), myGNEApp, MID_LOCATETLS);
    new FXMenuCommand(locateMenu,
        "Locate &Additional\tShift+A\tOpen a dialog for locating an Additional Structure.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEADD), myGNEApp, MID_LOCATEADD);
    new FXMenuCommand(locateMenu,
        "Locate P&oI\tShift+O\tOpen a dialog for locating a Point of Interest.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEPOI), myGNEApp, MID_LOCATEPOI);
    new FXMenuCommand(locateMenu,
        "Locate Po&lygon\tShift+L\tOpen a dialog for locating a Polygon.",
        GUIIconSubSys::getIcon(GUIIcon::LOCATEPOLY), myGNEApp, MID_LOCATEPOLY);
}

// ---------------------------------------------------------------------------
// GNEViewNet::WindowsMenuCommands - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::WindowsMenuCommands::WindowsMenuCommands(GNEApplicationWindow* GNEApp) :
    myGNEApp(GNEApp) {
}


void
GNEApplicationWindowHelper::WindowsMenuCommands::buildWindowsMenuCommands(FXMenuPane* windowsMenu) {
    // build windows menu commands
    new FXMenuCheck(windowsMenu,
        "&Show Status Line\t\tToggle this Status Bar on/off.",
        myGNEApp->myStatusbar, FXWindow::ID_TOGGLESHOWN);
    new FXMenuCheck(windowsMenu,
        "Show &Message Window\t\tToggle the Message Window on/off.",
        myGNEApp->myMessageWindow, FXWindow::ID_TOGGLESHOWN);
    new FXMenuCommand(windowsMenu,
        "&Clear Message Window\t\tClear the message window.",
        nullptr, myGNEApp, MID_CLEARMESSAGEWINDOW);
}

// ---------------------------------------------------------------------------
// GNEViewNet::NetworkCheckableButtons - methods
// ---------------------------------------------------------------------------

GNEApplicationWindowHelper::SupermodeCommands::SupermodeCommands(GNEApplicationWindow* GNEApp) :
    networkMode(nullptr),
    demandMode(nullptr),
    dataMode(nullptr),
    myGNEApp(GNEApp),
    myHorizontalSeparator(nullptr) {
}


void
GNEApplicationWindowHelper::SupermodeCommands::showSupermodeCommands() {
    networkMode->show();
    demandMode->show();
    dataMode->show();
    // also show separator
    myHorizontalSeparator->show();
}


void
GNEApplicationWindowHelper::SupermodeCommands::hideSupermodeCommands() {
    networkMode->hide();
    demandMode->hide();
    dataMode->hide();
    // also hide separator
    myHorizontalSeparator->hide();
}


void
GNEApplicationWindowHelper::SupermodeCommands::buildSupermodeCommands(FXMenuPane* editMenu) {
    // build supermode menu commands
    networkMode = new FXMenuCommand(editMenu, 
        "&Network mode\tF2\tSelect network mode.",
        GUIIconSubSys::getIcon(GUIIcon::SUPERMODENETWORK), myGNEApp, MID_HOTKEY_F2_SUPERMODE_NETWORK);
    demandMode = new FXMenuCommand(editMenu, 
        "&Demand mode\tF3\tSelect demand mode.",
        GUIIconSubSys::getIcon(GUIIcon::SUPERMODEDEMAND), myGNEApp, MID_HOTKEY_F3_SUPERMODE_DEMAND);
    dataMode = new FXMenuCommand(editMenu, 
        "&Data mode\tF4\tSelect data mode.",
        GUIIconSubSys::getIcon(GUIIcon::SUPERMODEDATA), myGNEApp, MID_HOTKEY_F4_SUPERMODE_DATA);
    // build separator
    myHorizontalSeparator = new FXMenuSeparator(editMenu);
}

/****************************************************************************/