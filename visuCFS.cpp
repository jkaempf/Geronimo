#include <QtGui>
#include <QGLWidget>

#include "visuCFS.h"
#include "glwidget.h"

MaForme::MaForme(QWidget *parent):QWidget(parent)
{
    ui.setupUi(this);

    QPalette p = ui.lineEdit_c1->palette();
    p.setColor( QPalette::Normal, QPalette::Base, QColor::fromRgb( 254, 252, 48 ));
    ui.lineEdit_c0->setPalette(p);
    p.setColor( QPalette::Normal, QPalette::Base, QColor::fromRgb( 253, 140, 15 ));
    ui.lineEdit_c1->setPalette(p);
    p.setColor( QPalette::Normal, QPalette::Base, QColor::fromRgb( 170, 32, 41 ));
    ui.lineEdit_c2->setPalette(p);
    p.setColor( QPalette::Normal, QPalette::Base, QColor::fromRgb( 144, 115, 89 ));
    ui.lineEdit_c3->setPalette(p);
    p.setColor( QPalette::Normal, QPalette::Base, QColor::fromRgb( 18, 178, 140 ));
    ui.lineEdit_c4->setPalette(p);
    p.setColor( QPalette::Normal, QPalette::Base, QColor::fromRgb( 10, 22, 183 ));
    ui.lineEdit_c5->setPalette(p);

    // setup the lightmap
    ///w = NULL;

    // connections for the signal/slots
    // the shellcmd
    connect(&radSimul.shellcmd, SIGNAL(started()), this, SLOT(freezeWindow()));
    connect(&radSimul.shellcmd, SIGNAL(taskState(int,QString)), this, SLOT(displayTaskState(int,QString)));
    connect(&radSimul.shellcmd, SIGNAL(finished()), this, SLOT(releaseWindow()));
    // the run of radSimul
    connect(&radSimul, SIGNAL(started()), this, SLOT(freezeWindow()));
    connect(&radSimul, SIGNAL(finished()), this, SLOT(loadSimulationResults()));
    // the connections of the window elements to create the sky
    connect(ui.latitude, SIGNAL(textChanged(QString)), &radSimul, SLOT(setLatitude(QString)));
    connect(ui.longitude, SIGNAL(textChanged(QString)), &radSimul, SLOT(setLongitude(QString)));
    connect(ui.meridian, SIGNAL(textChanged(QString)), &radSimul, SLOT(setMeridian(QString)));
    connect(ui.month, SIGNAL(textChanged(QString)), &radSimul, SLOT(setMonth(QString)));
    connect(ui.day, SIGNAL(textChanged(QString)), &radSimul, SLOT(setDay(QString)));
    // the connections of the window elements to create the octree
    connect(ui.mkillum, SIGNAL(toggled(bool)), &radSimul, SLOT(setMkillum(bool)));
    connect(ui.prism2, SIGNAL(toggled(bool)), &radSimul, SLOT(setPrism2(bool)));
    connect(ui.bsdf, SIGNAL(toggled(bool)), &radSimul, SLOT(setBSDF(bool)));

    // default values
    ui.latitude->setText("46");
    ui.longitude->setText("6");
    ui.meridian->setText("1");
    ui.orientationDial->setValue(0);
    ui.month->setText("1");
    ui.day->setText("1");
    ui.hour->setText("12");
    ui.lineEdit_Indirect->setText("1");
    //setModelChanged(ui.comboBox_model->currentIndex());
    setModeChanged(ui.comboBox_analysisMode->currentIndex());
    ui.radianceParameters->setPlainText("-u -ds .1 -ab 2 -av 0 0 0 -ad 256 -as 128 -aa .2 -ar 32 -dc 0.75 -dp 4096 -dj 0.9 -st 0.01 -dt 0.05 -lr 10 -lw 0.002");
    ui.mkillumParameters->setVisible(false);
    ui.mkillumParameters->setPlainText("-dr 1 -ab 1 -ds .02 -ad 128 -as 32 -aa .2 -dj .1 -dt .5 -dc .25 -lr 30 -lw 0.01");
    ui.reqIlluminance->setText("300");
    ui.logScale->setChecked(true);
    ui.lineEdit_glass->setText("0.88");
    ui.lineEdit_scale->setText("1");

}

