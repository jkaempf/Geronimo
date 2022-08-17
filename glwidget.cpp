#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLTexture>

#include "glwidget.h"
//wheelRot(0),textureImage(0),
GLWidget::GLWidget(QWidget *parent):QOpenGLWidget(parent),parent(parent),dx(0),dy(0),phi(270.f),theta(0.f),wheelRot(0),image(false) {

    // the view point
    vp[0] = 1.725;
    vp[1] = 4.8;
    vp[2] = 1.8;

    // the view direction
    vd[0] = cos(theta*M_PI/180.)*cos(phi*M_PI/180.);
    vd[1] = cos(theta*M_PI/180.)*sin(phi*M_PI/180.);
    vd[2] = sin(theta*M_PI/180.);

    std::cout << "vd: (" << vd[0] << ", " << vd[1] << ", " << vd[2] << ")" << std::endl;

    // the view up
    vu[0] = 0.;
    vu[1] = 0.;
    vu[2] = 1.;

    // start with model 0
    model = 0;

    // initialize the values for the scene size
    xMax=0.0;
    xMin=0.0;
    yMax=0.0;
    yMin=0.0;
    zMax=0.0;
    zMin=0.0;

    // scale
    scale = 1.f;

}

GLWidget::~GLWidget() {

    if (texture) delete texture;

}

void GLWidget::initializeGL()
{
     // initialise the OpenGL context
     initializeOpenGLFunctions();

     glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
     glShadeModel(GL_FLAT);

     glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LESS);

     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

     glFrontFace(GL_CCW);
     glCullFace(GL_BACK);
     //glEnable(GL_CULL_FACE);
     glDrawBuffer(GL_BACK);

     // loads the scene
     object = makeObject(QString::fromStdString("base_" + toString(model) + ".rad"));

     //std::cout << "initializedGL" << std::endl;

}

void GLWidget::resizeGL(int width, int height)
{
     glViewport(0, 0, width, height);

     //std::cout << "resizedGL" << std::endl;

}

void GLWidget::paintGL()
{
    // clears the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!image) {

        double d, xmin, xmax, ymin, ymax, zmin, zmax;

        double viewHoriz = 45.;
        double viewVert  = 45.;
        double viewHorizOff = 0.;
        double viewVertOff = 0.;

        zmin = 0.1;
        zmax = 1000.;
        xmax = zmin * tan(M_PI/180./2. * viewHoriz);
        xmin = -xmax;
        d = viewHorizOff * (xmax - xmin);
        xmin += d; xmax += d;
        ymax = zmin * tan(M_PI/180./2. * viewVert);
        ymin = -ymax;
        d = viewVertOff * (ymax - ymin);
        ymin += d; ymax += d;

        // defines the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glScalef(1.0, static_cast<float>(this->width() / this->height()), 1.0); // scaling to counterbalance the effect of the screen
        glFrustum(xmin, xmax, ymin, ymax, zmin, zmax);

        // defines the view to the model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // updates the camera rotation
        phi += (180./M_PI)*atan( dx * tan(M_PI/180./2. * viewHoriz) / ((float) this->width() / 2.) );
        theta += (180./M_PI)*atan( dy * tan(M_PI/180./2. * viewHoriz) / ((float) this->width() / 2.) );

        // the view direction
        vd[0] = cos(theta*M_PI/180.)*cos(phi*M_PI/180.);
        vd[1] = cos(theta*M_PI/180.)*sin(phi*M_PI/180.);
        vd[2] = sin(theta*M_PI/180.);

        // the view point
        float distanceIncrement = 0.25f;
        vp[0] += vd[0]*wheelRot/120.*distanceIncrement;
        vp[1] += vd[1]*wheelRot/120.*distanceIncrement;
        vp[2] += vd[2]*wheelRot/120.*distanceIncrement;

        // the view up
        vu[0] = 0.;
        vu[1] = 0.;
        vu[2] = 1.;

        // goes to the correct view
        gluLookAt(vp[0], vp[1], vp[2],
                  vp[0]+ vd[0], vp[1]+ vd[1], vp[2]+ vd[2],
                  vu[0], vu[1], vu[2]);

        // loads the list of objects
        glScalef(scale,scale,scale);
        glCallList(object);

        // sends the view to visuCFS
        emit(view(vp[0],vp[1],vp[2],phi,theta));

    }
    else {

        // defines the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, imageTexture.width(), 0, imageTexture.height());

        // defines the view to the model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // bind the texture
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.f,1.f,1.f);
        texture->bind();

        // creates a polygon of dimension 1 and 1 with the texture applied
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.f, 0.f,  0.f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f( imageTexture.width(), 0.f,  0.f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f( imageTexture.width(), imageTexture.height(), 0.f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.f, imageTexture.height(), 0.f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

    }

    //std::cout << "paintedGL" << std::endl;
}

