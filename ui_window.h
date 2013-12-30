/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created: Mon 30. Dec 17:36:26 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDial>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_VisuCFS
{
public:
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *month;
    QLineEdit *day;
    QPushButton *pushButton_radSimul;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *latitude;
    QLineEdit *longitude;
    QLineEdit *meridian;
    QLabel *label_10;
    GLWidget *widget;
    QPushButton *pushButton_map;
    QDial *orientationDial;
    QLabel *label_5;
    QGroupBox *btdfMethod;
    QRadioButton *prism2;
    QRadioButton *glass;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QRadioButton *bsdf;
    QCheckBox *mkillum;
    QPlainTextEdit *mkillumParameters;
    QLabel *label_6;
    QGroupBox *predefinedDay;
    QRadioButton *summerSolstice;
    QRadioButton *winterSolstice;
    QRadioButton *springEquinox;
    QGroupBox *falsecolor;
    QLineEdit *lineEdit_c1;
    QLineEdit *lineEdit_c2;
    QLineEdit *lineEdit_c3;
    QLineEdit *lineEdit_c4;
    QLineEdit *lineEdit_c5;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLineEdit *lineEdit_c0;
    QRadioButton *illuminance;
    QRadioButton *bluminance;
    QComboBox *comboBox_analysisMode;
    QComboBox *comboBox_model;
    QLabel *label_21;
    QGroupBox *daylightFactor;
    QLabel *dfLabel;
    QLabel *label_24;
    QLineEdit *reqIlluminance;
    QRadioButton *logScale;
    QRadioButton *linScale;
    QGroupBox *glare;
    QComboBox *comboBox_glare;
    QLineEdit *glare_lineEdit;
    QGroupBox *groupBox;
    QLineEdit *filesDirectory;
    QPushButton *pushButton_browse;
    QPushButton *pushButton_convert;
    QLabel *label_status;
    QGroupBox *renderingSettings;
    QLabel *label_Quality;
    QSlider *horizontalSlider_Quality;
    QLabel *label_Detail;
    QSlider *horizontalSlider_Detail;
    QLabel *label_Variability;
    QSlider *horizontalSlider_Variability;
    QLabel *label_Indirect;
    QLineEdit *lineEdit_Indirect;
    QPlainTextEdit *radianceParameters;
    QLabel *label_26;
    QLabel *label_27;
    QLineEdit *orientation;
    QLineEdit *hour;

    void setupUi(QWidget *VisuCFS)
    {
        if (VisuCFS->objectName().isEmpty())
            VisuCFS->setObjectName(QString::fromUtf8("VisuCFS"));
        VisuCFS->resize(1051, 661);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VisuCFS->sizePolicy().hasHeightForWidth());
        VisuCFS->setSizePolicy(sizePolicy);
        VisuCFS->setMinimumSize(QSize(1051, 661));
        VisuCFS->setMaximumSize(QSize(1051, 661));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icon/leso.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        VisuCFS->setWindowIcon(icon);
        label_2 = new QLabel(VisuCFS);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 160, 46, 14));
        QFont font;
        font.setPointSize(9);
        label_2->setFont(font);
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_3 = new QLabel(VisuCFS);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(130, 160, 46, 14));
        label_3->setFont(font);
        label_3->setLayoutDirection(Qt::RightToLeft);
        label_4 = new QLabel(VisuCFS);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 290, 31, 16));
        label_4->setFont(font);
        label_4->setLayoutDirection(Qt::LeftToRight);
        month = new QLineEdit(VisuCFS);
        month->setObjectName(QString::fromUtf8("month"));
        month->setEnabled(true);
        month->setGeometry(QRect(70, 160, 41, 20));
        QFont font1;
        font1.setPointSize(8);
        month->setFont(font1);
        month->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        month->setReadOnly(false);
        day = new QLineEdit(VisuCFS);
        day->setObjectName(QString::fromUtf8("day"));
        day->setEnabled(true);
        day->setGeometry(QRect(170, 160, 41, 20));
        day->setFont(font1);
        day->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        day->setReadOnly(false);
        pushButton_radSimul = new QPushButton(VisuCFS);
        pushButton_radSimul->setObjectName(QString::fromUtf8("pushButton_radSimul"));
        pushButton_radSimul->setGeometry(QRect(250, 630, 141, 23));
        pushButton_radSimul->setFont(font);
        label_7 = new QLabel(VisuCFS);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 60, 81, 16));
        label_7->setFont(font);
        label_8 = new QLabel(VisuCFS);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 80, 81, 16));
        label_8->setFont(font);
        label_9 = new QLabel(VisuCFS);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 100, 91, 16));
        label_9->setFont(font);
        latitude = new QLineEdit(VisuCFS);
        latitude->setObjectName(QString::fromUtf8("latitude"));
        latitude->setGeometry(QRect(120, 60, 41, 20));
        latitude->setFont(font1);
        latitude->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        longitude = new QLineEdit(VisuCFS);
        longitude->setObjectName(QString::fromUtf8("longitude"));
        longitude->setGeometry(QRect(120, 80, 41, 20));
        longitude->setFont(font1);
        longitude->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        meridian = new QLineEdit(VisuCFS);
        meridian->setObjectName(QString::fromUtf8("meridian"));
        meridian->setGeometry(QRect(120, 100, 41, 20));
        meridian->setFont(font1);
        meridian->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_10 = new QLabel(VisuCFS);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 130, 91, 16));
        label_10->setFont(font);
        widget = new GLWidget(VisuCFS);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(230, 20, 600, 600));
        pushButton_map = new QPushButton(VisuCFS);
        pushButton_map->setObjectName(QString::fromUtf8("pushButton_map"));
        pushButton_map->setGeometry(QRect(170, 60, 41, 23));
        pushButton_map->setFont(font);
        orientationDial = new QDial(VisuCFS);
        orientationDial->setObjectName(QString::fromUtf8("orientationDial"));
        orientationDial->setGeometry(QRect(170, 100, 50, 64));
        orientationDial->setFont(font);
        orientationDial->setCursor(QCursor(Qt::ArrowCursor));
        orientationDial->setMaximum(359);
        orientationDial->setValue(180);
        orientationDial->setWrapping(true);
        label_5 = new QLabel(VisuCFS);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(170, 90, 51, 20));
        QPalette palette;
        QBrush brush(QColor(170, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_5->setPalette(palette);
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);
        btdfMethod = new QGroupBox(VisuCFS);
        btdfMethod->setObjectName(QString::fromUtf8("btdfMethod"));
        btdfMethod->setGeometry(QRect(20, 410, 191, 241));
        btdfMethod->setFont(font);
        btdfMethod->setFlat(false);
        btdfMethod->setCheckable(false);
        btdfMethod->setChecked(false);
        prism2 = new QRadioButton(btdfMethod);
        prism2->setObjectName(QString::fromUtf8("prism2"));
        prism2->setGeometry(QRect(20, 80, 91, 20));
        prism2->setFont(font);
        prism2->setLayoutDirection(Qt::RightToLeft);
        glass = new QRadioButton(btdfMethod);
        glass->setObjectName(QString::fromUtf8("glass"));
        glass->setGeometry(QRect(80, 40, 51, 20));
        glass->setLayoutDirection(Qt::LeftToRight);
        glass->setChecked(true);
        label_12 = new QLabel(btdfMethod);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(20, 30, 41, 41));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/Icon/glass.bmp")));
        label_12->setScaledContents(true);
        label_13 = new QLabel(btdfMethod);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(130, 70, 41, 41));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/Icon/prism2.bmp")));
        label_13->setScaledContents(true);
        label_14 = new QLabel(btdfMethod);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 110, 41, 41));
        label_14->setPixmap(QPixmap(QString::fromUtf8(":/Icon/mkillum.bmp")));
        label_14->setScaledContents(true);
        bsdf = new QRadioButton(btdfMethod);
        bsdf->setObjectName(QString::fromUtf8("bsdf"));
        bsdf->setGeometry(QRect(80, 120, 82, 17));
        bsdf->setFont(font);
        bsdf->setChecked(false);
        bsdf->setAutoExclusive(true);
        mkillum = new QCheckBox(btdfMethod);
        mkillum->setObjectName(QString::fromUtf8("mkillum"));
        mkillum->setGeometry(QRect(130, 0, 70, 17));
        mkillumParameters = new QPlainTextEdit(btdfMethod);
        mkillumParameters->setObjectName(QString::fromUtf8("mkillumParameters"));
        mkillumParameters->setGeometry(QRect(10, 160, 171, 71));
        mkillumParameters->setFont(font1);
        mkillumParameters->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mkillumParameters->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label_6 = new QLabel(VisuCFS);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(120, 290, 81, 16));
        label_6->setFont(font);
        predefinedDay = new QGroupBox(VisuCFS);
        predefinedDay->setObjectName(QString::fromUtf8("predefinedDay"));
        predefinedDay->setGeometry(QRect(20, 190, 191, 91));
        predefinedDay->setFont(font);
        predefinedDay->setCheckable(true);
        predefinedDay->setChecked(false);
        summerSolstice = new QRadioButton(predefinedDay);
        summerSolstice->setObjectName(QString::fromUtf8("summerSolstice"));
        summerSolstice->setGeometry(QRect(30, 20, 121, 17));
        summerSolstice->setChecked(false);
        summerSolstice->setAutoExclusive(true);
        winterSolstice = new QRadioButton(predefinedDay);
        winterSolstice->setObjectName(QString::fromUtf8("winterSolstice"));
        winterSolstice->setGeometry(QRect(30, 40, 121, 17));
        winterSolstice->setAutoExclusive(true);
        springEquinox = new QRadioButton(predefinedDay);
        springEquinox->setObjectName(QString::fromUtf8("springEquinox"));
        springEquinox->setGeometry(QRect(30, 60, 121, 17));
        springEquinox->setAutoExclusive(true);
        falsecolor = new QGroupBox(VisuCFS);
        falsecolor->setObjectName(QString::fromUtf8("falsecolor"));
        falsecolor->setGeometry(QRect(850, 50, 181, 111));
        falsecolor->setCheckable(true);
        falsecolor->setChecked(false);
        lineEdit_c1 = new QLineEdit(falsecolor);
        lineEdit_c1->setObjectName(QString::fromUtf8("lineEdit_c1"));
        lineEdit_c1->setGeometry(QRect(50, 40, 41, 20));
        lineEdit_c1->setLayoutDirection(Qt::RightToLeft);
        lineEdit_c1->setReadOnly(true);
        lineEdit_c2 = new QLineEdit(falsecolor);
        lineEdit_c2->setObjectName(QString::fromUtf8("lineEdit_c2"));
        lineEdit_c2->setGeometry(QRect(50, 60, 41, 20));
        lineEdit_c2->setLayoutDirection(Qt::RightToLeft);
        lineEdit_c2->setReadOnly(true);
        lineEdit_c3 = new QLineEdit(falsecolor);
        lineEdit_c3->setObjectName(QString::fromUtf8("lineEdit_c3"));
        lineEdit_c3->setGeometry(QRect(100, 20, 41, 20));
        lineEdit_c3->setLayoutDirection(Qt::RightToLeft);
        lineEdit_c3->setReadOnly(true);
        lineEdit_c4 = new QLineEdit(falsecolor);
        lineEdit_c4->setObjectName(QString::fromUtf8("lineEdit_c4"));
        lineEdit_c4->setGeometry(QRect(100, 40, 41, 20));
        lineEdit_c4->setLayoutDirection(Qt::RightToLeft);
        lineEdit_c4->setReadOnly(true);
        lineEdit_c5 = new QLineEdit(falsecolor);
        lineEdit_c5->setObjectName(QString::fromUtf8("lineEdit_c5"));
        lineEdit_c5->setGeometry(QRect(100, 60, 41, 20));
        lineEdit_c5->setLayoutDirection(Qt::RightToLeft);
        lineEdit_c5->setReadOnly(true);
        label_15 = new QLabel(falsecolor);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 40, 46, 13));
        label_16 = new QLabel(falsecolor);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 60, 46, 13));
        label_17 = new QLabel(falsecolor);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(150, 20, 46, 13));
        label_18 = new QLabel(falsecolor);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(150, 40, 46, 13));
        label_19 = new QLabel(falsecolor);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(150, 60, 46, 13));
        label_20 = new QLabel(falsecolor);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 20, 46, 13));
        lineEdit_c0 = new QLineEdit(falsecolor);
        lineEdit_c0->setObjectName(QString::fromUtf8("lineEdit_c0"));
        lineEdit_c0->setGeometry(QRect(50, 20, 41, 20));
        lineEdit_c0->setLayoutDirection(Qt::RightToLeft);
        illuminance = new QRadioButton(falsecolor);
        illuminance->setObjectName(QString::fromUtf8("illuminance"));
        illuminance->setGeometry(QRect(10, 90, 82, 17));
        illuminance->setChecked(true);
        bluminance = new QRadioButton(falsecolor);
        bluminance->setObjectName(QString::fromUtf8("bluminance"));
        bluminance->setGeometry(QRect(100, 90, 82, 17));
        comboBox_analysisMode = new QComboBox(VisuCFS);
        comboBox_analysisMode->setObjectName(QString::fromUtf8("comboBox_analysisMode"));
        comboBox_analysisMode->setGeometry(QRect(930, 20, 101, 22));
        comboBox_model = new QComboBox(VisuCFS);
        comboBox_model->setObjectName(QString::fromUtf8("comboBox_model"));
        comboBox_model->setGeometry(QRect(20, 20, 191, 22));
        label_21 = new QLabel(VisuCFS);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(850, 20, 81, 16));
        daylightFactor = new QGroupBox(VisuCFS);
        daylightFactor->setObjectName(QString::fromUtf8("daylightFactor"));
        daylightFactor->setGeometry(QRect(849, 170, 181, 201));
        daylightFactor->setCheckable(true);
        daylightFactor->setChecked(false);
        dfLabel = new QLabel(daylightFactor);
        dfLabel->setObjectName(QString::fromUtf8("dfLabel"));
        dfLabel->setGeometry(QRect(30, 50, 121, 121));
        dfLabel->setScaledContents(true);
        label_24 = new QLabel(daylightFactor);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(10, 20, 251, 20));
        reqIlluminance = new QLineEdit(daylightFactor);
        reqIlluminance->setObjectName(QString::fromUtf8("reqIlluminance"));
        reqIlluminance->setGeometry(QRect(130, 20, 41, 20));
        reqIlluminance->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        reqIlluminance->setReadOnly(false);
        logScale = new QRadioButton(daylightFactor);
        logScale->setObjectName(QString::fromUtf8("logScale"));
        logScale->setGeometry(QRect(10, 180, 82, 17));
        logScale->setChecked(true);
        linScale = new QRadioButton(daylightFactor);
        linScale->setObjectName(QString::fromUtf8("linScale"));
        linScale->setGeometry(QRect(90, 180, 82, 17));
        glare = new QGroupBox(VisuCFS);
        glare->setObjectName(QString::fromUtf8("glare"));
        glare->setEnabled(true);
        glare->setGeometry(QRect(850, 380, 181, 51));
        glare->setCheckable(true);
        glare->setChecked(false);
        comboBox_glare = new QComboBox(glare);
        comboBox_glare->addItem(QString::fromUtf8("DGP"));
        comboBox_glare->addItem(QString());
        comboBox_glare->addItem(QString());
        comboBox_glare->addItem(QString());
        comboBox_glare->addItem(QString());
        comboBox_glare->setObjectName(QString::fromUtf8("comboBox_glare"));
        comboBox_glare->setGeometry(QRect(10, 20, 51, 22));
        glare_lineEdit = new QLineEdit(glare);
        glare_lineEdit->setObjectName(QString::fromUtf8("glare_lineEdit"));
        glare_lineEdit->setGeometry(QRect(72, 20, 101, 20));
        glare_lineEdit->setInputMask(QString::fromUtf8(""));
        glare_lineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        glare_lineEdit->setReadOnly(true);
        groupBox = new QGroupBox(VisuCFS);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 320, 191, 81));
        groupBox->setFont(font);
        filesDirectory = new QLineEdit(groupBox);
        filesDirectory->setObjectName(QString::fromUtf8("filesDirectory"));
        filesDirectory->setGeometry(QRect(10, 20, 171, 20));
        filesDirectory->setFont(font1);
        pushButton_browse = new QPushButton(groupBox);
        pushButton_browse->setObjectName(QString::fromUtf8("pushButton_browse"));
        pushButton_browse->setGeometry(QRect(10, 50, 75, 23));
        pushButton_browse->setFont(font);
        pushButton_convert = new QPushButton(groupBox);
        pushButton_convert->setObjectName(QString::fromUtf8("pushButton_convert"));
        pushButton_convert->setGeometry(QRect(100, 50, 81, 23));
        pushButton_convert->setFont(font);
        label_status = new QLabel(VisuCFS);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setGeometry(QRect(420, 630, 411, 21));
        label_status->setFont(font);
        label_status->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        renderingSettings = new QGroupBox(VisuCFS);
        renderingSettings->setObjectName(QString::fromUtf8("renderingSettings"));
        renderingSettings->setGeometry(QRect(850, 440, 181, 211));
        label_Quality = new QLabel(renderingSettings);
        label_Quality->setObjectName(QString::fromUtf8("label_Quality"));
        label_Quality->setGeometry(QRect(10, 40, 46, 16));
        horizontalSlider_Quality = new QSlider(renderingSettings);
        horizontalSlider_Quality->setObjectName(QString::fromUtf8("horizontalSlider_Quality"));
        horizontalSlider_Quality->setGeometry(QRect(80, 40, 91, 20));
        horizontalSlider_Quality->setMaximum(2);
        horizontalSlider_Quality->setPageStep(2);
        horizontalSlider_Quality->setOrientation(Qt::Horizontal);
        horizontalSlider_Quality->setTickPosition(QSlider::TicksBelow);
        label_Detail = new QLabel(renderingSettings);
        label_Detail->setObjectName(QString::fromUtf8("label_Detail"));
        label_Detail->setGeometry(QRect(10, 60, 46, 16));
        horizontalSlider_Detail = new QSlider(renderingSettings);
        horizontalSlider_Detail->setObjectName(QString::fromUtf8("horizontalSlider_Detail"));
        horizontalSlider_Detail->setGeometry(QRect(80, 60, 91, 20));
        horizontalSlider_Detail->setMaximum(2);
        horizontalSlider_Detail->setPageStep(2);
        horizontalSlider_Detail->setOrientation(Qt::Horizontal);
        horizontalSlider_Detail->setTickPosition(QSlider::TicksBelow);
        label_Variability = new QLabel(renderingSettings);
        label_Variability->setObjectName(QString::fromUtf8("label_Variability"));
        label_Variability->setGeometry(QRect(10, 80, 46, 16));
        horizontalSlider_Variability = new QSlider(renderingSettings);
        horizontalSlider_Variability->setObjectName(QString::fromUtf8("horizontalSlider_Variability"));
        horizontalSlider_Variability->setGeometry(QRect(80, 80, 91, 20));
        horizontalSlider_Variability->setMaximum(2);
        horizontalSlider_Variability->setPageStep(2);
        horizontalSlider_Variability->setOrientation(Qt::Horizontal);
        horizontalSlider_Variability->setTickPosition(QSlider::TicksBelow);
        label_Indirect = new QLabel(renderingSettings);
        label_Indirect->setObjectName(QString::fromUtf8("label_Indirect"));
        label_Indirect->setGeometry(QRect(50, 100, 46, 21));
        lineEdit_Indirect = new QLineEdit(renderingSettings);
        lineEdit_Indirect->setObjectName(QString::fromUtf8("lineEdit_Indirect"));
        lineEdit_Indirect->setGeometry(QRect(110, 100, 31, 20));
        lineEdit_Indirect->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        radianceParameters = new QPlainTextEdit(renderingSettings);
        radianceParameters->setObjectName(QString::fromUtf8("radianceParameters"));
        radianceParameters->setEnabled(true);
        radianceParameters->setGeometry(QRect(10, 130, 161, 71));
        radianceParameters->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        radianceParameters->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label_26 = new QLabel(renderingSettings);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(80, 20, 21, 16));
        label_27 = new QLabel(renderingSettings);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(150, 20, 21, 16));
        orientation = new QLineEdit(VisuCFS);
        orientation->setObjectName(QString::fromUtf8("orientation"));
        orientation->setGeometry(QRect(120, 130, 41, 20));
        orientation->setFont(font1);
        orientation->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        hour = new QLineEdit(VisuCFS);
        hour->setObjectName(QString::fromUtf8("hour"));
        hour->setGeometry(QRect(70, 290, 41, 20));
        hour->setFont(font1);
        hour->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(VisuCFS);
        QObject::connect(pushButton_map, SIGNAL(clicked()), VisuCFS, SLOT(getPosition()));
        QObject::connect(predefinedDay, SIGNAL(toggled(bool)), month, SLOT(setDisabled(bool)));
        QObject::connect(predefinedDay, SIGNAL(toggled(bool)), day, SLOT(setDisabled(bool)));
        QObject::connect(pushButton_browse, SIGNAL(clicked()), VisuCFS, SLOT(getPath()));
        QObject::connect(summerSolstice, SIGNAL(toggled(bool)), VisuCFS, SLOT(setSummerSolstice(bool)));
        QObject::connect(winterSolstice, SIGNAL(toggled(bool)), VisuCFS, SLOT(setWinterSolstice(bool)));
        QObject::connect(springEquinox, SIGNAL(toggled(bool)), VisuCFS, SLOT(setSpringEquinox(bool)));
        QObject::connect(predefinedDay, SIGNAL(toggled(bool)), VisuCFS, SLOT(setPredefinedDay(bool)));
        QObject::connect(comboBox_analysisMode, SIGNAL(currentIndexChanged(int)), VisuCFS, SLOT(setModeChanged(int)));
        QObject::connect(radianceParameters, SIGNAL(textChanged()), VisuCFS, SLOT(setRadianceParameters()));
        QObject::connect(comboBox_model, SIGNAL(currentIndexChanged(int)), VisuCFS, SLOT(setModelChanged(int)));
        QObject::connect(glare, SIGNAL(toggled(bool)), VisuCFS, SLOT(setGlareRiskCalculation(bool)));
        QObject::connect(glare, SIGNAL(toggled(bool)), daylightFactor, SLOT(setDisabled(bool)));
        QObject::connect(glare, SIGNAL(toggled(bool)), falsecolor, SLOT(setDisabled(bool)));
        QObject::connect(pushButton_convert, SIGNAL(clicked()), VisuCFS, SLOT(convertBTDF_clicked()));
        QObject::connect(pushButton_radSimul, SIGNAL(clicked()), VisuCFS, SLOT(radianceSimulation_clicked()));
        QObject::connect(comboBox_glare, SIGNAL(currentIndexChanged(int)), VisuCFS, SLOT(setGlareRiskIndex()));
        QObject::connect(falsecolor, SIGNAL(toggled(bool)), VisuCFS, SLOT(setFalseColor(bool)));
        QObject::connect(logScale, SIGNAL(toggled(bool)), VisuCFS, SLOT(setLogScale(bool)));
        QObject::connect(reqIlluminance, SIGNAL(editingFinished()), VisuCFS, SLOT(setDFRequiredIlluminance()));
        QObject::connect(daylightFactor, SIGNAL(toggled(bool)), VisuCFS, SLOT(setDaylightFactor(bool)));
        QObject::connect(horizontalSlider_Quality, SIGNAL(valueChanged(int)), VisuCFS, SLOT(setQuality()));
        QObject::connect(horizontalSlider_Detail, SIGNAL(valueChanged(int)), VisuCFS, SLOT(setDetail()));
        QObject::connect(horizontalSlider_Variability, SIGNAL(valueChanged(int)), VisuCFS, SLOT(setVariability()));
        QObject::connect(lineEdit_Indirect, SIGNAL(textChanged(QString)), VisuCFS, SLOT(setIndirect()));
        QObject::connect(bluminance, SIGNAL(toggled(bool)), VisuCFS, SLOT(setBluminance(bool)));
        QObject::connect(illuminance, SIGNAL(toggled(bool)), VisuCFS, SLOT(setIlluminance(bool)));
        QObject::connect(orientation, SIGNAL(textChanged(QString)), VisuCFS, SLOT(setSiteOrientation(QString)));
        QObject::connect(orientationDial, SIGNAL(valueChanged(int)), VisuCFS, SLOT(setSiteOrientation(int)));
        QObject::connect(hour, SIGNAL(textChanged(QString)), VisuCFS, SLOT(setHour(QString)));
        QObject::connect(mkillum, SIGNAL(toggled(bool)), mkillumParameters, SLOT(setVisible(bool)));
        QObject::connect(mkillumParameters, SIGNAL(textChanged()), VisuCFS, SLOT(setRadianceParametersMkillum()));

        QMetaObject::connectSlotsByName(VisuCFS);
    } // setupUi

    void retranslateUi(QWidget *VisuCFS)
    {
        VisuCFS->setWindowTitle(QApplication::translate("VisuCFS", "Geronimo: the CFS Daylighting Wizard, LESO-PB / EPFL", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("VisuCFS", "Month", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VisuCFS", "Day", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("VisuCFS", "Hour", 0, QApplication::UnicodeUTF8));
        month->setText(QString());
        day->setText(QString());
        pushButton_radSimul->setText(QApplication::translate("VisuCFS", "RADIANCE simulation", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("VisuCFS", "Latitude (\302\260N)", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("VisuCFS", "Longitude (\302\260E)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_9->setToolTip(QApplication::translate("VisuCFS", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">GMT+?</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        label_9->setWhatsThis(QApplication::translate("VisuCFS", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_9->setText(QApplication::translate("VisuCFS", "Meridian (hours)", 0, QApplication::UnicodeUTF8));
        latitude->setText(QString());
        longitude->setText(QString());
#ifndef QT_NO_TOOLTIP
        meridian->setToolTip(QApplication::translate("VisuCFS", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">GMT+?</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        meridian->setWhatsThis(QApplication::translate("VisuCFS", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        meridian->setText(QString());
        label_10->setText(QApplication::translate("VisuCFS", "Site orientation", 0, QApplication::UnicodeUTF8));
        pushButton_map->setText(QApplication::translate("VisuCFS", "Map", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("VisuCFS", "N", 0, QApplication::UnicodeUTF8));
        btdfMethod->setTitle(QApplication::translate("VisuCFS", "Upper glazing", 0, QApplication::UnicodeUTF8));
        prism2->setText(QApplication::translate("VisuCFS", "CFS prism2", 0, QApplication::UnicodeUTF8));
        glass->setText(QApplication::translate("VisuCFS", "glass", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_12->setToolTip(QApplication::translate("VisuCFS", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Standard glass 88% transmittance</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_12->setText(QString());
        label_13->setText(QString());
        label_14->setText(QString());
        bsdf->setText(QApplication::translate("VisuCFS", "CFS BSDF", 0, QApplication::UnicodeUTF8));
        mkillum->setText(QApplication::translate("VisuCFS", "mkillum", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mkillumParameters->setToolTip(QApplication::translate("VisuCFS", "RADIANCE mkillum parameters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        mkillumParameters->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        mkillumParameters->setDocumentTitle(QString());
        label_6->setText(QApplication::translate("VisuCFS", "h (local time)", 0, QApplication::UnicodeUTF8));
        predefinedDay->setTitle(QApplication::translate("VisuCFS", "predefined day", 0, QApplication::UnicodeUTF8));
        summerSolstice->setText(QApplication::translate("VisuCFS", "Summer Solstice", 0, QApplication::UnicodeUTF8));
        winterSolstice->setText(QApplication::translate("VisuCFS", "Winter Solstice", 0, QApplication::UnicodeUTF8));
        springEquinox->setText(QApplication::translate("VisuCFS", "Spring Equinox", 0, QApplication::UnicodeUTF8));
        falsecolor->setTitle(QApplication::translate("VisuCFS", "False Color Image", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("VisuCFS", "90%", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("VisuCFS", "70%", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("VisuCFS", "50%", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("VisuCFS", "30%", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("VisuCFS", "10%", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("VisuCFS", "100%", 0, QApplication::UnicodeUTF8));
        illuminance->setText(QApplication::translate("VisuCFS", "Illuminance", 0, QApplication::UnicodeUTF8));
        bluminance->setText(QApplication::translate("VisuCFS", "Bluminance", 0, QApplication::UnicodeUTF8));
        comboBox_analysisMode->clear();
        comboBox_analysisMode->insertItems(0, QStringList()
         << QApplication::translate("VisuCFS", "Basic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Medium", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Advanced", 0, QApplication::UnicodeUTF8)
        );
        comboBox_model->clear();
        comboBox_model->insertItems(0, QStringList()
         << QApplication::translate("VisuCFS", "Model: LESO-PB Office", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: Golay 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: Golay 4a", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: Golay 4b", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: Golay 4c", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: Golay 5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: Golay 10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("VisuCFS", "Model: base_7", 0, QApplication::UnicodeUTF8)
        );
        label_21->setText(QApplication::translate("VisuCFS", "Analysis mode", 0, QApplication::UnicodeUTF8));
        daylightFactor->setTitle(QApplication::translate("VisuCFS", "Daylight Factor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dfLabel->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        dfLabel->setText(QString());
        label_24->setText(QApplication::translate("VisuCFS", "Required Illuminance (lx)", 0, QApplication::UnicodeUTF8));
        reqIlluminance->setText(QString());
        logScale->setText(QApplication::translate("VisuCFS", "Log Scale", 0, QApplication::UnicodeUTF8));
        linScale->setText(QApplication::translate("VisuCFS", "Linear Scale", 0, QApplication::UnicodeUTF8));
        glare->setTitle(QApplication::translate("VisuCFS", "Glare Risk", 0, QApplication::UnicodeUTF8));
        comboBox_glare->setItemText(1, QApplication::translate("VisuCFS", "DGI", 0, QApplication::UnicodeUTF8));
        comboBox_glare->setItemText(2, QApplication::translate("VisuCFS", "UGR", 0, QApplication::UnicodeUTF8));
        comboBox_glare->setItemText(3, QApplication::translate("VisuCFS", "VCP", 0, QApplication::UnicodeUTF8));
        comboBox_glare->setItemText(4, QApplication::translate("VisuCFS", "CGI", 0, QApplication::UnicodeUTF8));

        groupBox->setTitle(QApplication::translate("VisuCFS", "IEA21 BTDF directory", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        filesDirectory->setWhatsThis(QApplication::translate("VisuCFS", "Essai", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        pushButton_browse->setText(QApplication::translate("VisuCFS", "Browse...", 0, QApplication::UnicodeUTF8));
        pushButton_convert->setText(QApplication::translate("VisuCFS", "Convert", 0, QApplication::UnicodeUTF8));
        label_status->setText(QString());
        renderingSettings->setTitle(QApplication::translate("VisuCFS", "Rendering settings", 0, QApplication::UnicodeUTF8));
        label_Quality->setText(QApplication::translate("VisuCFS", "Quality", 0, QApplication::UnicodeUTF8));
        label_Detail->setText(QApplication::translate("VisuCFS", "Detail", 0, QApplication::UnicodeUTF8));
        label_Variability->setText(QApplication::translate("VisuCFS", "Variability", 0, QApplication::UnicodeUTF8));
        label_Indirect->setText(QApplication::translate("VisuCFS", "Indirect", 0, QApplication::UnicodeUTF8));
        lineEdit_Indirect->setText(QString());
#ifndef QT_NO_TOOLTIP
        radianceParameters->setToolTip(QApplication::translate("VisuCFS", "RADIANCE rendering parameters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        radianceParameters->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        radianceParameters->setPlainText(QString());
        label_26->setText(QApplication::translate("VisuCFS", "Low", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("VisuCFS", "High", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VisuCFS: public Ui_VisuCFS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