void MaForme::setMaxValue() {

    float value = radSimul.getMaxValue();
    if (value > 0.f) {
        ui.lineEdit_c0->setText(QString::number(value,'f',0));
        ui.lineEdit_c1->setText(QString::number(value*0.9f,'f',0));
        ui.lineEdit_c2->setText(QString::number(value*0.7f,'f',0));
        ui.lineEdit_c3->setText(QString::number(value*0.5f,'f',0));
        ui.lineEdit_c4->setText(QString::number(value*0.3f,'f',0));
        ui.lineEdit_c5->setText(QString::number(value*0.1f,'f',0));
    }
    else {
        ui.lineEdit_c0->setText("");
        ui.lineEdit_c1->setText("");
        ui.lineEdit_c2->setText("");
        ui.lineEdit_c3->setText("");
        ui.lineEdit_c4->setText("");
        ui.lineEdit_c5->setText("");
    }

}

void MaForme::setMaxValue(QString textValue) {

    float value = textValue.toFloat();
    radSimul.setMaxValue(value);
    if (value > 0.f) {
        ui.lineEdit_c0->setText(QString::number(value,'f',0));
        ui.lineEdit_c1->setText(QString::number(value*0.9f,'f',0));
        ui.lineEdit_c2->setText(QString::number(value*0.7f,'f',0));
        ui.lineEdit_c3->setText(QString::number(value*0.5f,'f',0));
        ui.lineEdit_c4->setText(QString::number(value*0.3f,'f',0));
        ui.lineEdit_c5->setText(QString::number(value*0.1f,'f',0));
    }
    else {
        ui.lineEdit_c0->setText("");
        ui.lineEdit_c1->setText("");
        ui.lineEdit_c2->setText("");
        ui.lineEdit_c3->setText("");
        ui.lineEdit_c4->setText("");
        ui.lineEdit_c5->setText("");
    }

}

void MaForme::setDFImage() {

    //ui.dfLabel->setPixmap(QString::fromStdString("mesh_" + toString(ui.comboBox_model->currentIndex()) + ".bmp"));
    ui.dfLabel->setPixmap(radSimul.getDFimage());

}

void MaForme::getPath()
{

    QString directoryName = QFileDialog::getExistingDirectory(
        this,
        "Choose a directory to open",
        QString::null,
        QFileDialog::ShowDirsOnly);

    if (!directoryName.isEmpty())
        ui.filesDirectory->setText(directoryName);

}

void MaForme::getFileBSDF()
{

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Window XML File"),
        QString::null,
        tr("XML file (*.xml)"));

    if (!fileName.isEmpty())
        ui.lineEdit_bsdf->setText(fileName);

}

void MaForme::getFilePrism2()
{

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Prism2 CAL file"),
        QString::null,
        tr("CAL file (*.cal)"));

    if (!fileName.isEmpty())
        ui.lineEdit_prism2->setText(fileName);

}

void MaForme::setPosition(qreal latitude, qreal longitude)
{
    ui.latitude->setText(QString::number(latitude,'f',1));
    ui.longitude->setText(QString::number(longitude,'f',1));

}

void MaForme::getPosition()
{

//    QWebView *webView = new QWebView(this);
//    webView->resize(600,450);
//    webView->move(10,10);
//    QString gMapURL = "England"; // this is where you want to point
//    gMapURL = "http://maps.google.com.sg/maps?q="+gMapURL+"&oe=utf-8&rls=org.mozilla:en-US:official&client=firefox-a&um=1&ie=UTF-8&hl=en&sa=N&tab=wl";
//    webView->setUrl(QUrl(gMapURL));

//    if ( w == NULL) {
//        w = new LightMaps(ui.latitude->text().toDouble(), ui.longitude->text().toDouble());
//        connect( w->m_normalMap, SIGNAL( positionChanged(qreal,qreal) ), this, SLOT( setPosition(qreal,qreal) ) );
//        w->setWindowTitle("OpenStreetMap");
//    }
//    w->resize(600, 450);
//    webView->show();

}

void MaForme::setPredefinedDay(bool value) {

    if (value) {
        if (ui.summerSolstice->isChecked()) { setSummerSolstice(true); return; }
        if (ui.winterSolstice->isChecked()) { setWinterSolstice(true); return; }
        if (ui.springEquinox->isChecked()) { setSpringEquinox(true); return; }
    }

}

void MaForme::setSummerSolstice(bool value) {

    if (value) {
        ui.month->setText("6");
        ui.day->setText("21");
    }
}

void MaForme::setWinterSolstice(bool value) {

    if (value) {
        ui.month->setText("12");
        ui.day->setText("21");
    }
}

