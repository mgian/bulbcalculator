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

#include "../include/BulbCalculator.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QtXml>





ViewArea::ViewArea(QGraphicsScene *scene, BulbCalculator *bcp,  int type, QWidget *parent): QGraphicsView(scene, parent) {

    this->bc = bcp;
    this->view = type;

}

void ViewArea::resizeEvent( QResizeEvent * event ) {

    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    }
    QGraphicsView::resizeEvent(event);
    this->UpdateView();

}

void ViewArea::UpdateView() {

    long h,w, hl, hy;
    long TopAxis_y, SideAxis_y ;

    w = (int)this->scene()->width();
    h = (int)this->scene()->height();

    SideAxis_y = (h/4) * 2;
    TopAxis_y = SideAxis_y + h/2;
    hy = h/2;
    hl = w*0.75;

    this->scene()->clear();
    this->scene()->addLine(10,h/2.0,w-10,h/2.0,QPen(Qt::black));
    this->scene()->addLine(hl,h+10,hl,h/h-10,QPen(Qt::black));

    ViewArea::DrawAxisTopSide(hl,hy,TopAxis_y);
    ViewArea::DrawAxisTopSide(hl,hy,SideAxis_y);
    ViewArea::DrawAxisFront(hl+((w-hl)/2),hy,SideAxis_y, w-hl);
    ViewArea::DrawText(w,h);
    ViewArea::DrawBulbTop(hl, TopAxis_y);
    ViewArea::DrawBulbSide(hl, SideAxis_y);
    ViewArea::DrawBulbFront(hl+((w-hl)/2), SideAxis_y);

}

void ViewArea::DrawText(long w, long h) {

    QString prof;
    QPainterPath path;
    QFont font("Monospace");
    font.setFamily(font.defaultFamily());
    font.setBold(false);
    font.setStyleHint(QFont::TypeWriter);
    font.setStyleStrategy(QFont::NoAntialias);
    font.setPixelSize(14);

    prof.clear();
    prof.append(tr("Profile: "));
    prof.append(this->bc->naca_profile.foil_name.c_str());
    prof.append(tr(" - Side view"));
    path.addText(15, 15, font,  prof);

    prof.clear();
    prof.append(tr("Profile: "));
    prof.append(this->bc->naca_profile.foil_name.c_str());
    prof.append(tr(" - Top view"));
    path.addText(15, h/2+15, font,  prof);

    prof.clear();
    prof.append(tr("Profile: "));
    prof.append(this->bc->naca_profile.foil_name.c_str());
    prof.append(tr(" - Front view"));
    path.addText(w*0.75+15, 15, font,  prof);

    this->scene()->addPath(path, QPen(QBrush(Qt::black), 0.4), QBrush(Qt::black));

}

void ViewArea::DrawAxisFront(long wr, long hr, long origin, long wl) {

    long y;

    y = origin-hr/2.0;

    this->scene()->addLine(wr-wl/2+10,y,wr+wl/2-10,y,QPen(Qt::red));
    this->scene()->addLine(wr,y-(hr/2-10),wr,y+(hr/2-10),QPen(Qt::red));
    this->scene()->addEllipse(wr-10,y-10, 20,20, QPen(Qt::red));

}

void ViewArea::DrawAxisTopSide(long wr, long hr, long origin) {

    float OriginY;
    long y;

    y = origin-hr/2.0;

    OriginY = wr * this->bc->naca_profile.gcentre ;
    this->scene()->addLine(10,y,wr-10,y,QPen(Qt::red));
    this->scene()->addLine(OriginY,y-(hr/2-10),OriginY,y+(hr/2-10),QPen(Qt::red));
    this->scene()->addEllipse((OriginY-10),y-10, 20,20, QPen(Qt::red));


}

void ViewArea::DrawBulbTop(long hl, long Origin_Top) {

    long w;
    double OriginY;
    double mult;
    w = hl - 30;


    OriginY = Origin_Top/2 + (Origin_Top/4);

    double point_x[w];
    double point_wyu[w];
    double point_wyl[w];

    mult = this->bc->naca_profile.num_step/(double)w;

    for(int i=0;i < w; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        point_wyu[i] = pd.width * w;
        point_wyl[i] = -pd.width * w;
    }

    for(int i=1; i < w; i++) {
        this->scene()->addLine(15 + point_x[i-1],OriginY-point_wyu[i-1],
                             15 + point_x[i],OriginY-point_wyu[i],QPen(Qt::black));
    }
    for(int i=1; i < w; i++) {
        this->scene()->addLine(15 + point_x[i-1],OriginY-point_wyl[i-1],
                             15 + point_x[i],OriginY-point_wyl[i],QPen(Qt::black));
    }

}

void ViewArea::DrawBulbSide(long hl, long Origin_Side) {

    long w;
    double OriginY;
    double mult;

    w = hl - 30;
    OriginY = Origin_Side / 2.0;
    double point_x[w];
    double point_hyu[w];
    double point_hyl[w];

    mult = this->bc->naca_profile.num_step/(double)w;

    for(int i=0;i < w; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        point_hyu[i] = pd.height_u * w;
        point_hyl[i] = pd.height_l * w;
    }

    for(int i=1; i < w; i++) {
        this->scene()->addLine(15 + point_x[i-1],OriginY-point_hyu[i-1],
                             15 + point_x[i],OriginY-point_hyu[i],QPen(Qt::black));
    }
    for(int i=1; i < w; i++) {
        this->scene()->addLine(15 + point_x[i-1],OriginY-point_hyl[i-1],
                             15 + point_x[i],OriginY-point_hyl[i],QPen(Qt::black));
    }

}

void ViewArea::DrawBulbFront(long Origin_X, long Origin_Top) {

    long w;
    double OriginY;
    double OriginX;
    double xa, yu, yl;

    w = (int)this->scene()->width()*0.75 - 30;

    OriginY = Origin_Top / 2.0;
    OriginX = Origin_X ;

    xa = this->bc->naca_profile.max_width * w;
    yu = this->bc->naca_profile.max_height_u * w;
    yl = this->bc->naca_profile.max_height_l * w;

    QGraphicsEllipseItem* itemU = new QGraphicsEllipseItem(OriginX-xa,OriginY-yu, xa*2, yu*2);
    itemU->setStartAngle(0);
    itemU->setSpanAngle(180*16);
    this->scene()->addItem(itemU);

    QGraphicsEllipseItem* itemL = new QGraphicsEllipseItem(OriginX-xa,OriginY-yl, xa*2, yl*2);
    itemL->setStartAngle(180*16);
    itemL->setSpanAngle(180*16);
    this->scene()->addItem(itemL);

}
