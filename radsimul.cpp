#include <QProcess>
#include <QProcessEnvironment>
#include <QDir>

#include "radsimul.h"

#include "glwidget.h"

ShellCommand::ShellCommand():commandsWindow(new QDialog()) {

  if (commandsWindow->objectName().isEmpty())
      commandsWindow->setObjectName(QStringLiteral("Commands"));
  commandsWindow->resize(640, 300);
  plainTextEdit = new QPlainTextEdit(commandsWindow);
  plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
  plainTextEdit->setGeometry(QRect(10, 10, 621, 281));
  plainTextEdit->setReadOnly(true);
  plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  commandsWindow->setWindowTitle("Geronimo: commands, LESO-PB/EPFL");
  commandsWindow->show();
  connect(this, SIGNAL(commandLine(QString)), this, SLOT(showCommandLine(QString)));

}

void ShellCommand::run()
{
    for (size_t cmdIndex=0; cmdIndex < commands.size(); ++cmdIndex) {

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
#ifdef WIN32
        env.insert("PATH", env.value("PATH") + ";" + QDir::toNativeSeparators(QDir::currentPath()+"/bin"));
#else
        env.insert("PATH", env.value("PATH") + ":" + QDir::toNativeSeparators(QDir::currentPath()+"/bin"));
#endif
        QProcess process;
        process.setProcessEnvironment(env);
        std::cout << "Running command [" << cmdIndex << "]: " << commands[cmdIndex] << std::endl;
#ifdef WIN32
        QString cmd = "cmd /C " + QString::fromStdString(commands[cmdIndex]);
#else
        QString cmd = "sh -c "  + QString::fromStdString(commands[cmdIndex]);
#endif
        emit(commandLine(QString::fromStdString(commands[cmdIndex])));
        process.startCommand(cmd); //system(commands[cmdIndex].c_str());
        if (process.waitForStarted(-1)) {
            emit(taskState(-1, descriptions[cmdIndex]));
            QString msg;
            do {
                msg = QString::fromLatin1(process.readAllStandardOutput());
                if (!msg.isEmpty()) emit(commandLine(msg.remove(QChar::LineFeed).remove(QChar::CarriageReturn)));
                msg = QString::fromLatin1(process.readAllStandardError());
                if (!msg.isEmpty()) emit(commandLine(msg.remove(QChar::LineFeed).remove(QChar::CarriageReturn)));
            }
            while(!process.waitForFinished(1000));
            emit(taskState(process.exitCode(), descriptions[cmdIndex]));
            if (process.exitCode()!=0) {
                emit(commandLine(QString::fromLatin1(process.readAllStandardOutput())));
                emit(commandLine(QString::fromLatin1(process.readAllStandardError())));
                break;
            }
        }
        else {
            emit(taskState(process.exitCode(), descriptions[cmdIndex]));
            emit(commandLine(QString::fromLatin1(process.readAllStandardOutput())));
            emit(commandLine(QString::fromLatin1(process.readAllStandardError())));
            break;
        }
    }
    commands.clear();
    descriptions.clear();
}

RadianceSimulation::RadianceSimulation():skyChanged(true),octreeChanged(true) {

    // start with an illum=false (normal view)
    falsecolor = false;
    luminance = false;
    bluminance = false;
    dl = false;
    prism2 = false;
    mkillum = false;
    bsdf = false;
    glare = false;

    // start with model 0
    model = 0;

    // maxIlluminance set to zero
    maxValue = 0.f;

    // DF calculation, number of measurement points
    xSubDivMax = 20;
    ySubDivMax = 20;

    // scale by default
    scale = 1.f;

}