void MaForme::setSpringEquinox(bool value) {

    if (value) {
        ui.month->setText("3");
        ui.day->setText("21");
    }
}

void MaForme::setHour(QString text) {
    radSimul.setHour(text.toFloat());
}

void MaForme::setSiteOrientation(int value) {
    ui.orientation->setText(QString::number(value));
}

void MaForme::setSiteOrientation(QString text) {
    // make sure that we are within 360° range
    int value = text.toInt()%360;
    ui.orientationDial->setValue(value);
    radSimul.setSiteOrientation(value);
}

void MaForme::setModeChanged(int value) {

    if (value == 0) {
        // make things disappear
        ui.falsecolor->setHidden(true);
        ui.daylightFactor->setHidden(true);
        ui.glare->setHidden(true);
        ui.radianceParameters->setHidden(true);
        ui.mkillumParameters->setDisabled(true);
        ui.renderingSettings->setHidden(true);
    }
    else if (value == 1) {
        // make things disapepar
        ui.falsecolor->setVisible(true);
        ui.daylightFactor->setVisible(true);
        ui.glare->setVisible(true);
        ui.radianceParameters->setVisible(true);
        ui.radianceParameters->setDisabled(true);
        ui.mkillumParameters->setDisabled(true);
        ui.renderingSettings->setVisible(true);
        ui.label_Quality->setDisabled(false);
        ui.label_Detail->setDisabled(false);
        ui.label_Variability->setDisabled(false);
        ui.label_Indirect->setDisabled(false);
        ui.horizontalSlider_Quality->setDisabled(false);
        ui.horizontalSlider_Detail->setDisabled(false);
        ui.horizontalSlider_Variability->setDisabled(false);
        ui.lineEdit_Indirect->setDisabled(false);
    }
    else {
        // make things appear
        ui.falsecolor->setVisible(true);
        ui.daylightFactor->setVisible(true);
        ui.glare->setVisible(true);
        ui.radianceParameters->setVisible(true);
        ui.radianceParameters->setDisabled(false);
        ui.mkillumParameters->setDisabled(false);
        ui.renderingSettings->setVisible(true);
        ui.label_Quality->setDisabled(true);
        ui.label_Detail->setDisabled(true);
        ui.label_Variability->setDisabled(true);
        ui.label_Indirect->setDisabled(true);
        ui.horizontalSlider_Quality->setDisabled(true);
        ui.horizontalSlider_Detail->setDisabled(true);
        ui.horizontalSlider_Variability->setDisabled(true);
        ui.lineEdit_Indirect->setDisabled(true);
    }
}

void MaForme::setPredefinedRadianceParameters() {

    QString radParam = "-u+ -av 0 0 0 ";
    // the INDIRECT parameter
    radParam += "-ab ";
    radParam += QString::number(ui.lineEdit_Indirect->text().toInt()+1);
    // tha AMBIENT parameters
    if (ui.horizontalSlider_Quality->value()==0) {
        // low quality
        radParam += " -ds 0.4 -dj 0 -dt 0.2 -dc 0.25";
        // detail
        if (ui.horizontalSlider_Detail->value()==0) {
            // low
            radParam += " -dr 0 -dp 64";
            radParam += " -ar 4";
        }
        else if (ui.horizontalSlider_Detail->value()==1) {
            // medium
            radParam += " -dr 0 -dp 128";
            radParam += " -ar 8";
        }
        else {
            // high
            radParam += " -dr 0 -dp 256";
            radParam += " -ar 16";
        }
        // variability
        if (ui.horizontalSlider_Variability->value()==0) {
            // low
            radParam += " -aa 0.4 -ad 64 -as 0";
        }
        else if (ui.horizontalSlider_Variability->value()==1) {
            // medium
            radParam += " -aa 0.2 -ad 128 -as 0";
        }
        else {
            // high
            radParam += " -aa 0 -ad 256 -as 0";
        }
    }
    else if (ui.horizontalSlider_Quality->value()==1) {
        // medium quality
        radParam += " -ds 0.2 -dj 0.5 -dt 0.1 -dc 0.5";
        if (ui.horizontalSlider_Detail->value()==0) {
            // low detail
            radParam += " -dr 1 -dp 256";
            radParam += " -ar 8";
        }
        else if (ui.horizontalSlider_Detail->value()==1) {
            // medium detail
            radParam += " -dr 1 -dp 512";
            radParam += " -ar 16";
        }
        else {
            // high detail
            radParam += " -dr 1 -dp 1024";
            radParam += " -ar 32";
        }
        // variability
        if (ui.horizontalSlider_Variability->value()==0) {
            // low
            radParam += " -aa 0.3 -ad 128 -as 64";
        }
        else if (ui.horizontalSlider_Variability->value()==1) {
            // medium
            radParam += " -aa 0.15 -ad 256 -as 128";
        }
        else {
            // high
            radParam += " -aa 0 -ad 512 -as 256";
        }
    }
    else {
        // high quality
        radParam += " -ds 0.01 -dj 0.6 -dt 0.05 -dc 0.17";
        if (ui.horizontalSlider_Detail->value()==0) {
            // low detail
            radParam += " -dr 3 -dp 1024";
            radParam += " -ar 16";
        }
        else if (ui.horizontalSlider_Detail->value()==1) {
            // medium detail
            radParam += " -dr 3 -dp 2048";
            radParam += " -ar 32";
        }
        else {
            // high detail
            radParam += " -dr 3 -dp 4096";
            radParam += " -ar 64";
        }
        // variability
        if (ui.horizontalSlider_Variability->value()==0) {
            // low
            radParam += " -aa 0.25 -ad 256 -as 128";
        }
        else if (ui.horizontalSlider_Variability->value()==1) {
            // medium
            radParam += " -aa 0.125 -ad 512 -as 256";
        }
        else {
            // high
            radParam += " -aa 0 -ad 1024 -as 512";
        }
    }
    ui.radianceParameters->setPlainText(radParam);

}

