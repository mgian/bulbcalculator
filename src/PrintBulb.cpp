/*
Copyright (C) 2011  Gianluca Montecchi <gian@grys.it>

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

#include <QtGui>
#include <QMessageBox>

#include "../include/PrintBulb.h"
#include "../include/Utils.h"

PrintDraw::PrintDraw(BulbCalculator *bc, int pt) {

    QPrintPreviewDialog preview;
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(PrintPreview(QPrinter*)));
    this->bc = bc;
    this->PrintType = pt;
    preview.exec();

}


void PrintDraw::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;

}

void PrintDraw::PrintPreview(QPrinter *pr) {

    int o;
    QPainter aPainter;

    float aLogicalWidth = pr->widthMM() * 100;
    float aLogicalHeight = pr->heightMM() * 100;
    float aPhysicalWidth = pr->width();
    float aPhysicalHeight = pr->height();

    if (this->PrintType == P_DATA) {
        this->DrawBulbData(pr);
    } else {
        pr->setPageMargins(0.0,0.0,0.0,0.0, pr->Millimeter);
        aPainter.begin(pr);
        o = pr->orientation();
        if (o == QPrinter::Landscape) {
            aPainter.setWindow(0, 0, aLogicalHeight, aLogicalWidth);
            aPainter.setViewport(0, 0, aPhysicalHeight,aPhysicalWidth);

        }
        if (o == QPrinter::Portrait) {
            aPainter.setWindow(0, 0, aLogicalWidth, aLogicalHeight);
            aPainter.setViewport(0, 0, aPhysicalWidth, aPhysicalHeight);

        }

        this->CheckBulbDimension(&aPainter, pr->orientation());
        switch(this->PrintType) {
            case P_SECTIONS_TOP:
                this->DrawBulbSection(&aPainter, P_SECTIONS_TOP, pr->orientation());
                break;
            case P_SECTIONS_SIDE:
                this->DrawBulbSection(&aPainter, P_SECTIONS_SIDE, pr->orientation());
                break;
            case P_LINESPLAN_TOP:
                this->DrawBulbLinesPlanTop(&aPainter, pr->orientation());
                break;
            case P_LINESPLAN_SIDE:
                this->DrawBulbLinesPlanSide(&aPainter, pr->orientation());
                break;
        }
        aPainter.end();
    }

}

void PrintDraw::DrawBulbData(QPrinter *prn) {

    float wr;
    float w, h, offset;

    int x, y;
    wr = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    //wr *= 1000;
    qDebug()<< wr;
    QFont TextFont("Helvetica", 12);
    QFont TextFontBold("Helvetica", 12, QFont::Bold);

    prn->setPageSize(prn->A4);
    prn->setOutputFormat ( QPrinter::NativeFormat );

    prn->setOrientation( QPrinter::Portrait);
    prn->setPageMargins(20.0,20.0,20.0,20.0, prn->Millimeter);
    prn->setResolution(1200);
    prn->setColorMode(prn->GrayScale);

    w = prn->width();
    h = prn->height();

    QPainter p;
    p.begin(prn);
    y =  this->DrawBulbView(&p, w, h);

    p.setFont(TextFont);

    p.drawText(100,200, QString(" ")+QString(this->bc->naca_profile.foil_name.c_str()));
    x = 0;

    offset = 2000;
    p.setFont(TextFontBold);
    p.drawText(x, y, QString("Bulb data"));
    p.setFont(TextFont);
    p.drawText(x,y+250, QString("Lenght"));
    p.drawText(x,y+500, QString("Material Density"));
    p.drawText(x,y+750, QString("Projected weight"));
    p.drawText(x,y+1000, QString("Center"));
    p.drawText(x,y+1250, QString("Volume"));
    p.drawText(x,y+1500, QString("Wetted surface"));


    if (this->bc->units == UNIT_MM) {
        p.drawText(x+offset,y+250, QString(": ")+QString::number(this->bc->length)+QString(" cm"));
        p.drawText(x+offset,y+500, QString(": ")+QString::number(this->bc->material_density)+QString(" gr/cm³"));
        p.drawText(x+offset,y+750, QString(": ")+QString::number(this->bc->target_weight)+QString(" Kg"));
        p.drawText(x+offset,y+1000, QString(": ")+QString::number(this->bc->naca_profile.gcentre*wr)+QString(" cm"));
        p.drawText(x+offset,y+1250, QString(": ")+QString::number(this->bc->bulb_volume)+QString(" cm³"));
        p.drawText(x+offset,y+1500, QString(": ")+QString::number(this->bc->bulb_wet_surface)+QString(" cm²"));
    } else {
        p.drawText(x+offset,y+250, QString(": ")+DisplayValue(this->bc->length/2.54,this->bc->units)+QString(" in"));
        p.drawText(x+offset,y+500, QString(": ")+DisplayValue(this->bc->length/2.54,this->bc->units)+QString(" in"));
        p.drawText(x+offset,y+750, QString(": ")+DisplayValue(this->bc->target_weight*1000*0.035,this->bc->units)+QString(" oz"));
        p.drawText(x+offset,y+1000, QString(": ")+DisplayValue((this->bc->naca_profile.gcentre*wr)/2.54,this->bc->units)+QString(" in"));
        p.drawText(x+offset,y+1250, QString(": ")+DisplayValue(this->bc->bulb_volume*0.061,this->bc->units)+QString(" cu.in"));
        p.drawText(x+offset,y+1500, QString(": ")+DisplayValue(this->bc->bulb_wet_surface*0.155,this->bc->units)+QString(" sq.in"));
    }

    offset = 4000;
    p.drawText(x+offset, y+250, QString("Lenght/Height"));
    p.drawText(x+offset, y+500, QString("Width/Height"));
    p.drawText(x+offset, y+750, QString("Center"));
    p.drawText(x+offset, y+1000, QString("Center/Lenght"));
    p.drawText(x+offset, y+1250, QString("Max Diam"));
    p.drawText(x+offset, y+1500, QString("Frontal area"));

    offset = 6000;

    double mu, ml, mx;
    mx = this->bc->naca_profile.max_width * wr;
    mu = this->bc->naca_profile.max_height_u * wr;
    ml = this->bc->naca_profile.max_height_l * wr;

    if (ml < 0) {
        ml *= -1;
    }
//    my = mu+ml;
    double fal = (ml*mx*3.14)/2.0;
    double fau = (mu*mx*3.14)/2.0;
    double fa = fal+fau;

    if (this->bc->units == UNIT_MM) {
        p.drawText(x+offset,y+250, QString(": ")+QString::number(this->bc->naca_profile.HLRatio*100)+QString(" %"));
        p.drawText(x+offset,y+500, QString(": ")+QString::number(this->bc->naca_profile.WHRatio*100)+QString(" %"));
        p.drawText(x+offset,y+750, QString(": ")+QString::number(this->bc->naca_profile.gcentre*wr)+QString(" cm"));
        p.drawText(x+offset,y+1000, QString(": ")+QString::number(this->bc->naca_profile.gcentre*100)+QString(" %"));
        p.drawText(x+offset,y+1250, QString(": ")+QString::number(mx*2)+QString(" cm"));
        p.drawText(x+offset,y+1500, QString(": ")+QString::number(fa)+QString(" cm²"));
    } else {
        p.drawText(x+offset,y+250, QString(": ")+DisplayValue(this->bc->naca_profile.HLRatio*100,this->bc->units)+QString(" %"));
        p.drawText(x+offset,y+500, QString(": ")+DisplayValue(this->bc->naca_profile.WHRatio*100,this->bc->units)+QString(" %"));
        p.drawText(x+offset,y+750, QString(": ")+DisplayValue(this->bc->naca_profile.gcentre*wr/2.54,this->bc->units)+QString(" in"));
        p.drawText(x+offset,y+1000, QString(": ")+DisplayValue((this->bc->naca_profile.gcentre*100),this->bc->units)+QString(" %"));
        p.drawText(x+offset,y+1250, QString(": ")+DisplayValue(mx*2/2.54,this->bc->units)+QString(" in"));
        p.drawText(x+offset,y+1500, QString(": ")+DisplayValue(fa*0.155,this->bc->units)+QString(" sq.in"));
    }

    p.setFont(TextFontBold);
    p.drawText(x, y+2250, QString("Sections data"));
    p.setFont(TextFont);
    long begin = y+2500;
    long line = 0;
    p.drawText(x, begin, QString("Position"));
    p.drawText(x+1500, begin, QString("X Pos"));
    p.drawText(x+3000, begin, QString("Height Max"));
    p.drawText(x+4500, begin, QString("Height Min"));
    p.drawText(x+6000, begin, QString("Width"));
    begin += 250;
    for(int c=0; c<this->bc->num_sect+2; c++) {
        p.drawText(x, begin+(line*250),QString(this->bc->GetSectionValue(c,0)));
        p.drawText(x+1500, begin+(line*250),QString(this->bc->GetSectionValue(c,1)));
        p.drawText(x+3000, begin+(line*250),QString(this->bc->GetSectionValue(c,2)));
        p.drawText(x+4500, begin+(line*250),QString(this->bc->GetSectionValue(c,3)));
        p.drawText(x+6000, begin+(line*250),QString(this->bc->GetSectionValue(c,4)));

        if (begin+(line*250) > h-250) {
            prn->newPage();
            begin = 250;
            line = 0;
            p.drawText(x, begin, QString("Position"));
            p.drawText(x+1500, begin, QString("X Pos"));
            p.drawText(x+3000, begin, QString("Height Max"));
            p.drawText(x+4500, begin, QString("Height Min"));
            p.drawText(x+6000, begin, QString("Width"));
            begin += 250;
        } else {
            line++;
        }
    }

    p.end();

}

void PrintDraw::CheckBulbDimension(QPainter *painter, int ori) {

    float wr;
    float w;

    wr = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    wr *= 1000;
    w = 0;
    if (ori == QPrinter::Landscape) {
        w = painter->window().height();
    }
    if (ori == QPrinter::Portrait) {
        w = painter->window().width();
    }

    if (wr > w) {
        QMessageBox::warning(NULL, tr("BulbCalculator"),
                                tr("The draw is bigger than the printing area"),
                                QMessageBox::Ok );
    }


}


void PrintDraw::DrawBulbSection(QPainter *painter, int MainView, int ori) {

    switch(MainView) {
        case  P_SECTIONS_TOP:
            this->DrawBulbTop(painter, ori);
            break;
        case  P_SECTIONS_SIDE:
            this->DrawBulbProfile(painter, ori);
            break;
    }

}

void PrintDraw::DrawBulbLinesPlanTop(QPainter *painter, int ori) {

    double OriginY;
    double OriginX;
    long YAxis;
    double mult;
    float x, y, wr;
    long wri;
    int AxisStart, AxisEnd;
    QPen aPen;

    // TOP VIEW
    wr = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    wr *= 1000;
    wri = (int)wr;
    x = y = 0;
    if (ori == QPrinter::Landscape) {
        y = painter->window().width();
        x = painter->window().height();
    }
    if (ori == QPrinter::Portrait) {
        x = painter->window().width();
        y = painter->window().height();
    }


    OriginX = (x - wr) / 2.0;
    OriginY = (y*25)/100;

    AxisStart = OriginY - this->bc->naca_profile.max_width * wr - (y*2)/100;
    AxisEnd = OriginY + this->bc->naca_profile.max_width * wr + (y*2)/100;

    // Draw Axis

    aPen.setColor(Qt::black);
    aPen.setWidth(0);
    aPen.setStyle(Qt::SolidLine);
    painter->setPen(aPen);
    YAxis = wr * this->bc->naca_profile.gcentre;
    aPen.setColor(Qt::red);
    painter->setPen(aPen);
    painter->drawLine(QPoint(YAxis+OriginX,AxisStart), QPoint(YAxis+OriginX,AxisEnd));
    painter->drawLine(QPoint(0,OriginY), QPoint(x,OriginY));
    painter->drawEllipse(QPoint(YAxis+OriginX, OriginY), 500, 500);
    aPen.setColor(Qt::black);
    painter->setPen(aPen);

    // Draw the top
    //double t_point_x[wri];
    //double t_point_wyu[wri];
    //double t_point_wyl[wri];

    mult = this->bc->naca_profile.num_step/(double)wr;
/*
    for(int i=0;i < wri; i++) {
        t_point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        t_point_wyu[i] = pd.width * wr;
        t_point_wyl[i] = -pd.width * wr;
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(t_point_x[i-1]+OriginX,OriginY-t_point_wyu[i-1],
                             t_point_x[i]+OriginX,OriginY-t_point_wyu[i]);
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(t_point_x[i-1]+OriginX,OriginY-t_point_wyl[i-1],
                             t_point_x[i]+OriginX,OriginY-t_point_wyl[i]);
    }
*/
    for (unsigned cross_sect=0;;cross_sect++) {
        vector<double> x_data(wri, 0.0);
        vector<double> yu_data(wri, 0.0);
        vector<double> yl_data(wri, 0.0);
        float xy = (this->bc->slice_thickness*cross_sect)/(wr/1000);
        for (int i=0; i<wri; i++) {
            x_data[i] = ((double)i)/((double)wri-1) * wr;
            unsigned prof_data_ndx = (unsigned)floor(mult*x_data[i]+0.5);
            profile_data& pd1(this->bc->naca_profile[prof_data_ndx]);
            yu_data[i] = ellipse_get_x(pd1.width, pd1.height_u, xy) * wr;
            yl_data[i] = -yu_data[i];

        }

        for(int i=1; i < wri; i++) {
            if (yu_data[i] > 0) {
                painter->drawLine(x_data[i-1]+OriginX,OriginY-yu_data[i-1], x_data[i]+OriginX,OriginY-yu_data[i]);
                painter->drawLine(x_data[i-1]+OriginX,OriginY-yl_data[i-1], x_data[i]+OriginX,OriginY-yl_data[i]);
            }
        }
        if ( (xy > this->bc->naca_profile.max_height_l) && (xy > this->bc->naca_profile.max_height_u) ) {
            break;
        }
    }

}


