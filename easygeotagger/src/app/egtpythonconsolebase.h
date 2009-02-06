/*
** File: "egtpythonconsolebase.h
** Author(s): Peter J. Ersts (ersts at amnh.org), Roberto Garcia-Yunta
** Creation Date: 2008-11-18
**
** Copyright (c) 2008-2009, American Museum of Natural History. All rights reserved.
**
** This library/program is free software; you can redistribute it
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library/program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** This work was made possible through a grant by the the John D. and
** Catherine T. MacArthur Foundation with additional support from the
** Center For Biodiversity and Conservation and The Spanish Ministry of
** Science and Innovation's INTEGRANTS program.
**
**/
#ifndef EGTPYTHONCONSOLEBASE_H
#define EGTPYTHONCONSOLEBASE_H

#include <Python.h>
#include <QWidget>

#include "ui_egtpythonconsoledialog.h"
#include "egtapplicationinterface.h"

class MS_DLL_SPEC EgtPythonConsoleBase : public QWidget, Ui::EgtPythonConsoleDialog
{
    Q_OBJECT

  public:
    /*! \brief Constructor */
    EgtPythonConsoleBase( EgtApplicationInterface* );

    /*! \brief Execute set of python commands */
    bool runCommand( QString theCommand );

    /*! \brief Get last error - Borrowed from QGIS http://qgis.org */
    bool getError( QString&, QString&);

    /*! \brief Get last error - Borrowed from QGIS http://qgis.org */
    QString getTypeAsString( PyObject* );

    /*! \brief Were the required python libraries found */
    bool pythonFound() { return cvPythonFound; }

  public slots:
    /*! \brief Slot called to execute code from the console */
    void on_pbtnRun_clicked();

  private:
    PyObject* cvMainModule;
    PyObject* cvDictionary;
    bool cvPythonFound;

};
#endif // EGTPYTHONCONSOLEBASE_H
