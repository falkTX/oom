//=========================================================
//  OOMidi
//  OpenOctave Midi and Audio Editor
//    $Id: lcombo.h,v 1.1.1.1.2.3 2009/07/01 22:14:56 spamatica Exp $
//  (C) Copyright 2000 Werner Schweer (ws@seh.de)
//=========================================================

#ifndef __LCOMBO_H__
#define __LCOMBO_H__

#include <QAbstractItemView>
#include <QComboBox>
#include <QVariant>

class QString;


//---------------------------------------------------------
//   LabelCombo
//---------------------------------------------------------

class LabelCombo : public QWidget
{
    QComboBox* box;
    Q_OBJECT

signals:
    void activated(int);

public slots:

    void clearFocus()
    {
        box->clearFocus();
    }
    void setCurrentIndex(int i);

public:
    LabelCombo(const QString& label, QWidget* parent,
            const char* name = 0);

    void addItem(const QString& txt, const QVariant & userData = QVariant())
    {
        box->addItem(txt, userData);
    }

    void insertItem(int index, const QString& txt, const QVariant & userData = QVariant())
    {
        box->insertItem(index, txt, userData);
    }
    //void setListBox(Q3ListBox* lb) { box->setListBox(lb); } // ddskrjo

    void setView(QAbstractItemView* v)
    {
        box->setModel(v->model());
        box->setView(v);
    } // p4.0.3

    void setFocusPolicy(Qt::FocusPolicy fp)
    {
        box->setFocusPolicy(fp);
    }
};

#endif
