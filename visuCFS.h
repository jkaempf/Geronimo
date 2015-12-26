#ifndef VISUCFS_H
#define VISUCFS_H

#include "ui_window.h"
///#include "lightmaps.h"
#include "radsimul.h"

class MaForme : public QWidget {

    Q_OBJECT

public:
    MaForme(QWidget *parent = 0);
    ~MaForme() { /*if (w != NULL) delete w;*/ }
    void setMaxValue();
    void setDFImage();
    void setDFToolTip(QString value) {
#ifndef QT_NO_TOOLTIP
        ui.dfLabel->setToolTip(QApplication::translate("VisuCFS", value.toStdString().c_str(), 0));
#endif // QT_NO_TOOLTIP
    }

public slots:

    void getPath();
    void getFileBSDF();
    void getFilePrism2();
    void setModelChanged(int value) { ui.widget->setModel(value); radSimul.setModel(value); }
    void setPosition(qreal latitude, qreal longitude);
    void getPosition();
    void setPredefinedDay(bool value);
    void setSummerSolstice(bool value);
    void setWinterSolstice(bool value);
    void setSpringEquinox(bool value);
    void setHour(QString value);
    void setSiteOrientation(int value);
    void setSiteOrientation(QString text);

    void setModeChanged(int value);
    void setFalseColor(bool value) { radSimul.setFalseColor(value); radSimul.setLuminance(ui.luminance->isChecked()); radSimul.setBluminance(ui.bluminance->isChecked()); setMaxValue(); }
    void setLuminance(bool value) { radSimul.setLuminance(value); setMaxValue(); }
    void setBluminance(bool value) { radSimul.setBluminance(value); setMaxValue(); }
    void setDaylightFactor(bool value) { radSimul.setDaylight(value); radSimul.setLogScale(ui.logScale->isChecked()); ui.dfLabel->clear(); }
    void setDFRequiredIlluminance() { radSimul.setDFReqIllum(ui.reqIlluminance->text()); setDFImage(); setDFToolTip(radSimul.getDFmsg()); }
    void setLogScale(bool value) { radSimul.setLogScale(value); setDFImage(); setDFToolTip(radSimul.getDFmsg()); }
    void setRadianceParameters();
    void setRadianceParametersMkillum();

    void setQuality() { setPredefinedRadianceParameters(); }
    void setDetail() { setPredefinedRadianceParameters(); }
    void setVariability() { setPredefinedRadianceParameters(); }
    void setIndirect() { setPredefinedRadianceParameters(); }

    // glare calculation and getting the glare index from RadianceSimulation
    void setGlareRiskCalculation(bool value) { radSimul.setGlare(value); setGlareRiskIndex(); }
    void setGlareRiskIndex() {
        // lookup for the glare result and show it if available
        if (radSimul.glareIndices.find(ui.comboBox_glare->currentText().toLower().toStdString()) != radSimul.glareIndices.end()) {
            ui.glare_lineEdit->setText(QString::number(radSimul.glareIndices.find(ui.comboBox_glare->currentText().toLower().toStdString())->second,'f', 5));
        }
        else {
            ui.glare_lineEdit->clear();
        }
    }

    void radianceSimulation_clicked();
    void convertBTDF_clicked();
    void freezeWindow();
    void displayTaskState(int errorState, QString description);
    void releaseWindow();
    void loadSimulationResults();

    void setMaxValue(QString textValue);

    void setFileBSDF(QString textValue) { radSimul.setBSDF_file(textValue); }
    void setFilePrism2(QString textValue) { radSimul.setPrism2_file(textValue); }
    void setGlassTransmissivity(QString textValue) { radSimul.setGlass_transmissivity(textValue); }

    void setView(float vpx, float vpy, float vpz, float phi, float theta) {
        // writes the coordinates in the front window
        ui.lineEdit_vpx->setText(QString::number(vpx,'f',2));
        ui.lineEdit_vpy->setText(QString::number(vpy,'f',2));
        ui.lineEdit_vpz->setText(QString::number(vpz,'f',2));
        ui.lineEdit_phi->setText(QString::number(phi,'f',0));
        ui.lineEdit_theta->setText(QString::number(theta,'f',0));
    }
    void setViewPoint() { ui.widget->setViewPoint(ui.lineEdit_vpx->text().toFloat(), ui.lineEdit_vpy->text().toFloat(), ui.lineEdit_vpz->text().toFloat()); }
    void setViewDirection() { ui.widget->setViewDirection(ui.lineEdit_phi->text().toFloat(), ui.lineEdit_theta->text().toFloat()); }
    //void setScale(QString scale) { ui.widget->setScale(scale.toFloat()); radSimul.setScale(scale.toFloat()); }
    void setScale() { ui.widget->setScale(ui.lineEdit_scale->text().toFloat()); radSimul.setScale(ui.lineEdit_scale->text().toFloat()); }

    void closeEvent(QCloseEvent *event) {
        radSimul.quit();
        event->accept();
    }

 private:

    Ui::VisuCFS ui;
    ///LightMaps *w;
    RadianceSimulation radSimul;

    void setPredefinedRadianceParameters();

};

#endif // VISUCFS_H
