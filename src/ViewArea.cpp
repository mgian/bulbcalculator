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
    this->DrawAxis();
    this->DrawBulb();

}

void ViewArea::UpdateView() {

    ViewArea::DrawAxis();
    ViewArea::DrawBulb();

}


void ViewArea::DrawAxis(void) {

    long wr, hr;
    QString prof;
    float OriginY;


    wr = (int)this->scene()->width();
    hr = (int)this->scene()->height();
    OriginY = wr * this->bc->naca_profile.gcentre ;
    this->scene()->clear();
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
    double point_hyu[w];
    double point_hyl[w];

    mult = this->bc->naca_profile.num_step/(double)w;

    for(int i=0;i < w; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        point_wyu[i] = pd.width * w;
        point_wyl[i] = -pd.width * w;
        point_hyu[i] = pd.height_u * w;
        point_hyl[i] = pd.height_l * w;
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

void ViewArea::DrawBulbSide() {

    long w,h;
    double OriginY;
    double mult;

    w = (int)this->scene()->width() - 30;
    h = (int)this->scene()->height();
    OriginY = h/2.0;


    double point_x[w];
    double point_wyu[w];
    double point_wyl[w];
    double point_hyu[w];
    double point_hyl[w];

    mult = this->bc->naca_profile.num_step/(double)w;

    for(int i=0;i < w; i++) {
        point_x[i] = i;
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        point_wyu[i] = pd.width * w;
        point_wyl[i] = -pd.width * w;
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
        }
}