void PrintDraw::DrawBulbLinesPlanSide(QPainter *painter, int ori) {


    double OriginY;
    double OriginX;
    long YAxis;
    double mult;
    float x, y, wr;
    long wri;
    int AxisStart, AxisEnd;
    QPen aPen;


    wr = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    wr *= 1000;
    wri = (int)wr;
    x = y = 0;
    if (ori == QPrinter::Landscape) {
        y = painter->window().width();
        x = painter->window().height();
    }
    if (ori == QPrinter::Portrait) {
        x = painter->window().width();
        y = painter->window().height();
    }

    OriginX = (x - wr) / 2.0;
    OriginY = (y*25)/100;

    AxisStart = OriginY - this->bc->naca_profile.max_width * wr - (y*2)/100;
    AxisEnd = OriginY + this->bc->naca_profile.max_width * wr + (y*2)/100;

    // Draw Axis

    aPen.setColor(Qt::black);
    aPen.setStyle(Qt::SolidLine);
    painter->setPen(aPen);
    YAxis = wr * this->bc->naca_profile.gcentre;
    aPen.setColor(Qt::red);
    painter->setPen(aPen);
    painter->drawLine(QPoint(YAxis+OriginX,AxisStart), QPoint(YAxis+OriginX,AxisEnd));
    painter->drawLine(QPoint(0,OriginY), QPoint(x,OriginY));
    painter->drawEllipse(QPoint(YAxis+OriginX, OriginY), 500, 500);
    aPen.setColor(Qt::black);
    painter->setPen(aPen);

    // Draw the profiles
    //double s_point_x[wri];
    //double s_point_hyu[wri];
    //double s_point_hyl[wri];

    mult = this->bc->naca_profile.num_step/(double)wr;
/*
    for(int i=0;i < wri; i++) {
        s_point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        s_point_hyu[i] = pd.height_u * wr;
        s_point_hyl[i] = pd.height_l * wr;
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(s_point_x[i-1]+OriginX,OriginY-s_point_hyu[i-1],
                             s_point_x[i]+OriginX,OriginY-s_point_hyu[i]);
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(s_point_x[i-1]+OriginX,OriginY-s_point_hyl[i-1],
                             s_point_x[i]+OriginX,OriginY-s_point_hyl[i]);
    }
*/
    for (unsigned cross_sect=0;;cross_sect++) {
        vector<double> x_data(wri, 0.0);
        vector<double> yu_data(wri, 0.0);
        vector<double> yl_data(wri, 0.0);
        float xy = (this->bc->slice_thickness*cross_sect)/(wr/1000);
        for (int i=0; i<wri; i++) {
            x_data[i] = ((double)i)/((double)wri-1) * wr;
            unsigned prof_data_ndx = (unsigned)floor(mult*x_data[i]+0.5);
            profile_data& pd1(this->bc->naca_profile[prof_data_ndx]);
            yu_data[i] = ellipse_get_x(pd1.height_u, pd1.width, xy) * wr;
            yl_data[i] = ellipse_get_x(pd1.height_l, pd1.width, xy) * wr;
        }
        int i;
        for(i=1; i < wri; i++) {
            if (yu_data[i] > 0) {
                painter->drawLine(x_data[i-1]+OriginX,OriginY-yu_data[i-1], x_data[i]+OriginX,OriginY-yu_data[i]);
                painter->drawLine(x_data[i-1]+OriginX,OriginY-yl_data[i-1], x_data[i]+OriginX,OriginY-yl_data[i]);
            }
        }

        if (xy > this->bc->naca_profile.max_width) {
            break;
        }

    }

}

