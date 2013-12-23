#include "radsimul.h"

#include "glwidget.h"

void ShellCommand::run()
{
    for (size_t cmdIndex=0; cmdIndex < commands.size(); ++cmdIndex) {
        int errorState = -1;
        emit(taskState(errorState, descriptions[cmdIndex]));
        cout << commands[cmdIndex] << endl;
        errorState = system(commands[cmdIndex].c_str());
        emit(taskState(errorState, descriptions[cmdIndex]));
        if (errorState!=0) break;
    }
    commands.clear();
    descriptions.clear();
}

RadianceSimulation::RadianceSimulation():skyChanged(true),octreeChanged(true) {

    // start with an illum=false (normal view)
    illum = false;
    dl = false;
    mkillum = false;
    prism2 = false;
    glare = false;

    // start with model 0
    model = 0;

    // maxIlluminance set to zero
    maxIlluminance = 0.f;

    // DF calculation, number of measurement points
    xSubDivMax = 20;
    ySubDivMax = 20;

}

void RadianceSimulation::run() {

    // starts the simulation with Radiance executables
    string path=".\\bin\\"; // this defines the path to the Radiance executables
    // the commands are stored in a string
    string command;

    if (skyChanged) { // generates the new sky

        if (dl) { // use the CIE overcast sky (-c) to predict daylight factor
            // generates a clear sky
            command = path + "gensky " + toString(month) + " " + toString(day) + " " + toString(hour) + " -m " + toString(meridianW) + " -c -g 0.2 -a " + toString(latitudeN) + " -o " + toString(longitudeW) + " > sky.rad";
        }
        else {
            // generates a clear sky with sun (+s)
            command = path + "gensky " + toString(month) + " " + toString(day) + " " + toString(hour) + " -m " + toString(meridianW) + " +s -g 0.2 -a " + toString(latitudeN) + " -o " + toString(longitudeW) + " > sky.rad";
        }
        shellcmd.addCommand(command,"Generation of the sky in RADIANCE format");
        //cout << command << endl;
        //system(command.c_str());

        // rotation of the scene
        command = path + "xform -rz " + toString(siteOrientation) + " ./sky.rad > skyR.rad";
        shellcmd.addCommand(command,"Rotation of the sky");
        //cout << command << endl;
        //system(command.c_str());

    }
    if (skyChanged || octreeChanged) {
        // preparation of the octree
        if (mkillum) {
            #ifdef MKILLUM
            command = path + "oconv skyR.rad skyglow.rad base_" + toString(model) + ".rad windowMKILLUM_" + toString(model) + ".rad > base_" + toString(model) + "_0.oct";
            shellcmd.addCommand(command,"Preparation of the main octree (mkillum)");
            //cout << command << endl;
            //system(command.c_str());
            command = path + "mkillum " + radianceParametersMkillum.toStdString() + " base_" + toString(model) + "_0.oct \"<\" windowMKILLUM_" + toString(model) + ".rad > illums_" + toString(model) + ".rad";
            shellcmd.addCommand(command,"Preparation of the secondary sources (mkillum)");
            //cout << command << endl;
            //system(command.c_str());
            command = path + "oconv -f skyR.rad skyglow.rad base_" + toString(model) + ".rad illums_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,"Preparation of the secondary octree (mkillum)");
            //cout << command << endl;
            //system(command.c_str());
            #else
            command = path + "oconv -f skyR.rad skyglow.rad base_" + toString(model) + ".rad windowBSDF_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,"Preparation of the octree (bsdf)");
            //cout << command << endl;
            //system(command.c_str());
            #endif
        }
        else if (prism2) { // prism2, the -f is for frozen octree (no references to the files)
            #ifdef MKILLUM
            command = path + "oconv skyR.rad skyglow.rad base_" + toString(model) + ".rad windowPRISM2_" + toString(model) + ".rad > base_" + toString(model) + "_0.oct";
            shellcmd.addCommand(command,"Preparation of the main octree (prism2)");
            //cout << command << endl;
            //system(command.c_str());
            command = path + "mkillum " + radianceParametersMkillum.toStdString() + " base_" + toString(model) + "_0.oct \"<\" windowPRISM2_" + toString(model) + ".rad > illums_" + toString(model) + ".rad";
            shellcmd.addCommand(command,"Preparation of the secondary sources (prism2)");
            //cout << command << endl;
            //system(command.c_str());
            command = path + "oconv -f skyR.rad skyglow.rad base_" + toString(model) + ".rad illums_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,"Preparation of the secondary octree (prism2)");
            //cout << command << endl;
            //system(command.c_str());
            #else
            command = path + "oconv -f skyR.rad skyglow.rad base_" + toString(model) + ".rad windowPRISM2_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,"Preparation of the octree (prism2)");
            //cout << command << endl;
            //system(command.c_str());
            #endif
        }
        else {
            #ifdef MKILLUM
            command = path + "oconv skyR.rad skyglow.rad base_" + toString(model) + ".rad windowGLASS_" + toString(model) + ".rad > base_" + toString(model) + "_0.oct";
            shellcmd.addCommand(command,"Preparation of the main octree (glass)");
            //cout << command << endl;
            //system(command.c_str());
            command = path + "mkillum " + radianceParametersMkillum.toStdString() + " base_" + toString(model) + "_0.oct \"<\" windowGLASS_" + toString(model) + ".rad > illums_" + toString(model) + ".rad";
            shellcmd.addCommand(command,"Preparation of the secondary sources (glass)");
            //cout << command << endl;
            //system(command.c_str());
            command = path + "oconv -f skyR.rad skyglow.rad base_" + toString(model) + ".rad illums_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,"Preparation of the secondary octree (glass)");
            //cout << command << endl;
            //system(command.c_str());
            #else
            command = path + "oconv -f skyR.rad skyglow.rad base_" + toString(model) + ".rad windowGLASS_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,"Preparation of the octree (glass)");
            //cout << command << endl;
            //system(command.c_str());
            #endif
        }

        // removes the old amb file
        //cout << "rm base.amb" << endl;
        //remove("base.amb");

    }

    // the sky and octree are generated
    skyChanged = false;
    octreeChanged = false;

    // creates the image
    /*
    string renderingOptions_normal = "-u -dr 0 -ab 1 -av 0 0 0"; //  -dj 0.25 -ds 0.01
    string renderingOptions_mkillum= "-u -ds .1 -ab 2 -av 0 0 0 -ad 256 -as 128 -aa .2 -ar 32 -dc 0.75 -dp 4096 -dj 0.9 -st 0.01 -ps 1 -pt 0.05 -dt 0.05 -sj 1 -lr 10 -lw 0.002";
    string renderingOptions_high   = "-u -vh 45 -vv 45 -ps 1 -pt .05 -dp 1024 -dt 0.05 -dc 0.75 -dr 1 -dj 0.9 -ds 0.01 -sj 2 -st 0.01 -aa 0.2 -ab 2 -ad 256 -as 128 -ar 32 -av 0 0 0 -lr 10 -lw 0.002";
    string renderingOptions_veryHigh   = "-u -vh 45 -vv 45 -ps 1 -pt .05 -dp 128 -dt .05 -dc 0 -dr 3 -dp 4096 -dj 1 -ds .01 -sj .5 -st .03 -aa 0.2 -ab 2 -ad 256 -as 128 -ar 32 -av 0 0 0 -lr 10 -lw .002";
    */
    string renderingOptions = radianceParameters.toStdString();

    if (!illum) {
        // visual case, with human eye filter
        command = path + "rpict -t 10 -pa " + toString((float) width/height) + " -x " + toString(width) + " -y " + toString(height)
                  + " -vtv -vp " + toString(vp[0]) + " " + toString(vp[1]) + " " + toString(vp[2])
                  + " -vd " + toString(vd[0]) + " " + toString(vd[1]) + " " + toString(vd[2])
                  + " -vu " + toString(vu[0]) + " " + toString(vu[1]) + " " + toString(vu[2])
                  + " " + renderingOptions + " base_" + toString(model) + "_1.oct > base_" + toString(model) + ".hdr";
        shellcmd.addCommand(command,"Ray-tracing a radiance image (rpict)");
        //cout << command << endl;
        //system(command.c_str());

        if (!glare) {
            // sets the exposure to human eye
            command = path + "pcond -h base_" + toString(model) + ".hdr > base_" + toString(model) + "_H.hdr";
            shellcmd.addCommand(command,"Conditioning the image for human eye");
            //cout << command << endl;
            //system(command.c_str());
            // transforms the image in bmp
            command = path + "ra_bmp base_" + toString(model) + "_H.hdr base_" + toString(model) + ".bmp";
            shellcmd.addCommand(command,"Converting the image in BMP format");
            //cout << command << endl;
            //system(command.c_str());
        }
    }
    else if (!glare) {
        // illuminance case, create an illuminance image, -dv
        command = path + "rpict -i -t 10 -pa " + toString((float) width/height) + " -x " + toString(width) + " -y " + toString(height)
                  + " -vtv -vp " + toString(vp[0]) + " " + toString(vp[1]) + " " + toString(vp[2])
                  + " -vd " + toString(vd[0]) + " " + toString(vd[1]) + " " + toString(vd[2])
                  + " -vu " + toString(vu[0]) + " " + toString(vu[1]) + " " + toString(vu[2])
                  + " " + renderingOptions + " base_" + toString(model) + "_1.oct > base_" + toString(model) + "_I.hdr";
        shellcmd.addCommand(command,"Ray-tracing an illuminance image");
        //cout << command << endl;
        //system(command.c_str());

        // gets the maximum and minimum points
        command = path + "pextrem -o base_" + toString(model) + "_I.hdr > extrema.dat";
        shellcmd.addCommand(command,"Getting the extrema in the illuminance image");
        //cout << command << endl;
        //system(command.c_str());

        // start the simulations as what follows depends on it
        shellcmd.start();
        shellcmd.wait();

        // reads the maximum
        vector<float> extrema;
        load("extrema.dat", extrema);
        for (unsigned int i=0; i<extrema.size()/5; ++i) {
            cout << "red: " << extrema[i*5+2] << "\tgreen: " << extrema[i*5+3] << "\tblue: " << extrema[i*5+4] << endl;
        }
        maxIlluminance = 179.f*(extrema[7]*.27f+extrema[8]*.67f+extrema[9]*.06f);
        cout << "maximum illuminance: " << maxIlluminance << " lx" << endl;

        // writes the scale of the illuminance contours, pc0.cal
        fstream output("pc0.cal", ios::out | ios::binary | ios::trunc);
        output << "PI : 3.14159265358979323846 ;" << endl;
        output << "scale : " << 179.f*(extrema[7]*.27f+extrema[8]*.67f+extrema[9]*.06f) << " ;" << endl;
        output << "mult : " << 179.f << " ;" << endl;
        output << "ndivs : " << 5 << " ;\n" << endl;

        output << "or(a,b) : if(a,a,b);"  << endl;
        output << "EPS : 1e-7;"  << endl;
        output << "neq(a,b) : if(a-b-EPS,1,b-a-EPS);"  << endl;
        output << "btwn(a,x,b) : if(a-x,-1,b-x);"  << endl;
        output << "clip(x) : if(x-1,1,if(x,x,0));"  << endl;
        output << "frac(x) : x - floor(x);" << endl;
        output << "boundary(a,b) : neq(floor(ndivs*a+.5),floor(ndivs*b+.5));\n" << endl;

        output << "old_red(x) = 1.6*x - .6;" << endl;
        output << "old_grn(x) = if(x-.375, 1.6-1.6*x, 8/3*x);" << endl;
        output << "old_blu(x) = 1 - 8/3*x;\n" << endl;

        output << "interp_arr2(i,x,f):(i+1-x)*f(i)+(x-i)*f(i+1);" << endl;
        output << "interp_arr(x,f):if(x-1,if(f(0)-x,interp_arr2(floor(x),x,f),f(f(0))),f(1));" << endl;
        output << "def_redp(i):select(i,0.18848,0.05468174," << endl;
        output << "0.00103547,8.311144e-08,7.449763e-06,0.0004390987,0.001367254," << endl;
        output << "0.003076,0.01376382,0.06170773,0.1739422,0.2881156,0.3299725," << endl;
        output << "0.3552663,0.372552,0.3921184,0.4363976,0.6102754,0.7757267," << endl;
        output << "0.9087369,1,1,0.9863);" << endl;
        output << "def_red(x):interp_arr(x/0.0454545+1,def_redp);" << endl;
        output << "def_grnp(i):select(i,0.0009766,2.35501e-05," << endl;
        output << "0.0008966244,0.0264977,0.1256843,0.2865799,0.4247083,0.4739468," << endl;
        output << "0.4402732,0.3671876,0.2629843,0.1725325,0.1206819,0.07316644," << endl;
        output << "0.03761026,0.01612362,0.004773749,6.830967e-06,0.00803605," << endl;
        output << "0.1008085,0.3106831,0.6447838,0.9707);" << endl;
        output << "def_grn(x):interp_arr(x/0.0454545+1,def_grnp);" << endl;
        output << "def_blup(i):select(i,0.2666,0.3638662,0.4770437," << endl;
        output << "0.5131397,0.5363797,0.5193677,0.4085123,0.1702815,0.05314236," << endl;
        output << "0.05194055,0.08564082,0.09881395,0.08324373,0.06072902," << endl;
        output << "0.0391076,0.02315354,0.01284458,0.005184709,0.001691774," << endl;
        output << "2.432735e-05,1.212949e-05,0.006659406,0.02539);" << endl;
        output << "def_blu(x):interp_arr(x/0.0454545+1,def_blup);\n" << endl;

        output << "isconta = if(btwn(0,v,1),or(boundary(vleft,vright),boundary(vabove,vbelow)),-1);"  << endl;
        output << "iscontb = if(btwn(0,v,1),btwn(.4,frac(ndivs*v),.6),-1);\n" << endl;

        output << "ra = 0;" << endl;
        output << "ga = 0;" << endl;
        output << "ba = 0;\n" << endl;

        output << "in = 1;\n" << endl;

        output << "ro = if(in,clip(def_red(v)),ra);" << endl;
        output << "go = if(in,clip(def_grn(v)),ga);" << endl;
        output << "bo = if(in,clip(def_blu(v)),ba);" << endl;
        output.close();

        // writes the second part of the file (pc1.cal)
        output.open("pc1.cal", ios::out | ios::binary | ios::trunc);
        output << "norm : mult/scale/le(1);\n" << endl;

        output << "v = map(li(1)*norm);\n" << endl;

        output << "vleft = map(li(1,-1,0)*norm);" << endl;
        output << "vright = map(li(1,1,0)*norm);" << endl;
        output << "vabove = map(li(1,0,1)*norm);" << endl;
        output << "vbelow = map(li(1,0,-1)*norm);\n" << endl;

        output << "map(x) = x;\n" << endl;

        output << "ra = ri(nfiles);" << endl;
        output << "ga = gi(nfiles);" << endl;
        output << "ba = bi(nfiles);" << endl;
        output.close();

        // uses the falsecolor to produce illuminance contours, -f pc0.cal -e in=isconta
        command = path + "pcomb -f pc0.cal -f pc1.cal base_" + toString(model) + "_I.hdr > base_" + toString(model) + "_IF.hdr";
        shellcmd.addCommand(command,"Preparation of the falsecolor image");
        //cout << command << endl;
        //system(command.c_str());

        // transforms the final image in bmp
        command = path + "ra_bmp base_" + toString(model) + "_IF.hdr base_" + toString(model) + ".bmp";
        shellcmd.addCommand(command,"Conversion of the image to BMP format");
        //cout << command << endl;
        //system(command.c_str());
    }

    // takes care of the daylight factor calculation
    if (!glare && dl) {

        // create measurment points in a file
        string meshInputFilename = "mesh_" + toString(model) + ".inp";
        fstream output(meshInputFilename.c_str(), ios::out | ios::binary | ios::trunc);
        // roof reference
        output << "0 0 " << zMax+0.8 << " 0 0 1" << endl;
        // measurement points inside the room
        for (unsigned int ySubDiv=1; ySubDiv < (ySubDivMax+1); ++ySubDiv) {
            for (unsigned int xSubDiv=1; xSubDiv < (xSubDivMax+1); ++xSubDiv) {
                output << xMin+xSubDiv*((xMax-xMin)/(xSubDivMax+1)) << " "
                       << yMin+ySubDiv*((yMax-yMin)/(ySubDivMax+1)) << " "
                       /// TODO: si le sol n'est pas plat, corriger la normale ainsi que la valeur de Z
                       << zMin+0.8 << " "
                       << "0 0 1" << endl;
            }
        }
        output.close();

        // rtrace the mesh points
        command = path + "rtrace -w -I -h " + renderingOptions + " base_" + toString(model) + "_1.oct < mesh_" + toString(model) + ".inp | " + path + "rcalc -e \"$1=179.*(0.265*$1+0.67*$2+0.065*$3)\" > mesh_" + toString(model) + ".out";
        shellcmd.addCommand(command,"Ray-tracing on mesh points (rtrace)");
        //cout << command << endl;
        //system(command.c_str());

        // start the simulation as the image depends on it
        shellcmd.start();
        shellcmd.wait();

        // reads the results and put them in the DF_values vector
        DF_values.clear();
        load("mesh_" + toString(model) + ".out", DF_values);

        // use the mesh points to create a BMP image
        createDFimage();

    }

    if (glare) {
        // start off the glare calculation and binds the image
        command = path + "evalglare -c base_" + toString(model) + "_G.hdr base_" + toString(model) + ".hdr > glare_" + toString(model) + ".out";
        shellcmd.addCommand(command,"Glare calculation on the image");
        //cout << command << endl;
        //system(command.c_str());

        // starts the simulation as the glare index depends on it
        shellcmd.start();
        shellcmd.wait();

        // open the output file for reading and extract the indices -> put them in the correct textEdit
        string glareInputFilename = "glare_" + toString(model) + ".out";
        fstream glareInput(glareInputFilename.c_str(), ios::in | ios::binary);
        string indices,buffer;
        // gets a string that contains the indices
        glareInput >> indices;
        // decomposes the indices, separated using ","
        size_t pos1 = 0, pos2 = indices.find(",",pos1);
        while (pos2 != string::npos) {
            glareInput >> buffer;
            glareIndices[indices.substr(pos1,pos2-pos1)] = to<float>(buffer);
            pos1 = pos2+1;
            pos2 = indices.find(",",pos1);
        }
        glareInput >> buffer;
        glareIndices[indices.substr(pos1,indices.find(":",pos1)-pos1)] = to<float>(buffer);

        // affichage
        for (map<string,float>::iterator it=glareIndices.begin();it!=glareIndices.end();++it) {
            cout << it->first << ": " << it->second << endl;
        }

        // sets the exposure to human vision
        command = path + "pcond -h base_" + toString(model) + "_G.hdr > base_" + toString(model) + "_GH.hdr";
        shellcmd.addCommand(command,"Conditionning the image for human eye");
        //cout << command << endl;
        //system(command.c_str());

        // transforms the image in bmp
        command = path + "ra_bmp base_" + toString(model) + "_GH.hdr base_" + toString(model) + ".bmp";
        shellcmd.addCommand(command,"Conversion of the image in BMP format");
        //cout << command << endl;
        //system(command.c_str());

    }

    // starts all the (remaining) commands
    shellcmd.start();
    shellcmd.wait();

}