GLuint GLWidget::makeObject(QString radFile)
{

    // chargement des données a afficher
    if (!radFile.isEmpty()) {

        QFile file1(radFile);

        if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {

            std::cout << "Reading RAD file..." << std::endl;

            QTextStream in1(&file1);
            do {

                 QString wordy;
                 in1 >> wordy; // clé de lecture
                 std::cout << "First word of the line: " << wordy.toStdString() << std::endl;

                 if ( wordy.isEmpty() || wordy[0] == '#' ) in1.readLine();
                 else if ( wordy == "void" ) { // définition de matériaux
                     in1 >> wordy; // material type
                     if (wordy == "alias") { // définition d'un alias
                         QString newOne, oldOne;
                         in1 >> newOne;
                         in1 >> oldOne;
                         qDebug("Material %s alias of %s", newOne.toStdString().c_str(), oldOne.toStdString().c_str());
                         mapMaterials.insert( std::pair<std::string,Color>(newOne.toStdString(), mapMaterials.at(oldOne.toStdString())) );
                     }
                     else {
                         QString label, red, green, blue;
                         in1 >> label; // label
                         qDebug("Material definition of: %s", label.toStdString().c_str());
                         in1 >> wordy; // 0
                         in1 >> wordy; // 0
                         in1 >> wordy; // number of arguments
                         in1 >> red;
                         in1 >> green;
                         in1 >> blue;
                         unsigned int nbArgs = wordy.toUInt();
                         for (unsigned int i=0; i<nbArgs-3; ++i) in1 >> wordy;
                         // material creation
                         mapMaterials.insert( std::pair<std::string,Color>(label.toStdString(), Color(red.toFloat(), green.toFloat(), blue.toFloat())) );
                     }
                 }
                 else if (wordy == "inherit") { // définition d'un alias
                     QString newOne, oldOne;
                     in1 >> wordy; // alias
                     in1 >> newOne;
                     in1 >> oldOne;
                     qDebug("Material %s alias of %s", newOne.toStdString().c_str(), oldOne.toStdString().c_str());
                     mapMaterials.insert( std::pair<std::string,Color>(newOne.toStdString(), mapMaterials.at(oldOne.toStdString())) );
                 }
                 else if ( mapMaterials.find(wordy.toStdString()) != mapMaterials.end() ) {
                    QString label = wordy; // label of the material
                    // found a material in the set... and creates the corresponding polygon
                    mapSurfaces.push_back( std::pair<std::string,Surface>( label.toStdString(), Surface() ) );
                    in1 >> wordy; // polygon
                    if ( wordy == "polygon" ) {
                        in1 >> wordy; // label of the polygon
                        qDebug("Polygon: %s of %s", wordy.toStdString().c_str(), label.toStdString().c_str());
                        in1 >> wordy; // 0
                        in1 >> wordy; // 0
                        in1 >> wordy; // number of points
                        unsigned int uiNbPoints = wordy.toUInt();
                        for (unsigned int i=0; i<uiNbPoints; ++i) {
                           in1 >> wordy;
                           ((mapSurfaces.back()).second).push_back(wordy.toFloat());
                        }
                    }
                 }

            } while (!in1.atEnd());

        }

    }

    std::cout << "Materials loaded: " << mapMaterials.size() << std::endl;
    std::cout << "Surfaces loaded: " << mapSurfaces.size() << std::endl;

    // calculation of the bounding box
    if (!mapSurfaces.empty()) {
        // arbitrarily sets of the value to the first element
        xMax = mapSurfaces.begin()->second.getMaxX();
        xMin = mapSurfaces.begin()->second.getMinX();
        yMax = mapSurfaces.begin()->second.getMaxY();
        yMin = mapSurfaces.begin()->second.getMinY();
        zMax = mapSurfaces.begin()->second.getMaxZ();
        zMin = mapSurfaces.begin()->second.getMinZ();

        for (std::vector<std::pair<std::string,Surface> >::iterator it = mapSurfaces.begin()+1; it != mapSurfaces.end(); ++it) {
            xMax = qMax(static_cast<float>(xMax), it->second.getMaxX());
            xMin = qMin(static_cast<float>(xMin), it->second.getMinX());
            yMax = qMax(static_cast<float>(yMax), it->second.getMaxY());
            yMin = qMin(static_cast<float>(yMin), it->second.getMinY());
            zMax = qMax(static_cast<float>(zMax), it->second.getMaxZ());
            zMin = qMin(static_cast<float>(zMin), it->second.getMinZ());
        }
    }
    else {
        // values set to zero, nothing in the scene
        xMax=0.0;
        xMin=0.0;
        yMax=0.0;
        yMin=0.0;
        zMax=0.0;
        zMin=0.0;
    }
    std::cout << "Bounding box: (" << xMin << "," << xMax << ") - (" << yMin << "," << yMax << ") - (" << zMin << "," << zMax << ")" << std::endl;

    // start of the list
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    // Draw the surfaces
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL); // remplissage
    for (std::vector<std::pair<std::string,Surface> >::iterator it=mapSurfaces.begin(); it != mapSurfaces.end(); ++it) {
        if ( it->second.getnCoordinates()/3 == 3 ) { // triangle
            glBegin(GL_TRIANGLES);
            glColor3f((mapMaterials.at(it->first)).getRed(),(mapMaterials.at(it->first)).getGreen(),(mapMaterials.at(it->first)).getBlue());
            for (unsigned int i=0; i<(*it).second.getnCoordinates()/3; ++i) glVertex3f((*it).second.getCoordinate(3*i), (*it).second.getCoordinate(3*i+1), (*it).second.getCoordinate(3*i+2));
            glEnd();
        }
        else if ( it->second.getnCoordinates()/3 == 4 ) { // quad
            glBegin(GL_QUADS);
            glColor3f((mapMaterials.at(it->first)).getRed(),(mapMaterials.at(it->first)).getGreen(),(mapMaterials.at(it->first)).getBlue());
            for (unsigned int i=0; i<(*it).second.getnCoordinates()/3; ++i) glVertex3f((*it).second.getCoordinate(3*i), (*it).second.getCoordinate(3*i+1), (*it).second.getCoordinate(3*i+2));
            glEnd();
        }
        else { // polygons
            glBegin(GL_POLYGON);
            glColor3f((mapMaterials.at(it->first)).getRed(),(mapMaterials.at(it->first)).getGreen(),(mapMaterials.at(it->first)).getBlue());
            for (unsigned int i=0; i<(*it).second.getnCoordinates()/3; ++i) glVertex3f((*it).second.getCoordinate(3*i), (*it).second.getCoordinate(3*i+1), (*it).second.getCoordinate(3*i+2));
            glEnd();
        }
    }
    /*
    glPolygonMode(GL_FRONT_AND_BACK , GL_LINE); // remplissage
    for (vector<pair<string,Surface> >::iterator it=mapSurfaces.begin(); it != mapSurfaces.end(); ++it) {
        if ( it->second.getnCoordinates()/3 == 3 ) { // triangle
            //cout << "Triangle of material: " << it->first << endl;
            //cout << "coordinates: " << endl;
            for (unsigned int i=0; i<it->second.getnCoordinates()/3; ++i) cout << "\t" << it->second.getCoordinate(3*i) << "\t" << it->second.getCoordinate(3*i+1) << "\t" << it->second.getCoordinate(3*i+2) << endl;
            glBegin(GL_TRIANGLES);
            glColor3f(0.0,0.0,0.0);
            for (unsigned int i=0; i<(*it).second.getnCoordinates()/3; ++i) glVertex3f((*it).second.getCoordinate(3*i), (*it).second.getCoordinate(3*i+1), (*it).second.getCoordinate(3*i+2));
            glEnd();
        }
        else if ( it->second.getnCoordinates()/3 == 4 ) { // quad
            //cout << "Quad of material: " << it->first << endl;
            //cout << "coordinates: " << endl;
            for (unsigned int i=0; i<it->second.getnCoordinates()/3; ++i) cout << "\t" << it->second.getCoordinate(3*i) << "\t" << it->second.getCoordinate(3*i+1) << "\t" << it->second.getCoordinate(3*i+2) << endl;
            glBegin(GL_QUADS);
            glColor3f(0.0,0.0,0.0);
            for (unsigned int i=0; i<(*it).second.getnCoordinates()/3; ++i) glVertex3f((*it).second.getCoordinate(3*i), (*it).second.getCoordinate(3*i+1), (*it).second.getCoordinate(3*i+2));
            glEnd();
        }
        else { // polygons
            //cout << "Polygon of material: " << it->first << endl;
            //cout << "coordinates: " << endl;
            for (unsigned int i=0; i<it->second.getnCoordinates()/3; ++i) cout << "\t" << it->second.getCoordinate(3*i) << "\t" << it->second.getCoordinate(3*i+1) << "\t" << it->second.getCoordinate(3*i+2) << endl;
            glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            for (unsigned int i=0; i<(*it).second.getnCoordinates()/3; ++i) glVertex3f((*it).second.getCoordinate(3*i), (*it).second.getCoordinate(3*i+1), (*it).second.getCoordinate(3*i+2));
            glEnd();
        }
    }
    */

    glEndList();

    // just read a new object to be displayed, so no image
    image = false;

    return list;
}

void GLWidget::deleteObject() {

    mapMaterials.clear();
    mapSurfaces.clear();
    glDeleteLists(object, 1);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
     lastPos = event->pos();
     image = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
     dx = event->position().x() - lastPos.x();
     dy = event->position().y() - lastPos.y();
     lastPos = event->pos();

     update();

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
      // set to zero the rotations
      dx = 0;
      dy = 0;
      event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    wheelRot += event->angleDelta().y();
    image = false;
    repaint();
    wheelRot = 0;
    event->accept();
}

void GLWidget::displayImage(QString filename)
{
    // creates a texture with the filename
    imageTexture.load(filename);
    std::cout << "Texture size x: " << imageTexture.width() << "\ty: " << imageTexture.height() << std::endl;

    texture = new QOpenGLTexture(imageTexture.mirrored());

    image = true;

    std::cout << "Texture created and binded" << std::endl;
    // updates the screen
    update();

}

void GLWidget::setModel(int value) {

    this->model = value;
    deleteObject();
    object = makeObject(QString::fromStdString("base_" + toString(model) + ".rad"));
    update();

}
