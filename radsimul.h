#include <QThread>
#include <QPixmap>

#include <vector>
#include <algorithm>
#include <cstring>
#include <map>
#include <iostream>

using namespace std;

struct Bmpfile_magic {
    unsigned char magic[2];
};

struct Bmpfile_header {
    uint32_t filesz;
    uint16_t creator1;
    uint16_t creator2;
    uint32_t bmp_offset;
};

struct Bitmap_image {
    uint32_t width;
    uint32_t height;
    uint16_t bitsPerPixel;
    uint32_t paletteColors;
    unsigned char *palette;
    unsigned char *data;
};

class ShellCommand : public QThread
{

    Q_OBJECT

private:

    vector<string> commands;
    vector<QString> descriptions;

public:

    ShellCommand() {}

    void addCommand(string command, QString description) { this->commands.push_back(command); this->descriptions.push_back(description); }

    void run();

signals:

    void taskState(int errorState, QString description);

};

class RadianceSimulation : public QThread {

    Q_OBJECT

private:
    // about the site
    double latitudeN, longitudeW, meridianW;
    int month, day;
    float hour;
    int siteOrientation;

    // about the calculation parameters
    bool skyChanged, octreeChanged;

    // the model itself
    int model;

    // illuminance & daylight calculation
    bool falsecolor, illuminance, bluminance, dl;
    float maxValue; // max value for the color scale

    // prism2 approximation of the BTDF (with Radiance)
    bool prism2;
    // mkillum calculation (with Radiance)
    bool mkillum;
    // BSDF approximation for the simulation
    bool bsdf;

    // string containing the radiance parameters
    QString radianceParameters,radianceParametersMkillum;

    // required illuminance for DF display
    unsigned int xSubDivMax, ySubDivMax;
    float DF_reqIllum;
    bool logScale;
    QString DF_msg;
    QPixmap DF_image;
    vector<float> DF_values;

    // for glare calculation
    bool glare;

    // size of the rendering, view point, view direction and view up, and finally scene dimensions
    int width, height;
    double vp[3], vd[3], vu[3];
    double xMin, xMax, yMin, yMax, zMin, zMax;

public:

    RadianceSimulation();
    void run();

    ShellCommand shellcmd;
    // creates the BMP image for DF
    void convertBTDF(QString directoryName);
    // gets for the daylight factor calculation
    void createDFimage();
    QPixmap getDFimage() { return DF_image; }
    QString getDFmsg() { return DF_msg; }
    static void write_bmp(const char *file, Bitmap_image *b);
    // public member to be accessed
    map<string,float> glareIndices;
    void setModel(int value) { this->model = value; octreeChanged=true; }
    // gets the maxIlluminance
    float getMaxValue() { return maxValue; }

    // this sets the glare calculation and empties the glareIndices
    void setGlare(bool value) { this->glare = value; glareIndices.clear(); }
    void setRadianceParameters(QString param) { this->radianceParameters = param; }
    void setRadianceParametersMkillum(QString param) { this->radianceParametersMkillum = param; octreeChanged=true; }
    // methods to set the elements from the view
    void setImageSize(int width, int height) { this->width=width; this->height=height; }
    void setView(vector<double> vp, vector<double> vd, vector<double> vu) { for (unsigned int i=0;i<3;++i) { this->vp[i]=vp[i]; this->vd[i]=vd[i]; this->vu[i]=vu[i]; } }
    void setBoundingBox(vector<double> xyzMinMax) {
        this->xMin = xyzMinMax[0]; this->xMax = xyzMinMax[1]; this->yMin = xyzMinMax[2]; this->yMax = xyzMinMax[3]; this->zMin = xyzMinMax[4]; this->zMax = xyzMinMax[5];
    }

public slots:
    void setLatitude(QString latitude) { this->latitudeN = latitude.toDouble(); skyChanged=true; }
    void setLongitude(QString longitude) { this->longitudeW = -longitude.toDouble(); skyChanged=true; }
    void setMeridian(QString meridian) { this->meridianW = -15.*meridian.toDouble(); skyChanged=true; }
    void setMonth(QString month) { this->month = month.toInt(); skyChanged=true; }
    void setDay(QString day) { this->day = day.toInt(); skyChanged=true; }
    void setHour(float hour) { this->hour = hour; skyChanged=true; }
    void setSiteOrientation(int siteOrientation) { this->siteOrientation = siteOrientation; skyChanged=true; }
    void setFalseColor(bool falsecolor) { this->falsecolor = falsecolor; maxValue = 0.f; }
    void setIlluminance(bool illuminance) { this->illuminance = illuminance; maxValue = 0.f; }
    void setBluminance(bool bluminance) { this->bluminance = bluminance; maxValue = 0.f; }
    void setDaylight(bool dl) { this->dl = dl; DF_values.clear(); skyChanged=true; }
    // sets the approximation of the BTDF
    void setPrism2(bool prism2) { this->prism2 = prism2; octreeChanged=true; }
    void setMkillum(bool mkillum) { this->mkillum = mkillum; octreeChanged=true; }
    void setBSDF(bool bsdf) { this->bsdf = bsdf; octreeChanged=true; }
    // for the DF calculation
    void setDFReqIllum(QString value) { this->DF_reqIllum = value.toFloat(); createDFimage(); }
    void setLogScale(bool value) { this->logScale = value; createDFimage(); }

};