void RadianceSimulation::convertBTDF(QString directoryName) {

    // generates the BTDF file .cal & .xml
    if (!directoryName.isEmpty()) {
        string command = "btdf2radiance ./" + directoryName.remove(0,directoryName.lastIndexOf("/")+1).toStdString() + "/*.txt";
        shellcmd.addCommand(command,"Conversion of BTDF in RADIANCE format");
        shellcmd.start();
        //cout << command << endl;
        //errorState = system(command.c_str());
    }

    // sets the simulation tag
    octreeChanged = true;

}

void RadianceSimulation::createDFimage() {

    if (!DF_values.empty()) {

        // checks the size of the vector
        if (DF_values.size() != xSubDivMax*ySubDivMax+1) throw(string("Invalid DF_values vector size."));

        // creates the image and binds it
        Bitmap_image image;
        image.width=xSubDivMax;
        image.height=ySubDivMax;
        image.bitsPerPixel=8;
        image.paletteColors=pow(2,image.bitsPerPixel);
        image.palette = new unsigned char[image.paletteColors*4];
        for (unsigned int index=0;index<image.paletteColors;++index) {
            // put the colors in the order, blue green red empty, same color for gray level
            image.palette[index*4]   = (unsigned char) index;
            image.palette[index*4+1] = (unsigned char) index;
            image.palette[index*4+2] = (unsigned char) index;
            image.palette[index*4+3] = (unsigned char) 0;
        }

        // read in the output values and write the data
        image.data = new unsigned char[xSubDivMax*ySubDivMax];
        float externalIlluminance = *DF_values.begin();
        float maxValue = *max_element(DF_values.begin()+1,DF_values.end());
        float minValue = *min_element(DF_values.begin()+1,DF_values.end());

        // for the log scale, computes the minimum threshold (below is 0 grey scale)
        // the minValue depends on the daylight autonomy
        float FLJmin = 0.01f + (0.01f/200.f)*(max(DF_reqIllum,200.f)-200.f); // this equation comes from Bernard Paule's thesis
        float minThresholdValue = FLJmin*externalIlluminance;
        for (unsigned int index=0;index<xSubDivMax*ySubDivMax;++index) {
            float factorGray;
            if (!logScale) {
                // linear scale
                factorGray = max((DF_values[index+1]-minThresholdValue),0.f)/(maxValue-minThresholdValue);
            }
            else {
                // log scale
                factorGray = max(log10(DF_values[index+1]/minThresholdValue),0.)/log10(maxValue/minThresholdValue);
            }
            image.data[index]=(unsigned char)((unsigned int) (static_cast<float>(image.paletteColors-1)*factorGray));
        }
        //string meshBMPFilename = "mesh_" + toString(model) + ".bmp";
        //write_bmp(meshBMPFilename.c_str(), &image);
        // emits the maxIlluminance on the grid value & the corresponding DF
        DF_msg = "max Illuminance/DF: " +     QString::number(maxValue,'f',0) + " lx/" + QString::number(100.f*maxValue/externalIlluminance,'f',1) + " %"
                      + "\nmin Illuminance/DF: " + QString::number(minValue,'f',0) + " lx/" + QString::number(100.f*minValue/externalIlluminance,'f',1) + " %"
                      + "\nthreshold: " +          QString::number(minThresholdValue,'f',0) + " lx/" + QString::number(100.f*FLJmin,'f',1) + " %";
        if (logScale) DF_msg += "\nlog scale";
        else          DF_msg += "\nlinear scale";

        // creation du Pixmap sur la base de l'image n/b
        DF_image = QPixmap::fromImage(QImage(image.data,image.width,image.height,QImage::Format_Indexed8).mirrored(true,false));

        // cleans the memory
        delete[] image.palette;
        delete[] image.data;
    }
    else DF_image = QPixmap();

}

