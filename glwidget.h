#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#include <GL\glu.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cfloat>

using namespace std;

class Color {

private:

    float red, green, blue;

public:

    Color(float red, float green, float blue):red(red),green(green),blue(blue) {}

    float getRed() { return red; }
    float getGreen() { return green; }
    float getBlue() { return blue; }

};

class Surface {

private:

    vector<float> coordinates;

public:

    Surface() {}

    void push_back(float value) { coordinates.push_back(value); }
    unsigned int getnCoordinates() { return coordinates.size(); }
    float getCoordinate(unsigned int i) { return coordinates[i]; }

    float getMinX() { float minX=numeric_limits<float>::max(); for (size_t index=0; index<coordinates.size()/3; ++index) minX=min(minX,coordinates[index*3]); return minX; }
    float getMaxX() { float maxX=numeric_limits<float>::min(); for (size_t index=0; index<coordinates.size()/3; ++index) maxX=max(maxX,coordinates[index*3]); return maxX; }
    float getMinY() { float minY=numeric_limits<float>::max(); for (size_t index=0; index<coordinates.size()/3; ++index) minY=min(minY,coordinates[index*3+1]); return minY; }
    float getMaxY() { float maxY=numeric_limits<float>::min(); for (size_t index=0; index<coordinates.size()/3; ++index) maxY=max(maxY,coordinates[index*3+1]); return maxY; }
    float getMinZ() { float minZ=numeric_limits<float>::max(); for (size_t index=0; index<coordinates.size()/3; ++index) minZ=min(minZ,coordinates[index*3+2]); return minZ; }
    float getMaxZ() { float maxZ=numeric_limits<float>::min(); for (size_t index=0; index<coordinates.size()/3; ++index) maxZ=max(maxZ,coordinates[index*3+2]); return maxZ; }

    float getRangeX() { float minX=numeric_limits<float>::max(), maxX=numeric_limits<float>::min();
                        for (size_t index=0; index<coordinates.size()/3; ++index) { minX=min(minX,coordinates[index*3]); maxX=max(maxX,coordinates[index*3]); }
                        return (maxX-minX); }
    float getRangeY() { float minY=numeric_limits<float>::max(), maxY=numeric_limits<float>::min();
                        for (size_t index=0; index<coordinates.size()/3; ++index) { minY=min(minY,coordinates[index*3+1]); maxY=max(maxY,coordinates[index*3+1]); }
                        return (maxY-minY); }
    float getRangeZ() { float minZ=numeric_limits<float>::max(), maxZ=numeric_limits<float>::min();
                        for (size_t index=0; index<coordinates.size()/3; ++index) { minZ=min(minZ,coordinates[index*3+2]); maxZ=max(maxZ,coordinates[index*3+2]); }
                        return (maxZ-minZ); }

};


template <typename T> inline string toString(const T &s) {

    std::stringstream ss;
    ss << s;

    return ss.str();

}

class GLWidget : public QGLWidget
{

    Q_OBJECT

public:

    GLWidget(QWidget *parent);

    // elements written in addition to the standard QGLWidget class
    GLuint makeObject(QString radFile);
    void deleteObject();
    void displayImage(QString filename);

    // sets the simulation to rerun from scratch
    void setModel(int value);

    // methods to get the view and the bounding box
    int getWidth() { return this->width(); }
    int getHeight() { return this->height(); }
    vector<double> getViewPoint() { vector<double> vp_v; for (unsigned int i=0; i<3; ++i) vp_v.push_back(vp[i]); return vp_v; }
    vector<double> getViewDirection() { vector<double> vd_v; for (unsigned int i=0; i<3; ++i) vd_v.push_back(vd[i]); return vd_v; }
    vector<double> getViewUp() { vector<double> vu_v; for (unsigned int i=0; i<3; ++i) vu_v.push_back(vu[i]); return vu_v; }
    vector<double> getBoundingBox() { vector<double> bb; bb.push_back(xMin); bb.push_back(xMax); bb.push_back(yMin); bb.push_back(yMax);
                                      bb.push_back(zMin); bb.push_back(zMax); return bb; }

    void setViewPoint(double vpx, double vpy, double vpz) { vp[0]=vpx; vp[1]=vpy; vp[2]=vpz; image=false; updateGL(); }
    void setViewDirection(float phi, float theta) { this->phi = phi; this->theta = theta; image=false; updateGL(); }
    void setScale(float scale) { this->scale = scale; image=false; updateGL(); }

signals:

    void view(float vpx, float vpy, float vpz, float phi, float theta);

protected:

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    // different events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QWidget *parent;

    QPoint lastPos;

    int dx, dy;
    float phi, theta;
    int wheelRot;
    float scale;

    // textures and images
    GLuint textureImage;
    QPixmap pixmap; // image in illuminance

    // stores the scene
    GLuint object;

    // about the scene, min, max
    double xMin, xMax, yMin, yMax, zMin, zMax;

    // display an image
    bool image;

    // about the view
    GLdouble vp[3],vd[3],vu[3];

    // different materials in the file
    map<string,Color> mapMaterials;
    vector<pair<string,Surface> > mapSurfaces;

    // integer that represents the model itself
    int model;

};

template <typename T> inline T to(const string &s) {

    T value;
    std::stringstream ss(s);
    ss >> value;

    return value;

}

template <class T> void load(const string filename, vector<T> &vector) {

    fstream input (filename.c_str(), ios::in | ios::binary);
    if (!input.is_open()) throw(string("Error loading file: " + filename));

    string buffer;
    do {
        input >> buffer;
        if (input.eof()) break;
        vector.push_back(to<T>(buffer));
    } while (!buffer.empty());

    input.close();

    return;

}

#endif // GLWIDGET_H