void PrintDraw::DrawBulbProfile(QPainter *painter, int ori) {

    double OriginY;
    double OriginX;
    long YAxis;
    double mult;
    float x, y, wr;
    long wri;
    int AxisStart, AxisEnd, t, space, offset;

    wr = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    x = y = 0;
    wr *= 1000;
    wri = (int)wr;
    if (ori == QPrinter::Landscape) {
        y = painter->window().width();
        x = painter->window().height();
    }
    if (ori == QPrinter::Portrait) {
        x = painter->window().width();
        y = painter->window().height();
    }

    OriginX = (x - wr) / 2.0;
    OriginY = this->bc->naca_profile.max_height_u * wr + (y*5)/100;

    AxisStart = OriginY - this->bc->naca_profile.max_width * wr - (y*2)/100;
    AxisEnd = OriginY + this->bc->naca_profile.max_width * wr + (y*2)/100;

    // Draw Axis
    QPen aPen;
    aPen.setColor(Qt::black);
    aPen.setWidth(0);
    aPen.setStyle(Qt::SolidLine);
    painter->setPen(aPen);
    YAxis = wr * this->bc->naca_profile.gcentre;
    aPen.setColor(Qt::red);
    painter->setPen(aPen);
    painter->drawLine(QPoint(YAxis+OriginX,AxisStart), QPoint(YAxis+OriginX,AxisEnd));
    painter->drawLine(QPoint(0,OriginY), QPoint(x,OriginY));
    painter->drawEllipse(QPoint(YAxis+OriginX, OriginY), 500, 500);
    aPen.setColor(Qt::black);
    painter->setPen(aPen);

    // Draw the profiles
    double point_x[wri];
    double point_hyu[wri];
    double point_hyl[wri];

    mult = this->bc->naca_profile.num_step/(double)wr;

    for(int i=0;i < wri; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);    
        point_hyu[i] = pd.height_u * wr;
        point_hyl[i] = pd.height_l * wr;
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(point_x[i-1]+OriginX,OriginY-point_hyu[i-1],
                             point_x[i]+OriginX,OriginY-point_hyu[i]);
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(point_x[i-1]+OriginX,OriginY-point_hyl[i-1],
                             point_x[i]+OriginX,OriginY-point_hyl[i]);
    }

    // Draw the sections
    this->bc->sect_pos.clear();
    for(int i=1; i<this->bc->num_sect+1; i++) {
        double xc = (double)i/(double)(this->bc->num_sect+1);
        if(this->bc->sect_dist == DIST_EVEN){
            this->bc->sect_pos.push_back(xc);
        } else {
            this->bc->sect_pos.push_back(1.0-cos(M_PI*xc/2.0));
        }
    }
    vector<double> data_pos(this->bc->sect_pos);
    space = 0;

    for(vector<double>::iterator sp_it = data_pos.begin(); sp_it != data_pos.end(); sp_it++) {
        double xs = *sp_it;
        profile_data& pd(this->bc->naca_profile[(unsigned)floor(this->bc->naca_profile.profile::num_step*xs)]);
        double x_pos		= xs * this->bc->length*1000;
        double height_max	= pd.height_u * this->bc->length*1000;// wr;
        double height_min	= pd.height_l * this->bc->length*1000;// wr;
        double width		= pd.width * this->bc->length*1000;// wr;

        float diam = height_max - height_min;
        int ecc = height_max - (diam/2.0);
        offset = OriginY + diam*1.30 + (y*15)/100;

        if (space == 0) {
            t = offset - ecc;
            space = 1;
        } else {
            t = (offset - ecc) + diam*1.30;
            space = 0;
        }

        aPen.setColor(Qt::black);
        aPen.setStyle(Qt::SolidLine);
        painter->setPen(aPen);
        painter->drawEllipse(QPointF(x_pos+OriginX, t), width, diam/2);
        aPen.setColor(Qt::gray);
        aPen.setStyle(Qt::DashLine);
        painter->setPen(aPen);
        painter->drawLine(QPoint(x_pos+OriginX, OriginY-ecc-(y*2)/100), QPoint(x_pos+OriginX, t+width+(width*10)/100));
        painter->drawLine(QPoint(x_pos+OriginX-(x*2)/100, OriginY-ecc), QPoint(x_pos+OriginX+(x*2)/100, OriginY-ecc));
        painter->drawLine(QPoint(x_pos+OriginX-(width*110)/100, t), QPoint(x_pos+OriginX+(width*110)/100, t));
    }

}