void RadianceSimulation::run() {

    // starts the simulation with Radiance executables
    // the commands are stored in a string
    std::string command;

    if (skyChanged) { // generates the new sky

        if (dl) { // use the CIE overcast sky (-c) to predict daylight factor
            // generates a clear sky
            command = "gensky " + toString(month) + " " + toString(day) + " " + toString(hour) + " -m " + toString(meridianW) + " -c -g 0.2 -a " + toString(latitudeN) + " -o " + toString(longitudeW) + " > sky.rad";
        }
        else {
            // generates a clear sky with sun (+s)
            command = "gensky " + toString(month) + " " + toString(day) + " " + toString(hour) + " -m " + toString(meridianW) + " +s -g 0.2 -a " + toString(latitudeN) + " -o " + toString(longitudeW) + " > sky.rad";
        }
        shellcmd.addCommand(command,"Generation of the sky in RADIANCE format");
        //cout << command << endl;
        //system(command.c_str());

        // rotation of the scene
        command = "xform -rz " + toString(siteOrientation) + " ./sky.rad > skyR.rad";
        shellcmd.addCommand(command,"Rotation of the sky");
        //cout << command << endl;
        //system(command.c_str());

    }
    if (skyChanged || octreeChanged) {

        command = "xform -s " + toString(scale) + " ./base_" + toString(model) + ".rad > base.rad";
        shellcmd.addCommand(command,"Scaling of the model");
        //cout << command << endl;
        //system(command.c_str());

        command = "xform -s " + toString(scale) + " ./window_" + toString(model) + ".rad > window.rad";
        shellcmd.addCommand(command,"Scaling of the CFS");
        //cout << command << endl;
        //system(command.c_str());

        // preparation of the octree, including the materials definition
        std::fstream output("material.rad", std::ios::out | std::ios::binary | std::ios::trunc);
        if (prism2) {
            output << "void prism2 cfs\n"
                   << "9 coef1 dx1 dy1 dz1 coef2 dx2 dy2 dz2 \'" << prism2_file.toLatin1().toStdString() << "\'\n"
                   << "0\n"
                   << "3 0 0 1" << std::endl;
        }
        else if (bsdf) {
            output << "void BSDF cfs\n"
                   << "6 0 '" << bsdf_file.toLatin1().toStdString() << "\' 0 0 1 .\n"
                   << "0\n"
                   << "0" << std::endl;
            /*
            # BSDF material definition (Radiance 4.1a)
            #	mod BSDF id
            #	6+ thick BSDFfile ux uy uz funcfile transform
            #	0
            #	0|3|6|9
            #		rfdif gfdif bfdif
            #		rbdif gbdif bbdif
            #		rtdif gtdif btdif

            # about the function file Greg said:
            # You don't really need a function file -- it's only if your up vector changes over the surface or something that you'd even need one.  I use '.' in place of the function file and constant values for the thickness (usually "0") and up vector (usually "0 0 1").  The coordinate mapping is part of the BSDF file.
            */
        }
        else {
            output << "void glass cfs\n"
                   << "0\n"
                   << "0\n"
                   << "3 " << glass_transmissivity.toStdString() << " " << glass_transmissivity.toStdString() << " " << glass_transmissivity.toStdString() << std::endl;
        }
        output.close();

        // test if mkillum should be used or not
        if (mkillum) {
            command = "oconv skyR.rad skyglow.rad base.rad material.rad window.rad > base_" + toString(model) + "_0.oct";
            shellcmd.addCommand(command,QString::fromStdString("Preparation of the main octree (oconv)"));
            //cout << command << endl;
            //system(command.c_str());
            command = "mkillum " + radianceParametersMkillum.toStdString() + " base_" + toString(model) + "_0.oct \"<\" material.rad window.rad > illums_" + toString(model) + ".rad";
            shellcmd.addCommand(command,QString::fromStdString("Preparation of the secondary sources (mkillum)"));
            //cout << command << endl;
            //system(command.c_str());
            command = "oconv -f skyR.rad skyglow.rad base.rad illums_" + toString(model) + ".rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,QString::fromStdString("Preparation of the secondary octree (oconv)"));
            //cout << command << endl;
            //system(command.c_str());
        }
        else {
            command = "oconv -f skyR.rad skyglow.rad base.rad material.rad window.rad > base_" + toString(model) + "_1.oct";
            shellcmd.addCommand(command,QString::fromStdString("Preparation of the octree (oconv)"));
            //cout << command << endl;
            //system(command.c_str());
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
    std::string renderingOptions_normal = "-u -dr 0 -ab 1 -av 0 0 0"; //  -dj 0.25 -ds 0.01
    std::string renderingOptions_mkillum= "-u -ds .1 -ab 2 -av 0 0 0 -ad 256 -as 128 -aa .2 -ar 32 -dc 0.75 -dp 4096 -dj 0.9 -st 0.01 -ps 1 -pt 0.05 -dt 0.05 -sj 1 -lr 10 -lw 0.002";
    std::string renderingOptions_high   = "-u -vh 45 -vv 45 -ps 1 -pt .05 -dp 1024 -dt 0.05 -dc 0.75 -dr 1 -dj 0.9 -ds 0.01 -sj 2 -st 0.01 -aa 0.2 -ab 2 -ad 256 -as 128 -ar 32 -av 0 0 0 -lr 10 -lw 0.002";
    std::string renderingOptions_veryHigh   = "-u -vh 45 -vv 45 -ps 1 -pt .05 -dp 128 -dt .05 -dc 0 -dr 3 -dp 4096 -dj 1 -ds .01 -sj .5 -st .03 -aa 0.2 -ab 2 -ad 256 -as 128 -ar 32 -av 0 0 0 -lr 10 -lw .002";
    */
    std::string renderingOptions = radianceParameters.toStdString();

    // creates the image in .hdr format
    command = "rpict -t 10 -pa " + toString(width/height) + " -x " + toString(width) + " -y " + toString(height)
              + " -vtv -vp " + toString(vp[0]) + " " + toString(vp[1]) + " " + toString(vp[2])
              + " -vd " + toString(vd[0]) + " " + toString(vd[1]) + " " + toString(vd[2])
              + " -vu " + toString(vu[0]) + " " + toString(vu[1]) + " " + toString(vu[2])
              + " " + renderingOptions + " base_" + toString(model) + "_1.oct > base_" + toString(model) + ".hdr";
    shellcmd.addCommand(command,"Ray-tracing a radiance image (rpict)");
    //cout << command << endl;
    //system(command.c_str());

    if (!falsecolor) {
        // visual case, with human eye filter
        if (!glare) {
            // sets the exposure to human eye
            command = "pcond -h base_" + toString(model) + ".hdr > base_" + toString(model) + "_H.hdr";
            shellcmd.addCommand(command,"Conditioning the image for human eye");
            //cout << command << endl;
            //system(command.c_str());
            // transforms the image in bmp
            command = "ra_bmp base_" + toString(model) + "_H.hdr base_" + toString(model) + ".bmp";
            shellcmd.addCommand(command,"Converting the image in BMP format");
            //cout << command << endl;
            //system(command.c_str());
        }
    }
    else if (!glare) {
        // gets the maximum and minimum points
        command = "pextrem -o base_" + toString(model) + ".hdr > extrema.dat";
        shellcmd.addCommand(command,"Getting the extrema in the radiance image");
        //cout << command << endl;
        //system(command.c_str());

        // start the simulations as what follows depends on it
        shellcmd.start();
        shellcmd.wait();

        // reads the maximum
        std::vector<float> extrema;
        load("extrema.dat", extrema);
        for (unsigned int i=0; i<extrema.size()/5; ++i) {
            std::cout << "red: " << extrema[i*5+2] << "\tgreen: " << extrema[i*5+3] << "\tblue: " << extrema[i*5+4] << std::endl;
        }

        // selection of the weights (RGB) for luminance or bluminance
        std::vector<float> weights;
        if (luminance) {
            weights.push_back(0.27f);
            weights.push_back(0.67f);
            weights.push_back(0.06f);
            if (maxValue <= 0.f)
                maxValue = 179.f*(extrema[7]*weights[0]+extrema[8]*weights[1]+extrema[9]*weights[2]);
            std::cout << "maximum luminance: " << maxValue << " cd/m2" << std::endl;
        }
        else { // bluminance
            weights.push_back(-0.0346f);
            weights.push_back(0.232f);
            weights.push_back(0.558f);
            if (maxValue <= 0.f)
                maxValue = extrema[7]*weights[0]+extrema[8]*weights[1]+extrema[9]*weights[2];
            std::cout << "maximum bluminance: " << maxValue << " W/(m2 sr)" << std::endl;
        }

        // writes the scale of the luminance contours, pc0.cal
        std::fstream output("pc0.cal", std::ios::out | std::ios::binary | std::ios::trunc);
        output << "PI : 3.14159265358979323846 ;" << std::endl;
        output << "scale : " << maxValue << " ;" << std::endl;
        output << "mult : " << toString(luminance ? 179.f : 1.f) << " ;" << std::endl;
        output << "ndivs : " << 5 << " ;\n" << std::endl;

        output << "or(a,b) : if(a,a,b);"  << std::endl;
        output << "EPS : 1e-7;"  << std::endl;
        output << "neq(a,b) : if(a-b-EPS,1,b-a-EPS);"  << std::endl;
        output << "btwn(a,x,b) : if(a-x,-1,b-x);"  << std::endl;
        output << "clip(x) : if(x-1,1,if(x,x,0));"  << std::endl;
        output << "frac(x) : x - floor(x);" << std::endl;
        output << "boundary(a,b) : neq(floor(ndivs*a+.5),floor(ndivs*b+.5));\n" << std::endl;

        output << "old_red(x) = 1.6*x - .6;" << std::endl;
        output << "old_grn(x) = if(x-.375, 1.6-1.6*x, 8/3*x);" << std::endl;
        output << "old_blu(x) = 1 - 8/3*x;\n" << std::endl;

        output << "interp_arr2(i,x,f):(i+1-x)*f(i)+(x-i)*f(i+1);" << std::endl;
        output << "interp_arr(x,f):if(x-1,if(f(0)-x,interp_arr2(floor(x),x,f),f(f(0))),f(1));" << std::endl;
        output << "def_redp(i):select(i,0.18848,0.05468174," << std::endl;
        output << "0.00103547,8.311144e-08,7.449763e-06,0.0004390987,0.001367254," << std::endl;
        output << "0.003076,0.01376382,0.06170773,0.1739422,0.2881156,0.3299725," << std::endl;
        output << "0.3552663,0.372552,0.3921184,0.4363976,0.6102754,0.7757267," << std::endl;
        output << "0.9087369,1,1,0.9863);" << std::endl;
        output << "def_red(x):interp_arr(x/0.0454545+1,def_redp);" << std::endl;
        output << "def_grnp(i):select(i,0.0009766,2.35501e-05," << std::endl;
        output << "0.0008966244,0.0264977,0.1256843,0.2865799,0.4247083,0.4739468," << std::endl;
        output << "0.4402732,0.3671876,0.2629843,0.1725325,0.1206819,0.07316644," << std::endl;
        output << "0.03761026,0.01612362,0.004773749,6.830967e-06,0.00803605," << std::endl;
        output << "0.1008085,0.3106831,0.6447838,0.9707);" << std::endl;
        output << "def_grn(x):interp_arr(x/0.0454545+1,def_grnp);" << std::endl;
        output << "def_blup(i):select(i,0.2666,0.3638662,0.4770437," << std::endl;
        output << "0.5131397,0.5363797,0.5193677,0.4085123,0.1702815,0.05314236," << std::endl;
        output << "0.05194055,0.08564082,0.09881395,0.08324373,0.06072902," << std::endl;
        output << "0.0391076,0.02315354,0.01284458,0.005184709,0.001691774," << std::endl;
        output << "2.432735e-05,1.212949e-05,0.006659406,0.02539);" << std::endl;
        output << "def_blu(x):interp_arr(x/0.0454545+1,def_blup);\n" << std::endl;

        output << "isconta = if(btwn(0,v,1),or(boundary(vleft,vright),boundary(vabove,vbelow)),-1);"  << std::endl;
        output << "iscontb = if(btwn(0,v,1),btwn(.4,frac(ndivs*v),.6),-1);\n" << std::endl;

        output << "ra = 0;" << std::endl;
        output << "ga = 0;" << std::endl;
        output << "ba = 0;\n" << std::endl;

        output << "in = 1;\n" << std::endl;

        output << "ro = if(in,clip(def_red(v)),ra);" << std::endl;
        output << "go = if(in,clip(def_grn(v)),ga);" << std::endl;
        output << "bo = if(in,clip(def_blu(v)),ba);" << std::endl;
        output.close();

        // writes the second part of the file (pc1.cal)
        output.open("pc1.cal", std::ios::out | std::ios::binary | std::ios::trunc);

        // JK - 30.12.2013 - suppression du li(1) et le(1)
        // li(1) donne la brightness du pixel courant de l'image, et le(1) donne l'exposition de l'image
        //output << "norm : mult/scale/le(1);\n" << std::endl;
        output << "norm : mult/scale/(re(1)*" << weights[0] << "+ge(1)*" << weights[1]<< "+be(1)*" << weights[2] << ");\n" << std::endl;
        //output << "v = map(li(1)*norm);\n" << std::endl;
        output << "v = map((ri(1)*" << weights[0] << "+gi(1)*" << weights[1] << "+bi(1)*" << weights[2] << ")*norm);\n" << std::endl;

        output << "vleft = map(li(1,-1,0)*norm);" << std::endl;
        output << "vright = map(li(1,1,0)*norm);" << std::endl;
        output << "vabove = map(li(1,0,1)*norm);" << std::endl;
        output << "vbelow = map(li(1,0,-1)*norm);\n" << std::endl;

        output << "map(x) = x;\n" << std::endl;

        output << "ra = ri(nfiles);" << std::endl;
        output << "ga = gi(nfiles);" << std::endl;
        output << "ba = bi(nfiles);" << std::endl;
        output.close();

        // uses the falsecolor to produce luminance contours, -f pc0.cal -e in=isconta
        command = "pcomb -f pc0.cal -f pc1.cal base_" + toString(model) + ".hdr > base_" + toString(model) + "_F.hdr";
        shellcmd.addCommand(command,"Preparation of the falsecolor image");
        //cout << command << endl;
        //system(command.c_str());

        // transforms the final image in bmp
        command = "ra_bmp base_" + toString(model) + "_F.hdr base_" + toString(model) + ".bmp";
        shellcmd.addCommand(command,"Conversion of the image to BMP format");
        //cout << command << endl;
        //system(command.c_str());
    }

    // takes care of the daylight factor calculation
    if (!glare && dl) {

        // create measurment points in a file
        std::string meshInputFilename = "mesh_" + toString(model) + ".inp";
        std::fstream output(meshInputFilename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
        // roof reference
        output << "0 0 " << zMax+0.8 << " 0 0 1" << std::endl;
        // measurement points inside the room
        for (unsigned int ySubDiv=1; ySubDiv < (ySubDivMax+1); ++ySubDiv) {
            for (unsigned int xSubDiv=1; xSubDiv < (xSubDivMax+1); ++xSubDiv) {
                // note: 0.5 meters to remove from the borders of the room
                output << xMin+0.5+xSubDiv*((xMax-xMin-1)/(xSubDivMax+1)) << " "
                       << yMin+0.5+ySubDiv*((yMax-yMin-1)/(ySubDivMax+1)) << " "
                       /// TODO: si le sol n'est pas plat, corriger la normale ainsi que la valeur de Z
                       << zMin+0.8 << " "
                       << "0 0 1" << std::endl;
            }
        }
        output.close();

        // rtrace the mesh points
        command = "rtrace -w -I -h " + renderingOptions + " base_" + toString(model) + "_1.oct < mesh_" + toString(model) + ".inp | " + "rcalc -e \"$1=179.*(0.265*$1+0.67*$2+0.065*$3)\" > mesh_" + toString(model) + ".out";
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
        command = "evalglare -c base_" + toString(model) + "_G.hdr base_" + toString(model) + ".hdr > glare_" + toString(model) + ".out";
        shellcmd.addCommand(command,"Glare calculation on the image");
        //cout << command << endl;
        //system(command.c_str());

        // starts the simulation as the glare index depends on it
        shellcmd.start();
        shellcmd.wait();

        // open the output file for reading and extract the indices -> put them in the correct textEdit
        std::string glareInputFilename = "glare_" + toString(model) + ".out";
        std::fstream glareInput(glareInputFilename.c_str(), std::ios::in | std::ios::binary);
        std::string indices,buffer;
        // gets a string that contains the indices
        glareInput >> indices;
        // decomposes the indices, separated using ","
        size_t pos1 = 0, pos2 = indices.find(",",pos1);
        while (pos2 != std::string::npos) {
            glareInput >> buffer;
            glareIndices[indices.substr(pos1,pos2-pos1)] = to<float>(buffer);
            pos1 = pos2+1;
            pos2 = indices.find(",",pos1);
        }
        glareInput >> buffer;
        glareIndices[indices.substr(pos1,indices.find(":",pos1)-pos1)] = to<float>(buffer);

        // affichage
        for (std::map<std::string,float>::iterator it=glareIndices.begin();it!=glareIndices.end();++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }

        // sets the exposure to human vision
        command = "pcond -h base_" + toString(model) + "_G.hdr > base_" + toString(model) + "_GH.hdr";
        shellcmd.addCommand(command,"Conditionning the image for human eye");
        //cout << command << endl;
        //system(command.c_str());

        // transforms the image in bmp
        command = "ra_bmp base_" + toString(model) + "_GH.hdr base_" + toString(model) + ".bmp";
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
        std::string command = "btdf2radiance " + directoryName.remove(0,directoryName.lastIndexOf("/")+1).toStdString() + "/*.txt";
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
        if (DF_values.size() != xSubDivMax*ySubDivMax+1) throw(std::string("Invalid DF_values vector size."));

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
        float avgValue = accumulate(DF_values.begin()+1,DF_values.end(),0.f)/distance(DF_values.begin()+1,DF_values.end());

        // for the log scale, computes the minimum threshold (below is 0 grey scale)
        // the minValue depends on the daylight autonomy
        float FLJmin = 0.01f + (0.01f/200.f)*(std::max(DF_reqIllum,200.f)-200.f); // this equation comes from Bernard Paule's thesis
        float minThresholdValue = FLJmin*externalIlluminance;
        for (unsigned int index=0;index<xSubDivMax*ySubDivMax;++index) {
            float factorGray;
            if (!logScale) {
                // linear scale
                factorGray = std::max((DF_values[index+1]-minThresholdValue),0.f)/(maxValue-minThresholdValue);
            }
            else {
                // log scale
                factorGray = std::max(log10(DF_values[index+1]/minThresholdValue),0.)/log10(maxValue/minThresholdValue);
            }
            image.data[index]=(unsigned char)((unsigned int) (static_cast<float>(image.paletteColors-1)*factorGray));
        }
        std::string meshBMPFilename = "mesh_" + toString(model) + ".bmp";
        write_bmp(meshBMPFilename.c_str(), &image);
        // emits the maxIlluminance on the grid value & the corresponding DF
        DF_msg = "max Illuminance/DF: " +     QString::number(maxValue,'f',0) + " lx/" + QString::number(100.f*maxValue/externalIlluminance,'f',1) + " %"
               + "\navg Illuminance/DF: " + QString::number(avgValue,'f',0) + " lx/" + QString::number(100.f*avgValue/externalIlluminance,'f',1) + " %"
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

    std::cout << "...writing file..." << std::endl;

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
    std::cout << "File size: " << h.filesz << " bytes\tcreator1: " << h.creator1 << "\tcreator2: " << h.creator2 << std::endl;
    std::cout << "Offset: " << h.bmp_offset << " bytes" << std::endl;

    // writes the number of bytes in the header from this point until the bitmap data
    uint32_t numBytesFromThisPoint=40;
    fwrite(&numBytesFromThisPoint,sizeof(unsigned char),4,fp);

    // reads the width and height
    fwrite(&b->width, sizeof(unsigned char), 4, fp);
    fwrite(&b->height,sizeof(unsigned char), 4, fp);
    uint16_t colorPlanes=1;
    fwrite(&colorPlanes,sizeof(unsigned char), 2, fp);
    fwrite(&b->bitsPerPixel,sizeof(unsigned char), 2, fp);
    std::cout << "width: " << b->width << " pixels\theight: " << b->height << " pixels\tbitsPerPixel: " << b->bitsPerPixel << std::endl;

    // writes the compression, no compression
    uint32_t compr=0;
    fwrite(&compr,sizeof(unsigned char),4,fp);
    std::cout << ((compr == 0) ? "No compression used." : "Compression: ") << std::endl;

    // size of the data & resolution
    uint32_t hRes=2835;
    uint32_t vRes=2835;
    fwrite(&rawSize,sizeof(unsigned char),4,fp);
    fwrite(&hRes,sizeof(unsigned char),4,fp);
    fwrite(&vRes,sizeof(unsigned char),4,fp);
    std::cout << "Raw BMP data: " << rawSize << " bytes" << std::endl;
    std::cout << "Horizontal resolution: " << hRes << " pixels/m\tVertical resolution: " << vRes << " pixels/m" << std::endl;

    // palette and important colors
    uint32_t importantColors=0;
    fwrite(&b->paletteColors,sizeof(unsigned char),4,fp);
    fwrite(&importantColors,sizeof(unsigned char),4,fp);
    std::cout << "Palette colors: " << b->paletteColors << std::endl;
    std::cout << "Important colors: " << importantColors << std::endl;

    // writes the palette that we want to use (gray scale palette)
    for (unsigned int colorIndex=0;colorIndex<b->paletteColors;++colorIndex) {
        fwrite(&(b->palette[colorIndex*4]),sizeof(unsigned char),4,fp);
    }
    std::cout << "Actual position within the file (at the end of header): " << ftell(fp) << std::endl;

    // writes the actual raw data, careful here when writing, padding data multiple of 4 bytes
    for (unsigned int index=0;index<b->height;++index) {
        fwrite(&(b->data[index*widthSize]),sizeof(unsigned char),widthSize,fp);
    }

  fclose(fp);

    return;

}