void RadianceSimulation::write_bmp(const char *file, Bitmap_image *b)
{

    cout << "...writing file..." << endl;

    Bmpfile_magic m;
    Bmpfile_header h;

    FILE *fp;

  /* open the file */
  if ((fp = fopen(file,"wb")) == NULL)
  {
    printf("Error opening file %s.\n",file);
    return;
  }

    // writes the magic number
    m.magic[0]='B';
    m.magic[1]='M';
    fwrite(&m,sizeof(unsigned char),2,fp);

    // writes the header of the file
    uint32_t widthSize = (static_cast<uint32_t>(ceil(static_cast<double>(b->width*b->bitsPerPixel)/32.)))*32/8; // in bytes multiple of 32 bits = 4 bytes
    uint32_t rawSize=b->height*widthSize; // (width + padding per line)*height of data size
    h.filesz=54+rawSize+((b->bitsPerPixel<=8) ? pow(2,b->bitsPerPixel) : 0)*4; // 4 bytes per palette color and 54 bytes of header
    h.creator1=0;
    h.creator2=0;
    h.bmp_offset=54+((b->bitsPerPixel<=8) ? pow(2,b->bitsPerPixel) : 0)*4; // offset to the data
    fwrite(&h,sizeof(unsigned char),12,fp);
    cout << "File size: " << h.filesz << " bytes\tcreator1: " << h.creator1 << "\tcreator2: " << h.creator2 << endl;
    cout << "Offset: " << h.bmp_offset << " bytes" << endl;

    // writes the number of bytes in the header from this point until the bitmap data
    uint32_t numBytesFromThisPoint=40;
    fwrite(&numBytesFromThisPoint,sizeof(unsigned char),4,fp);

    // reads the width and height
    fwrite(&b->width, sizeof(unsigned char), 4, fp);
    fwrite(&b->height,sizeof(unsigned char), 4, fp);
    uint16_t colorPlanes=1;
    fwrite(&colorPlanes,sizeof(unsigned char), 2, fp);
    fwrite(&b->bitsPerPixel,sizeof(unsigned char), 2, fp);
    cout << "width: " << b->width << " pixels\theight: " << b->height << " pixels\tbitsPerPixel: " << b->bitsPerPixel << endl;

    // writes the compression, no compression
    uint32_t compr=0;
    fwrite(&compr,sizeof(unsigned char),4,fp);
    cout << ((compr == 0) ? "No compression used." : "Compression: ") << endl;

    // size of the data & resolution
    uint32_t hRes=2835;
    uint32_t vRes=2835;
    fwrite(&rawSize,sizeof(unsigned char),4,fp);
    fwrite(&hRes,sizeof(unsigned char),4,fp);
    fwrite(&vRes,sizeof(unsigned char),4,fp);
    cout << "Raw BMP data: " << rawSize << " bytes" << endl;
    cout << "Horizontal resolution: " << hRes << " pixels/m\tVertical resolution: " << vRes << " pixels/m" << endl;

    // palette and important colors
    uint32_t importantColors=0;
    fwrite(&b->paletteColors,sizeof(unsigned char),4,fp);
    fwrite(&importantColors,sizeof(unsigned char),4,fp);
    cout << "Palette colors: " << b->paletteColors << endl;
    cout << "Important colors: " << importantColors << endl;

    // writes the palette that we want to use (gray scale palette)
    for (unsigned int colorIndex=0;colorIndex<b->paletteColors;++colorIndex) {
        fwrite(&(b->palette[colorIndex*4]),sizeof(unsigned char),4,fp);
    }
    cout << "Actual position within the file (at the end of header): " << ftell(fp) << endl;

    // writes the actual raw data, careful here when writing, padding data multiple of 4 bytes
    for (unsigned int index=0;index<b->height;++index) {
        fwrite(&(b->data[index*widthSize]),sizeof(unsigned char),widthSize,fp);
    }

  fclose(fp);

    return;

}