void PrintDraw::DrawBulbTop(QPainter *painter, int ori) {

    double OriginY;
    double OriginX;
    long YAxis;
    double mult;
    float x, y, wr;
    long wri;
    int space, offset;
    int AxisStart, AxisEnd;
    int t;

    wr = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    x = y = 0;
    wr *= 1000;
    wri = (int)wr;
    if (ori == QPrinter::Landscape) {
        y = painter->window().width();
        x = painter->window().height();
    }
    if (ori == QPrinter::Portrait) {
        x = painter->window().width();
        y = painter->window().height();
    }

    OriginX = (x - wr) / 2.0;
    OriginY = this->bc->naca_profile.max_width * wr + (y*5)/100;

    AxisStart = OriginY - this->bc->naca_profile.max_width * wr - (y*2)/100;
    AxisEnd = OriginY + this->bc->naca_profile.max_width * wr + (y*2)/100;

    // Draw Axis
    QPen aPen;
    aPen.setColor(Qt::black);
    aPen.setWidth(0);
    aPen.setStyle(Qt::SolidLine);
    painter->setPen(aPen);
    YAxis = wr * this->bc->naca_profile.gcentre;
    aPen.setColor(Qt::red);
    painter->setPen(aPen);
    painter->drawLine(QPoint(YAxis+OriginX,AxisStart), QPoint(YAxis+OriginX,AxisEnd));
    painter->drawLine(QPoint(0,OriginY), QPoint(x,OriginY));
    painter->drawEllipse(QPoint(YAxis+OriginX, OriginY), 500, 500);
    aPen.setColor(Qt::black);
    painter->setPen(aPen);

    // Draw the profiles
    double point_x[wri];
    double point_wyu[wri];
    double point_wyl[wri];

    mult = this->bc->naca_profile.num_step/(double)wr;

    for(int i=0;i < wri; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        point_wyu[i] = pd.width * wr;
        point_wyl[i] = -pd.width * wr;
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(point_x[i-1]+OriginX,OriginY-point_wyu[i-1],
                             point_x[i]+OriginX,OriginY-point_wyu[i]);
    }

    for(int i=1; i < wri; i++) {
        painter->drawLine(point_x[i-1]+OriginX,OriginY-point_wyl[i-1],
                             point_x[i]+OriginX,OriginY-point_wyl[i]);
    }


    // Draw the sections
    this->bc->sect_pos.clear();
    for(int i=1; i<this->bc->num_sect+1; i++) {
        double xc = (double)i/(double)(this->bc->num_sect+1);
        if(this->bc->sect_dist == DIST_EVEN){
            this->bc->sect_pos.push_back(xc);
        } else {
            this->bc->sect_pos.push_back(1.0-cos(M_PI*xc/2.0));
        }
    }
    vector<double> data_pos(this->bc->sect_pos);
    space = 0;

    for(vector<double>::iterator sp_it = data_pos.begin(); sp_it != data_pos.end(); sp_it++) {
        double xs = *sp_it;
        profile_data& pd(this->bc->naca_profile[(unsigned)floor(this->bc->naca_profile.profile::num_step*xs)]);
        double x_pos		= xs * this->bc->length*1000;
        double height_max	= pd.height_u * this->bc->length*1000;// wr;
        double height_min	= pd.height_l * this->bc->length*1000;// wr;
        double width		= pd.width * this->bc->length*1000;// wr;

        float diam = height_max - height_min;
        int ecc = height_max - (diam/2.0);

        offset = OriginY + width*1.30 + (y*15)/100;

        if (space == 0) {
            t = offset - ecc;
            space = 1;
        } else {
            t = (offset - ecc) + width*1.30;
            space = 0;
        }
        aPen.setColor(Qt::black);
        aPen.setStyle(Qt::SolidLine);
        painter->setPen(aPen);

        painter->drawEllipse(QPointF(x_pos+OriginX, t), diam/2, width);
        aPen.setColor(Qt::gray);
        aPen.setStyle(Qt::DashLine);
        painter->setPen(aPen);
        painter->drawLine(QPoint(x_pos+OriginX, OriginY), QPoint(x_pos+OriginX, t+width+(width*10)/100));
        painter->drawLine(QPoint(x_pos+OriginX-(height_min*110)/100, t), QPoint(x_pos+OriginX+(height_min*110)/100, t));
    }

}


