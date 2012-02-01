//===========================================================
//  OOMidi
//  OpenOctave Midi and Audio Editor
//  (C) Copyright 2011 Andrew Williams & Christopher Cherrett
//===========================================================

#ifndef _OOM_CREATE_TRACKS_DIALOG_
#define _OOM_CREATE_TRACKS_DIALOG_

#include "ui_createtrackbase.h"
#include <QMap>
#include "TrackManager.h"

#define InstrumentTypeRole Qt::UserRole+4
#define InstrumentNameRole Qt::UserRole+5
#define DeviceTypeRole Qt::UserRole+6
#define DeviceNameRole Qt::UserRole+7

class QShowEvent;
class QSize;
//class SynthPluginDevice;

class CreateTrackDialog : public QDialog, public Ui::CreateTrackBase {
	Q_OBJECT

	int m_height;
	int m_width;

	int m_insertType;
	int m_insertPosition;
	bool m_templateMode;
	
	bool m_createMidiInputDevice;
	bool m_createMidiOutputDevice;
	bool m_midiSameIO;
	
	int m_allChannelBit;

	int m_midiInPort;
	int m_midiOutPort;
	
	bool m_createTrackOnly;
	int m_showJackAliases;
	bool m_instrumentLoaded;
	
	QMap<int, QString> m_currentMidiInputList;
	QMap<int, QString> m_currentMidiOutputList;

	VirtualTrack *m_vtrack;
    //SynthPluginDevice *m_lastSynth;

	void importInputs();
	void importOutputs();

	void populateInputList();
	void populateOutputList();
	void populateNewInputList();
	void populateNewOutputList();
	void populateInstrumentList();
	void populateMonitorList();
	void populateBussList();
	
	void updateVisibleElements();
	
	int getFreeMidiPort();
	
	void initDefaults();
	void cleanup();

private slots:
	void addTrack();
	void cancelSelected();
	void updateInputSelected(bool);
	void updateOutputSelected(bool);
	void updateBussSelected(bool);
	void updateInstrument(int);
	void trackTypeChanged(int);
	void trackNameEdited();

protected:
	virtual void showEvent(QShowEvent*);

signals:
	void trackAdded(qint64);

public:
	CreateTrackDialog(int type = 0, int pos = -1, QWidget* parent = 0);
	CreateTrackDialog(VirtualTrack** vt, int type = 0, int pos = -1, QWidget* parent = 0);
	~CreateTrackDialog(){}
	void lockType(bool);
};

#endif