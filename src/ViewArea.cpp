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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QtXml>

#include "../include/BulbCalculator.h"



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

    TopAxis_y = (h/4) * 2;
    SideAxis_y = TopAxis_y + h/2;
    hy = h/2;
    hl = w*0.75;

    this->scene()->clear();
    this->scene()->addLine(10,h/2.0,w-10,h/2.0,QPen(Qt::black));
    this->scene()->addLine(hl,h+10,hl,h/h-10,QPen(Qt::black));

    ViewArea::DrawAxisTopSide(hl,hy,TopAxis_y);
    ViewArea::DrawAxisTopSide(hl,hy,SideAxis_y);
    ViewArea::DrawAxisFront(hl+((w-hl)/2),hy,TopAxis_y, w-hl);
    ViewArea::DrawText(w,h);
    ViewArea::DrawBulb();

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
    prof.append((char *)"Profile: ");
    prof.append((char *)this->bc->naca_profile.foil_name.c_str());
    prof.append((char *)" - Top view");
    path.addText(15, 15, font,  prof);

    prof.clear();
    prof.append((char *)"Profile: ");
    prof.append((char *)this->bc->naca_profile.foil_name.c_str());
    prof.append((char *)" - Side view");
    path.addText(15, h/2+15, font,  prof);

    prof.clear();
    prof.append((char *)"Profile: ");
    prof.append((char *)this->bc->naca_profile.foil_name.c_str());
    prof.append((char *)" - Front view");
    path.addText(w*0.75+15, 15, font,  prof);

    this->scene()->addPath(path, QPen(QBrush(Qt::black), 0.4), QBrush(Qt::black));

}

void ViewArea::DrawAxisFront(long wr, long hr, long origin, long wl) {

    QString prof;
    float OriginY;
    long y;

    y = origin-hr/2.0;


    OriginY = wr * this->bc->naca_profile.gcentre ;
    this->scene()->addLine(wr-wl/2+10,y,wr+wl/2-10,y,QPen(Qt::red));
    this->scene()->addLine(wr,y-(hr/2-10),wr,y+(hr/2-10),QPen(Qt::red));

    this->scene()->addEllipse(wr-10,y-10, 20,20, QPen(Qt::red));

}

void ViewArea::DrawAxisTopSide(long wr, long hr, long origin) {

    QString prof;
    float OriginY;
    long y;

    y = origin-hr/2.0;

    OriginY = wr * this->bc->naca_profile.gcentre ;
    this->scene()->addLine(10,y,wr-10,y,QPen(Qt::red));
    this->scene()->addLine(OriginY,y-(hr/2-10),OriginY,y+(hr/2-10),QPen(Qt::red));
    this->scene()->addEllipse((OriginY-10),y-10, 20,20, QPen(Qt::red));


}

void ViewArea::DrawBulbTop() {

    long w,h;
    double OriginY;
    double mult;

    w = (int)this->scene()->width() - 30;
    h = (int)this->scene()->height();
    OriginY = h/2.0;

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

void ViewArea::DrawBulb2d() {

    long h,w, mh, mw, lh;


    w = (int)this->scene()->width()-30;
    h = (int)this->scene()->height();



}

void ViewArea::DrawBulbSide() {

    long w,h;
    double OriginY;
    double mult;

    w = (int)this->scene()->width() - 30;
    h = (int)this->scene()->height();
    OriginY = h/2.0;


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

void ViewArea::DrawBulb() {

    switch(this->view) {
        case TOP_VIEW:
            ViewArea::DrawBulbTop();
            break;
        case SIDE_VIEW:
            ViewArea::DrawBulbSide();
            break;
        case PLAN_VIEW:
            ViewArea::DrawBulb2d();
            break;
        }

}