long PrintDraw::DrawBulbView(QPainter *p, int w, int h) {

    long wr;
    double OriginY, OriginX, SectOriginX;
    double mult;
    QPen pen;

    wr = (w-100)/2;

    OriginY = this->bc->naca_profile.max_width*wr + 500;
    OriginX = 0;

    double point_x[wr];
    double point_wyu[wr];
    double point_wyl[wr];
    double point_hyu[wr];
    double point_hyl[wr];

    mult = this->bc->naca_profile.num_step/(double)wr;

    for(int i=0;i < wr; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        point_wyu[i] = pd.width * wr;
        point_wyl[i] = -pd.width * wr;
        point_hyu[i] = pd.height_u * wr;
        point_hyl[i] = pd.height_l * wr;
    }

    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DashDotLine);
    p->setPen(pen);
    p->drawLine(0, OriginY, w, OriginY);

    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    p->setPen(pen);

    for(int i=1; i < wr; i++) {
        p->drawLine(OriginX + point_x[i-1],OriginY-point_wyu[i-1],
                             OriginX + point_x[i],OriginY-point_wyu[i]);
    }
    for(int i=1; i < wr; i++) {
        p->drawLine(OriginX + point_x[i-1],OriginY-point_wyl[i-1],
                             OriginX + point_x[i],OriginY-point_wyl[i]);
    }

    // Draw the sections
    SectOriginX = w/2 + this->bc->naca_profile.max_height_l*wr + w/4;
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DashDotLine);
    p->setPen(pen);

    p->drawLine(SectOriginX, 0, SectOriginX, OriginY*2);

    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    p->setPen(pen);

    // Draw the sections
    this->bc->sect_pos.clear();
    for(int i=1; i<this->bc->num_sect+1; i++) {
        double xc = (double)i/(double)(this->bc->num_sect+1);
        if(this->bc->sect_dist == DIST_EVEN){
            this->bc->sect_pos.push_back(xc);
        } else {
            this->bc->sect_pos.push_back(1.0-cos(M_PI*xc/2.0));
        }
    }
    vector<double> data_pos(this->bc->sect_pos);
    data_pos.insert(data_pos.begin(), 0.0);
    data_pos.push_back(1.0);
    for(vector<double>::iterator sp_it = data_pos.begin(); sp_it != data_pos.end(); sp_it++) {
        double xs = *sp_it;
        profile_data& pd(this->bc->naca_profile[(unsigned)floor(this->bc->naca_profile.profile::num_step*xs)]);
        double x_pos		= xs * wr;
        double height_max	= pd.height_u *wr;
        double height_min	= pd.height_l *wr;
        double width		= pd.width * wr;

        float diam = height_max - height_min;
        int ecc = height_max - (diam/2.0);

        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
        p->setPen(pen);
        p->drawEllipse(QPointF(SectOriginX+ecc, OriginY), diam/2, width);

        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);

        p->drawLine(OriginX+x_pos, OriginY-this->bc->naca_profile.max_width*wr-100, OriginX+x_pos, OriginY+this->bc->naca_profile.max_width*wr+100);
    }


    OriginY += (this->bc->naca_profile.max_height_l*wr) + (this->bc->naca_profile.max_width*wr) + 1500;

    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DashDotLine);
    p->setPen(pen);

    p->drawLine(0, OriginY, w/2+(w*10/100), OriginY);

    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    p->setPen(pen);

    for(int i=1; i < wr; i++) {
        p->drawLine(OriginX + point_x[i-1],OriginY-point_hyu[i-1],
                             OriginX + point_x[i],OriginY-point_hyu[i]);
    }
    for(int i=1; i < wr; i++) {
        p->drawLine(OriginX + point_x[i-1],OriginY-point_hyl[i-1],
                             OriginX + point_x[i],OriginY-point_hyl[i]);
    }



    return OriginY+1500;
}

