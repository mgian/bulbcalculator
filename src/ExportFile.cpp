/*
Copyright (C) 2011-2014  Gianluca Montecchi <gian@grys.it>

This file is part of BulbCalculator.

BulbCalculator is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 2 of the License, or (at your
option) any later version.

BulbCalculator is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with BulbCalculator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QString>
#include <QDialog>
#include <QFileDialog>
#include <QtWidgets>
#include <QMessageBox>

#include "../include/ExportFile.h"
#include "../include/Utils.h"

ExportFile::ExportFile() {

}

void ExportFile::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;

}
void ExportFile::ExportAsciiSTL(QString FileName) {

    long w, mult;
    double step;
    QSize t;
    double x;


    QFile fOut(FileName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fOutStream(&fOut);

    w = 2.0;

    mult = this->bc->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);

    x = 0.0;

    profile_data& pdi(this->bc->naca_profile[(unsigned)((double)0.02*mult)]);
    profile_data& pde(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    x = x + step;

    for(int i=1; i<mult; i++) {
        profile_data& pdi(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        profile_data& pde(this->bc->naca_profile[(unsigned)((double)(i+1)*mult)]);
        x = x + step;
    }

    fOutStream << "solid " << this->bc->naca_profile.foil_name.c_str() << "\n";

/*


    clIter.Begin();
    clEnd.End();
    while (clIter < clEnd) {
        pclFacet = &(*clIter);

        // normal^M
        rstrOut << "  facet normal " << pclFacet->GetNormal().x << " "
                                     << pclFacet->GetNormal().y << " "
                                     << pclFacet->GetNormal().z << std::endl;
        rstrOut << "    outer loop" << std::endl;

        // vertices^M
        for (i = 0; i < 3; i++) {
            rstrOut << "      vertex "  << pclFacet->_aclPoints[i].x << " "
                                        << pclFacet->_aclPoints[i].y << " "
                                        << pclFacet->_aclPoints[i].z << std::endl;
        }
        rstrOut << "    endloop" << std::endl;
        rstrOut << "  endfacet" << std::endl;

        ++clIter;
        seq.next(true);// allow to cancel
    }
*/
    fOut.close();
}
