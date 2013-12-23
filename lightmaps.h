#ifndef LIGHTMAPS_H
#define LIGHTMAPS_H

/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#if QT_VERSION < 0x0040500
#error You need Qt 4.5 or newer
#endif

#if defined (Q_OS_SYMBIAN)
#include "sym_iap_util.h"
#endif

#include <cmath>
#include <iostream>

class SlippyMap: public QObject
{
    Q_OBJECT

public:
    int width;
    int height;
    int zoom;
    qreal latitude;
    qreal longitude;

    SlippyMap(QObject *parent = 0);

    void invalidate();
    void render(QPainter *p, const QRect &rect);
    void pan(const QPoint &delta);

private slots:

    void handleNetworkData(QNetworkReply *reply);
    void download();

signals:
    void updated(const QRect &rect);
    void positionChanged(qreal latitude, qreal longitude);

protected:
    QRect tileRect(const QPoint &tp);

private:
    QPoint m_offset;
    QRect m_tilesRect;
    QPixmap m_emptyTile;
    QHash<QPoint, QPixmap> m_tilePixmaps;
    QNetworkAccessManager m_manager;
    QUrl m_url;
};

class LightMaps: public QWidget
{
    Q_OBJECT

public:
    LightMaps(qreal latitude, qreal longitude, QWidget *parent = 0)
            : QWidget(parent)
            , pressed(false)
            , snapped(false)
            , invert(false) {
        m_normalMap = new SlippyMap(this);
        m_normalMap->latitude = latitude;
        m_normalMap->longitude = longitude;
        connect(m_normalMap, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
    }

    void setCenter(qreal lat, qreal lng) {
        m_normalMap->latitude = lat;
        m_normalMap->longitude = lng;
        m_normalMap->invalidate();
    }

public slots:
    void toggleNightMode() {
        invert = !invert;
        update();
    }

private slots:
    void updateMap(const QRect &r) {
        update(r);
    }

protected:

    void resizeEvent(QResizeEvent *) {
        m_normalMap->width = width();
        m_normalMap->height = height();
        m_normalMap->invalidate();
    }

    void paintEvent(QPaintEvent *event) {
        QPainter p;
        p.begin(this);
        m_normalMap->render(&p, event->rect());
        p.setPen(Qt::black);
#if defined(Q_OS_SYMBIAN)
        QFont font = p.font();
        font.setPixelSize(13);
        p.setFont(font);
#endif
        p.drawText(rect(),  Qt::AlignBottom | Qt::TextWordWrap,
                   QString("OpenStreetMap.org - Latitude: ") + QString::number(m_normalMap->latitude) + QString(" - Longitude: ") + QString::number(m_normalMap->longitude));
        p.end();

        if (invert) {
            QPainter p(this);
            p.setCompositionMode(QPainter::CompositionMode_Difference);
            p.fillRect(event->rect(), Qt::white);
            p.end();
        }
    }

    void mousePressEvent(QMouseEvent *event) {
        if (event->buttons() != Qt::LeftButton)
            return;
        pressed = snapped = true;
        pressPos = dragPos = event->pos();
    }

    void mouseMoveEvent(QMouseEvent *event) {
        if (!event->buttons())
            return;
        if (!pressed || !snapped) {
            QPoint delta = event->pos() - pressPos;
            pressPos = event->pos();
            m_normalMap->pan(delta);
            return;
        } else {
            const int threshold = 10;
            QPoint delta = event->pos() - pressPos;
            if (snapped) {
                snapped &= delta.x() < threshold;
                snapped &= delta.y() < threshold;
                snapped &= delta.x() > -threshold;
                snapped &= delta.y() > -threshold;
            }
        }
    }

    void mouseReleaseEvent(QMouseEvent *) {
        std::cout << "latitude: " << m_normalMap->latitude << "\tlongitude: " << m_normalMap->longitude << "\tzoom: " << m_normalMap->zoom << std::endl;
        m_normalMap->invalidate();
    }

    void wheelEvent(QWheelEvent *event)
    {

        if ( event->delta() > 0 ) { m_normalMap->zoom = qMin(15,m_normalMap->zoom+1); }
        else { m_normalMap->zoom = qMax(3,m_normalMap->zoom-1); }
        m_normalMap->invalidate();

    }

    void keyPressEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_Left)
            m_normalMap->pan(QPoint(20, 0));
        if (event->key() == Qt::Key_Right)
            m_normalMap->pan(QPoint(-20, 0));
        if (event->key() == Qt::Key_Up)
            m_normalMap->pan(QPoint(0, 20));
        if (event->key() == Qt::Key_Down)
            m_normalMap->pan(QPoint(0, -20));
        if (event->key() == Qt::Key_Plus)
            { m_normalMap->zoom = qMin(15,m_normalMap->zoom+1); m_normalMap->invalidate(); }
        if (event->key() == Qt::Key_Minus)
            { m_normalMap->zoom = qMax(3,m_normalMap->zoom-1); m_normalMap->invalidate(); }
    }

public:

    SlippyMap *m_normalMap;

private:

    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;
    QPixmap zoomPixmap;
    QPixmap maskPixmap;
    bool invert;
};

#endif

//#include "moc_lightmaps.cpp"
//
//int main(int argc, char **argv)
//{
//#if defined(Q_WS_X11)
//    QApplication::setGraphicsSystem("raster");
//#endif
//
//    QApplication app(argc, argv);
//    app.setApplicationName("LightMaps");
//
//    MapZoom w;
//    w.setWindowTitle("OpenStreetMap");
//#if defined(Q_OS_SYMBIAN)
//    w.showMaximized();
//#else
//    w.resize(600, 450);
//    w.show();
//#endif
//
//    return app.exec();
//}