void MaForme::setRadianceParameters() {

    radSimul.setRadianceParameters(ui.radianceParameters->toPlainText());
    radSimul.setRadianceParametersMkillum(ui.radianceParameters->toPlainText());

}

void MaForme::setRadianceParametersMkillum() {

    radSimul.setRadianceParametersMkillum(ui.mkillumParameters->toPlainText());

}

void MaForme::radianceSimulation_clicked()
{
    // sets the important elements for the simulation
    radSimul.setImageSize(ui.widget->getWidth(), ui.widget->getHeight());
    radSimul.setView(ui.widget->getViewPoint(), ui.widget->getViewDirection(), ui.widget->getViewUp());
    radSimul.setBoundingBox(ui.widget->getBoundingBox());

    // do the raytracing
    radSimul.start();

}

void MaForme::convertBTDF_clicked() {

    if (!ui.filesDirectory->text().isEmpty())
        radSimul.convertBTDF(ui.filesDirectory->text());
    else {
        ui.label_status->setText("Conversion BTDF in RADIANCE format: empty directory error.");
        releaseWindow();
    }

}

void MaForme::freezeWindow() {

    // puts back the window in enabled mode
    this->setDisabled(true);

}

void MaForme::displayTaskState(int errorState, QString description) {

    if (errorState == -1)
        ui.label_status->setText(description.append(": running."));
    else if (errorState == 0)
        ui.label_status->setText(description.append(": done."));
    else
        ui.label_status->setText(description.append(": error."));

}

void MaForme::releaseWindow() {

    // puts back the window in enabled mode
    this->setDisabled(false);

}

void MaForme::loadSimulationResults() {

    // sets the max luminance
    if (ui.luminance->isChecked() || ui.bluminance->isChecked()) setMaxValue();
    // sets the DF results
    if (ui.daylightFactor->isChecked()) { setDFImage(); setDFToolTip(radSimul.getDFmsg()); }
    // set the glare results
    if (ui.glare->isChecked()) setGlareRiskIndex();
    // displays the image instead of the 3D projection
    ui.widget->displayImage(QString::fromStdString("base_" + toString(ui.comboBox_model->currentIndex()) + ".bmp"));
    ui.widget->show();
    // puts back the window in enabled mode
    this->setDisabled(false);

}

void MaForme::setView(float vpx, float vpy, float vpz, float vdx, float vdy, float vdz) {
    // writes the coordinates in the front window
    ui.lineEdit_vpx->setText(QString::number(vpx,'f',2));
    ui.lineEdit_vpy->setText(QString::number(vpy,'f',2));
    ui.lineEdit_vpz->setText(QString::number(vpz,'f',2));
    ui.lineEdit_vdx->setText(QString::number(vdx,'f',2));
    ui.lineEdit_vdy->setText(QString::number(vdy,'f',2));
    ui.lineEdit_vdz->setText(QString::number(vdz,'f',2));
}
