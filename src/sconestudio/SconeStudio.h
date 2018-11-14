/*
** SconeStudio.h
**
** Copyright (C) 2013-2018 Thomas Geijtenbeek. All rights reserved.
**
** This file is part of SCONE. For more information, see http://scone.software.
*/

#ifndef SCONESTUDIO_H
#define SCONESTUDIO_H

#include <QtCore/QtGlobal>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtWidgets/QMainWindow>

#include "ui_SconeStudio.h"

#include "scone/core/PropNode.h"
#include "scone/core/Statistic.h"

#include "xo/time/timer.h"
#include "xo/numerical/delta.h"
#include "ProgressDockWidget.h"
#include "QCodeEditor.h"
#include "SettingsEditor.h"
#include "xo/system/log_sink.h"
#include "QCompositeMainWindow.h"
#include "QDataAnalysisView.h"
#include "ResultsFileSystemModel.h"
#include "StudioModel.h"
#include "simvis/plane.h"
#include "simvis/prerequisites.h"

using scone::TimeInSeconds;

class SconeStudio : public QCompositeMainWindow
{
	Q_OBJECT

public:
	SconeStudio(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SconeStudio();

	bool init(osgViewer::ViewerBase::ThreadingModel threadingModel);

public slots:
	void runSimulation( const QString& filename );
	void activateBrowserItem( QModelIndex idx );
	void selectBrowserItem( const QModelIndex& idx, const QModelIndex& idxold );
	void resultsSelectionChanged( const QItemSelection& newitem, const QItemSelection& olditem ) {}
	void start();
	void stop();
	void refreshAnalysis();
	void fileOpen();
	void fileOpen( const QString& filename );
	void fileOpenRecent();
	void fileSave();
	void fileSaveAs();
	void fileExit();
	void helpAbout() {}
	void runScenario();
	void optimizeScenario();
	void optimizeScenarioMultiple();
	void abortOptimizations();
	void updateBackgroundTimer();
	void updateOptimizations();
	void createVideo();
	void captureImage();
	void tabCloseRequested( int idx );
	void updateViewSettings();
	void showSettingsDialog() { settings.showDialog( this ); }
	void setPlaybackTime( TimeInSeconds t ) { setTime( t, true ); }
	void fixViewCheckboxes();
	void updateTabTitles();

public:
	bool close_all;
	bool isRecording() { return !captureFilename.isEmpty(); }
	bool isEvalutating() { return model && model->IsEvaluating(); }

private:
	void evaluate();
	void setTime( TimeInSeconds t, bool update_vis );
	QCodeEditor* getActiveScenario( bool show_error = false );
	bool requestSaveChanges( QCodeEditor* s );
	int getTabIndex( QCodeEditor* s );
	void addProgressDock( ProgressDockWidget* pdw );
	void addRecentFile( const QString& filename );
	void updateRecentFilesMenu();
	QStringList recentFiles;

	vis::scene scene_;
	vis::plane ground_plane;
	std::unique_ptr< scone::StudioModel > model;

	bool createModel( const String& par_file, bool force_evaluation = false );

	Ui::SconeStudioClass ui;

	QTimer backgroundUpdateTimer;

	double slomo_factor;
	TimeInSeconds current_time;
	TimeInSeconds evaluation_time_step;
	xo::delta< scone::Vec3 > com_delta;

	QString currentParFile;

	std::vector< ProgressDockWidget* > optimizations;
	std::vector< QCodeEditor* > scenarios;

	SconeStorageDataModel storageModel;
	QDataAnalysisView* analysisView;

	ResultsFileSystemModel* resultsModel;
	QString captureFilename;
	QProcess* captureProcess;
	QDir captureImageDir;
	void finalizeCapture();

	scone::SettingsEditor settings;

protected:
	virtual void closeEvent( QCloseEvent * ) override;
};

#endif // SCONESTUDIO_H
