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

    long h,w, lh;
    long y1, y2;


    w = (int)this->scene()->width();
    h = (int)this->scene()->height();

    y1 = (h/4) * 2;
    y2 = (h/2) + y1*2;
    lh = w*0.75;

    this->scene()->clear();
    this->scene()->addLine(10,h/2.0,w-10,h/2.0,QPen(Qt::black));
    this->scene()->addLine(lh,h+10,lh,h/h-10,QPen(Qt::black));
    //this->scene()->addLine(OriginY,10,OriginY,hr-10,QPen(Qt::red));

    ViewArea::DrawAxis(lh,y1);
    //ViewArea::DrawAxis(lh,y2);
    //ViewArea::DrawAxis(w/2,h/2);
    ViewArea::DrawBulb();

}


void ViewArea::DrawAxis(long wr, long hr) {

    QString prof;
    float OriginY;


//    wr = (int)this->scene()->width();
//    hr = (int)this->scene()->height();
    qDebug() << hr;
    OriginY = wr * this->bc->naca_profile.gcentre ;
    this->scene()->addLine(10,hr/2.0,wr-10,hr/2.0,QPen(Qt::red));
    this->scene()->addLine(OriginY,10,OriginY,hr-10,QPen(Qt::red));
    prof.append((char *)"Profile: ");
    prof.append((char *)this->bc->naca_profile.foil_name.c_str());
    this->scene()->addText(prof);

    this->scene()->addEllipse((OriginY-10),(hr/2.0)-10, 20,20, QPen(Qt::red));


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
