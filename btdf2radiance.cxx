// Conversion between BTDF and Radiance files
//
// This program takes in account data with no double BTDF values
//
// Valid for prism2 approximation


// Jérôme KAEMPF, LESO, EPFL
// jerome.kaempf@epfl.ch

// multi-output version / complete pic separation / analytic model comparison / weighted pic position
// symetric data in btdf taken in account (Isym=0,2,3,4)
// interpolation between input zones
// view through the simulated panel


#include <iostream>
#include <iomanip> // such as setprecision()
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstdio> // to use scanf()
#include <cstring> // to use string definitions
#include <algorithm> // to find max_element and min_element
#include <sstream> // include string streams
#include <cassert> // to make assertions (verifications)
#include <vector>
#include <cfloat>

using namespace std;

template <typename T> inline T to(const string &s) {

    T value;
    std::stringstream ss(s);
    ss >> value;

    return value;

}

template <typename T> inline string toString(const T &s) {

    std::stringstream ss;
    ss << s;

    return ss.str();

}

const string version = "1.0";
const string date = "06/07/10";

// *** finds adjacents positions in a vector *** //
void adjacents(int pos, vector<int> &adj, float pasp, float past);
// *** zones according to the input/output directions *** //
int zone_tregenza(float phi, float theta);
int zone_5deg(float phi, float theta);
// *** correspondance between the standard tregenza counting and the reversed one (used by mkillum) *** //
int zone_tregenza(int zone_reversed);
int zone_5deg(int zone_5deg_reversed);
// *** transformation of the numbering of zones from the xyz'' to xyz' coordinate reference frames *** //
int zone_tregenza_xyz_prime(int zone_tregenza_xyz_doubleprime);
// *** display of the reversed zone (used by mkillum) according to the standard one *** //
int zone_tregenza_reversed(int zone);
// *** symetries *** //
int zone_tregenza_180minus(int zone);
int zone_5deg_180minus(int zone);
int zone_tregenza_180plus(int zone);
int zone_5deg_180plus(int zone);
int zone_tregenza_360minus(int zone);
int zone_5deg_360minus(int zone);
// *** projected solid angles of the zones *** //
double zone_tregenza_projected_solid_angle(int zone);
double zone_5deg_projected_solid_angle(int zone);
// *** cal and xml files *** //
string entetecal(char *material);
string basdepagecal();
string exrad(char *material);
float modulo(float nombre, float base);
string entetexml145x145(char *material);
string entetexml145x1297(char *material);
string basdepagexml();


int main(int argc, char * argv[]) {

// *** TEST de determination de la zone de Tregenza avec phi, theta *** //
    //    cout << "Phi: 27.7, Theta: 42.3, Zone Tregenza: " << zone_tregenza(27.7f,42.3f) << endl;
    //    cout << "Phi: 27.7, Theta: 6.0, Zone Tregenza: " << zone_tregenza(27.7f,6.f) << endl;

// *** TEST des zones de Tregenza renversees *** //
    //    cout << "Zone Tregenza:" << endl;
    //    for (size_t i=1; i<=145; ++i) cout << i << "\t" << zone_tregenza_reversed(i) << "\t" << zone_tregenza(zone_tregenza_reversed(i)) << endl;
    //    string essai;
    //    cin >> essai;

// *** TEST des zones de Tregenza x'y'z' version x''y''z'' *** //
//        cout << "Zone Tregenza:" << endl;
//        for (size_t i=1; i<=145; ++i) cout << i << "\t" << zone_tregenza_xyz_prime(i) << endl;
//        string essai;
//        cin >> essai;

// *** TEST des angles solides projetes *** //
    //    double sum=0.f;
    //    cout << endl;
    //    for (size_t i=0; i<145; ++i) { cout << "zone: " << i+1 << "\tprojectedSolidAngle: " << zone_tregenza_projected_solid_angle(i+1) << endl; sum+=zone_tregenza_projected_solid_angle(i+1);}
    //    cout << "sum: " << sum << endl;

// *** TEST de symétries du système *** //
    //    cout << "zone\t360-\t180-\t180+" << endl;
    //    for (unsigned int i=1;i<=145;++i) {
    //
    //        cout << i << "\t" << zone_tregenza_360minus(i) << "\t" << zone_tregenza_180minus(i)
    //             << "\t" << zone_tregenza_180plus(i) << endl;
    //
    //    }
    //    string essai;
    //    cin >> essai;


    string input_filename;
    string output_filename,output_filename2,output_filename3, output_filename4, output_filename5, output_filename6;
    string output_filename7, output_filename8;
    string output_filename9, output_filename10;
    string output_filename11, output_filename12;
    string output_filename13;

    stringstream output7a;
    stringstream output7b;

    char buffer[200];
    streampos pos,endoffset;

    int phi, theta;
    float btdf;

    vector<int> phi2,theta2;
    vector<double> btdf2,newbtdf;

    vector<int> pic1,pic2,supseuil,adjacent,adjacent2,reste;
    double max1,max2,maxtemp;
    double max1phi,max1theta,max2phi,max2theta;

    // definition des max pondérés par la btdf sur l'ensemble défini comme appartenant au pic

    double max1phiw,max1thetaw,max2phiw,max2thetaw;

    double seuil=0.1; // seuil fixé à 0.1 % de transmission

    double pasphi, pastheta;

    double somme,somme1,somme2,sommereste,sommesousseuil;
    int posmax,posmax1,posmax2;

    int nbrelements, premierpic2;

    bool maxsurpic1,maxsurpic2;

    int theta_in, phi_in;

    char material[100];

    // storage of the coefficients for 145 Tregenza zones (input), two peaks approximation
    double coeff1[145],coeff2[145];
    double p1[145],t1[145];
    double p2[145],t2[145];
    // instead of only two peaks, we have 145 of them, so an array is defined
    double coeff[145][145];
    // full resolution of our photogoniometer 5 deg by 5 deg in output and 145 in input
    double coeff5deg[145][1297];

    // initialisation of the values
    for (unsigned int i=0; i<145; ++i) {
      coeff1[i] = 0.;
      coeff2[i] = 0.;
      p1[i]     = 0.;
      p2[i]     = 0.;
      t1[i]     = 0.;
      t2[i]     = 0.;
      for (unsigned int j=0; j<145; ++j) coeff[i][j] = 0.;
      for (unsigned int j=0; j<1297; ++j) coeff5deg[i][j] = 0.;
    }

    // storage of the zones per symmetry (maximum 4 zones through symmetry), by default no symetry (Isym=0)
    unsigned int zone,zone2,zone3,zone4;
    int Isym=0;

    // ********** Début du programme ********** //

    cout << "\nBTDF2radiance v." << version << endl;

    if (argc==1) {
        // nombre d'arguments insuffisant
        cout << "Usage: btdf2radiance <BTDF data file(s)> OR btdf2radiance -f <145x145 transmission matrix file>\n" << endl;
        return EXIT_FAILURE;
    }
    else if (string(argv[1]) == "-f") {
        // test sur l'argument 1 pour voir si l'on a une fichier de type matrice de transmission
        if (argc==2) { cout << "Missing the transmission matrix file\n" << endl; return EXIT_FAILURE; }
        else if (string(argv[2]) == "identity") {
            // materiaux identitaire
            strcpy(material,"IdentityTransmissionMatrix");
            // écriture de la matrice identitaire
            for (unsigned int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
                for (unsigned int outputZoneIndex=0; outputZoneIndex<145; ++outputZoneIndex) {
                    coeff[inputZoneIndex][outputZoneIndex] = 0.;
                }
                //coeff[inputZoneIndex][zone_tregenza_identity(inputZoneIndex+1)-1] = 1./zone_tregenza_projected_solid_angle(zone_tregenza_identity(inputZoneIndex+1));
                coeff[inputZoneIndex][inputZoneIndex] = 1./zone_tregenza_projected_solid_angle(inputZoneIndex+1);
            }
        }
        else {

            // essaye d'ouvrir le fichier de matrice 145x145
            input_filename = argv[2];
            fstream input(input_filename.c_str(), ios::in | ios::binary);
            // test d'ouverture
            if (!input.is_open())
            {
                cerr << "Error opening input file: " << argv[2] << "\n" << endl;
                return EXIT_FAILURE;
            }
            // détermination du nom des fichiers de sortie... (en enlevant l'extension de 3 lettres)
            string filename(input_filename.begin(),input_filename.end()-4);

            // lecture du fichier et mise dans la mémoire
            string stringBuffer;
            for (unsigned int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
                for (unsigned int outputZoneIndex=0; outputZoneIndex<145; ++outputZoneIndex) {
                    // lecture des 145 valeurs de la zone d'entrée
                    input >> stringBuffer;
                    coeff[inputZoneIndex][outputZoneIndex] =  to<double>(stringBuffer);
                    coeff[inputZoneIndex][outputZoneIndex] /= zone_tregenza_projected_solid_angle(outputZoneIndex+1);
                }
            }

            // fermeture du fichier
            input.close();
        }
        // passage à l'écriture des résultats

    }
    else {

        // boucle sur tous les fichiers mentionnés
        for (int b=1;b<argc;b++) {

            input_filename = argv[b];
            fstream input(input_filename.c_str(), ios::in | ios::binary);
            // test d'ouverture
            if (!input.is_open())
            {
                cerr << "Error opening input file: " << argv[b] << "\n" << endl;
                return EXIT_FAILURE;
            }
            // détermination du nom des fichiers de sortie...
            string filename(input_filename.begin(),input_filename.end()-4);

            // création des noms des fichiers de sortie
            output_filename = filename;
            output_filename += "p1.dat";

            output_filename2 = filename;
            output_filename2 += "p2.dat";

            output_filename3 = filename;
            output_filename3 += "sups.dat";

            output_filename9 = filename;
            output_filename9 += "rehp.dat";

            output_filename11 = filename;
            output_filename11 += "ress.dat";

            output_filename4 = filename;
            output_filename4 += "hsphp1.dat";

            output_filename5 = filename;
            output_filename5 += "hsphp2.dat";

            output_filename6 = filename;
            output_filename6 += "hsphsups.dat";

            output_filename10 = filename;
            output_filename10 += "hsphrehp.dat";

            output_filename12 = filename;
            output_filename12 += "hsphress.dat";

            // ouverture des fichiers de sortie
            /*
            fstream output1 (output_filename.c_str(), ios::out | ios::binary);
            fstream output2 (output_filename2.c_str(), ios::out | ios::binary);
            fstream output3 (output_filename3.c_str(), ios::out | ios::binary);
            fstream output3rhp (output_filename9.c_str(), ios::out | ios::binary);
            fstream output3rss (output_filename11.c_str(), ios::out | ios::binary);
            fstream output4 (output_filename4.c_str(), ios::out | ios::binary);
            fstream output5 (output_filename5.c_str(), ios::out | ios::binary);
            fstream output6 (output_filename6.c_str(), ios::out | ios::binary);
            fstream output6rhp (output_filename10.c_str(), ios::out | ios::binary);
            fstream output6rss (output_filename12.c_str(), ios::out | ios::binary);
            */

            // initialisation des scalaires
            phi=0;
            theta=0;
            btdf=0.;
            somme=0.;
            somme1=0.;
            somme2=0.;
            sommereste=0.;
            sommesousseuil=0.;
            nbrelements=0;
            premierpic2=0;
            max1phi=0.;
            max1theta=0.;
            max2phi=0.;
            max2theta=0.;
            max1phiw=0.;
            max1thetaw=0.;
            max2phiw=0.;
            max2thetaw=0.;
            max1=0.;
            max2=0.;
            maxtemp=0.;
            posmax=0;
            posmax1=0;
            posmax2=0;
            pasphi=0.;
            pastheta=0.;
            theta_in=0;
            phi_in=0;
            zone=0;
            zone2=0;
            zone3=0;
            zone4=0;

            // initialisation des vecteurs
            phi2.clear();
            theta2.clear();
            btdf2.clear();
            newbtdf.clear();
            pic1.clear();
            pic2.clear();
            reste.clear();
            supseuil.clear();
            adjacent.clear();
            adjacent2.clear();

            // start the reading
            input.seekg (0, ios::end);
            endoffset = input.tellg();
            input.seekg (0, ios::beg);
            pos = input.tellg();

            while (!input.eof()&& input.tellg()!=endoffset) {

                input.seekg(pos);

                input.getline(buffer,200,'\n');

                // if not a comment (#) and END (the last thing produced by the photogonio)
                if (buffer[0] != '#' && buffer[0] != 'E') {
                    //			cout << buffer << endl;
                    sscanf(buffer, "%d\t%d\t%f", &phi, &theta, &btdf);

                    phi2.push_back(phi);
                    theta2.push_back(theta);
                    btdf2.push_back(btdf);

                }            // lecture de l entete
                else if ( buffer[0] == '#' && buffer[1] == 'p' && buffer[2] == 'h' && buffer[3] == 'i' && buffer[4] == '_' && buffer[5] == '1' ) {

                    sscanf(buffer, "#phi_1: %d°", &phi_in); // determination du phi en entrée

                }
                else if ( buffer[0] == '#' && buffer[1] == 't' && buffer[2] == 'h' && buffer[3] == 'e' && buffer[4] == 't' && buffer[5] == 'a' ) {

                    sscanf(buffer, "#theta_1: %d°", &theta_in);  // determination du theta en entrée

                }
                else if ( buffer[0] == '#' && buffer[1] == 'm' && buffer[2] == 'a' && buffer[3] == 't' && buffer[4] == 'e' && buffer[5] == 'r' ) {

                    sscanf(buffer, "#material: %s", &material[0]);
                    cout << "\nMaterial: " << material << endl; // determination du materiaux
                }
                else if ( buffer[0] == '#' && buffer[1] == 'I' && buffer[2] == 's' && buffer[3] == 'y' && buffer[4] == 'm' ) {

                    sscanf(buffer, "#Isym = %d", &Isym);
                    cout << "Symmetry: " << Isym << endl;  // determination de la symmetrie
                }

                pos=input.tellg();

            }

            // initialisation des vecteurs selon la taille de la BTDF
            newbtdf.assign(btdf2.size(), 0.);
            pic1.assign(btdf2.size(), 1);
            pic2.assign(btdf2.size(), 1);
            reste.assign(btdf2.size(), 1);
            supseuil.assign(btdf2.size(), 0);
            adjacent.assign(btdf2.size(), 0);
            adjacent2.assign(btdf2.size(), 0);

            // calcul de la zone de tregenza correspondante au phi_in et theta_in, calcul du pas en phi et theta
            zone = zone_tregenza(static_cast<float>(phi_in)+1e-1f, static_cast<float>(theta_in));
            cout << "\nInput Tregenza zone: " << zone << "\t\t(phi_1: " << phi_in << "°, theta_1: " << theta_in << "°)" << endl;
            pasphi=(phi2[1]-phi2[0]);
            pastheta=(theta2[(int)(360/pasphi)]-theta2[0]);
            cout << "\nStep in phi: " << pasphi << "° in theta: " << pastheta << "°" <<endl; // affichage du pas en phi et theta

            // mise à zéro des valeurs pour cet direction d'entrée
            for (unsigned int j=0; j<145; ++j) coeff[zone-1][j] = 0.;
            for (unsigned int j=0; j<1297; ++j) coeff5deg[zone-1][j] = 0.;

            // calculer les angles solides projetés en fonction de la zone choisie
            // et diviser par ceux-ci les résultats en transmission
            for (size_t j=0;j<btdf2.size();++j) {

                // calcul de la transmission pour des éléments de 0.5 degrés (ppmc entre résolution max du photogonio et la subdivision de Tregenza)
                // par multiplication de l'angle solide projeté
                // qui est ensuite divisé par l'angle solide projeté de la zone sur laquelle on fait la somme des éléments qui lui appartiennent
                // ce calcul est fait pour MKILLUM
                // attention la numérotation des zones est inversée
                if (theta2[j] < 180.-(pastheta/2.)) {
                    // calcul de la matrice 145x145 de transmission
                    //cout << "theta2: " << theta2[j] << "\tfrom: " << (theta2[j]-(pastheta/2.f)) << "\tto: " << (theta2[j]+(pastheta/2.f)) << endl;
                    //cout << "phi2: " << phi2[j] << "\tfrom: " << (phi2[j]-(pasphi/2.f)) << "\tto: " << (phi2[j]+(pasphi/2.f)) << endl;
                    // boucle sur les demi-degrés
                    for (double cpTheta = (theta2[j]-(pastheta/2.))+0.25; cpTheta < (theta2[j]+(pastheta/2.)); cpTheta += 0.5) {
                        //cout << "cpTheta: " << cpTheta << "\tfrom: " << cpTheta-0.25f << "\tto: " << cpTheta+0.25f << endl;
                        for (double cpPhi = (phi2[j]-(pasphi/2.))+0.25; cpPhi < (phi2[j]+(pasphi/2.)); cpPhi += 0.5) {
                            //cout << "cpPhi: " << cpPhi << "\tfrom: " << cpPhi-0.25f << "\tto: " << cpPhi+0.25f;
                            //cout << "\tzone: " << zone_tregenza(cpPhi,cpTheta) << endl;
                            // stockage dans la matrice du facteur de transmission du flux associé à cette zone
                            //cout << "zone_entrée: " << zone << "\tzone_sortie: " << zone_tregenza(cpPhi,cpTheta) << endl;
                            coeff[zone-1][zone_tregenza(cpPhi,cpTheta)-1] +=
                                btdf2[j]
                                *(cos( (cpTheta+0.25)/180.*M_PI ) - cos( (cpTheta-0.25)/180.*M_PI ))*cos(cpTheta/180.*M_PI)*(0.5*(M_PI/180.))
                                /zone_tregenza_projected_solid_angle(zone_tregenza(cpPhi,cpTheta));
                            //cout << "transmission: " << 100.f*btdf2[j]*(cos( (cpTheta+0.25f)/180.f*M_PI ) - cos( (cpTheta-0.25f)/180.f*M_PI ))*cos(cpTheta/180.f*M_PI)*(0.5f*(M_PI/180.f)) << endl;
                            coeff5deg[zone-1][zone_5deg(cpPhi,cpTheta)-1] +=
                                btdf2[j]
                                *(cos( (cpTheta+0.25)/180.*M_PI ) - cos( (cpTheta-0.25)/180.*M_PI ))*cos(cpTheta/180.*M_PI)*(0.5*(M_PI/180.))
                                /zone_5deg_projected_solid_angle(zone_5deg(cpPhi,cpTheta));

                        }
                    }
                }
                else {
                    // CAS spécial de la zone au sommet de l'hemisphere (1 zone)
                    //cout << "zone_entrée: " << zone << "\tzone_sortie: " << 145 << endl;
                    coeff[zone-1][144] +=
                        btdf2[j]
                        *(cos( (theta2[j]-(pastheta/2.))/180.*M_PI ) - cos( theta2[j]/180.*M_PI ))*2.*M_PI
                        /zone_tregenza_projected_solid_angle(145);
                    //cout << "transmission zone 145: " << 100.f*btdf2[j]*(cos( (theta2[j]-(pastheta/2.f))/180.f*M_PI ) - cos( theta2[j]/180.f*M_PI ))*2.f*M_PI;
                    coeff5deg[zone-1][1296] +=
                        btdf2[j]
                        *(cos( (theta2[j]-(pastheta/2.))/180.*M_PI ) - cos( theta2[j]/180.*M_PI ))*2.*M_PI
                        /zone_5deg_projected_solid_angle(1297);
                }

                // BTDF multipliée par l'angle solide correspondant que l'on projette sur le plan d'emission
                // et en POURCENT
                // ce calcul est fait pour prism2
                //        cout << phi2[j] << " " << theta2[j] << " " << btdf2[j] << " " << j << endl;
                if (theta2[j] < 180.-(pastheta/2.)) {
                    btdf2[j]*=(cos( (theta2[j]+(pastheta/2.))/180.*M_PI ) - cos( (theta2[j]-(pastheta/2.))/180.*M_PI ))*cos(theta2[j]/180.*M_PI)*(pasphi*(M_PI/180.))*100.;
                }
                else {
                    btdf2[j]*=(cos( (theta2[j]-(pastheta/2.))/180.*M_PI ) - cos( theta2[j]/180.*M_PI ))*2.*M_PI*100.;
                    // CAS spécial de la zone au sommet de l'hemisphere (1 zone)
                }

                somme+=btdf2[j];  // transmission globale

                // marquage des données supérieures au seuil, intégrées sur l'angle solide projeté
                if( btdf2[j] >= seuil ) {
                    supseuil[j]=1; // données marquées pour leur utilisation
                    ++nbrelements;
                }

                // SORTIE formatée pour PAW
                // sprintf(formatted, "%3d. %3d. %6.3f", phi2[j], theta2[j], btdf2[j]);
                // output << formatted << endl;
            }

            // recherche du maximum dans la BTDF
            max1 = *max_element(btdf2.begin(), btdf2.end()); // recherche du max
            posmax1 = distance(btdf2.begin(),max_element(btdf2.begin(),btdf2.end()));

            max1phi = phi2[posmax1];  // prise de la valeur phi pour pic1
            max1theta = theta2[posmax1];  // prise de la valeur theta pour pic2

            pic1[posmax1]=0;  // attribution de la position du max

            posmax = posmax1; // mise en variable temporaire
            maxsurpic1 = true;

            for (int l=1;l<nbrelements;l++) {

                // Boucle pour séparer les deux pics !

                if ( maxsurpic1 ) {
                  adjacents(posmax, adjacent, pasphi, pastheta);  // position des adjacents au max trouvé en dessus sur le pic1
                }
                else if ( maxsurpic2 ) {
                  adjacents(posmax, adjacent2, pasphi, pastheta);  // position des adjacents au max trouvé en dessus sur le pic2
                }

                for (size_t j=0;j<btdf2.size();++j) {

                  newbtdf[j]=btdf2[j]*pic1[j]*pic2[j]*reste[j]*supseuil[j];

                }

                maxtemp = *max_element(newbtdf.begin(), newbtdf.end()); // cherche le maximum dans newbtdf
                posmax = distance(newbtdf.begin(), max_element(newbtdf.begin(), newbtdf.end()));  // donne la position du maximum dans newbtdf

                if ( adjacent[posmax] == 1 ) {

                  pic1[posmax]=0;
                  maxsurpic1=true;
                  maxsurpic2=false;

                }
                else if ( adjacent2[posmax] == 1 || premierpic2 == 0 ){

                  pic2[posmax]=0;
                  maxsurpic1=false;
                  maxsurpic2=true;
                  premierpic2+=1;

                  if (premierpic2 == 1) {

                    posmax2 = posmax;
                    max2phi = phi2[posmax];
                    max2theta = theta2[posmax];
                    max2=btdf2[posmax];

                  }
                }
                else {

                  maxsurpic1=false;
                  maxsurpic2=false;
                  reste[posmax]=0;

                }
                // on recommence !!
                //    cout << "posmax: " << posmax << "max2: " << maxtemp << "pic1()" << pic1[posmax] << " pic2() " << pic2[posmax] << " maxsurpic1: " << maxsurpic1 << endl;

            }

            // Ecriture des données
            // Mis en veilleuse dans la version commerciale

            for (size_t j1=0;j1<btdf2.size();++j1) {

                /*

                if ( supseuil[j1] == 1 ) { // sortie de ce qui est supérieur au seuil seulement

                output1 << phi2[j1] << "\t" << theta2[j1] << "\t" << abs(pic1[j1]-1)*btdf2[j1] << endl;
                output2 << phi2[j1] << "\t" << theta2[j1] << "\t" << abs(pic2[j1]-1)*btdf2[j1] << endl;
                output3 << phi2[j1] << "\t" << theta2[j1] << "\t" << btdf2[j1] << endl;
                output3rhp << phi2[j1] << "\t" << theta2[j1] << "\t" << (1.*abs(reste[j1]-1))*btdf2[j1] << endl; // reste sup seuil (hors pic)
                output4 << cos(phi2[j1]/180.*M_PI)*sin(theta2[j1]/180.*M_PI) << "\t" << sin(theta2[j1]/180.*M_PI)*sin(phi2[j1]/180.*M_PI) << "\t" << abs(pic1[j1]-1)*btdf2[j1] << endl;
                output5 << cos(phi2[j1]/180.*M_PI)*sin(theta2[j1]/180.*M_PI) << "\t" << sin(theta2[j1]/180.*M_PI)*sin(phi2[j1]/180.*M_PI) << "\t" << abs(pic2[j1]-1)*btdf2[j1] << endl;
                output6 << cos(phi2[j1]/180.*M_PI)*sin(theta2[j1]/180.*M_PI) << "\t" << sin(theta2[j1]/180.*M_PI)*sin(phi2[j1]/180.*M_PI) << "\t" << btdf2[j1] << endl;
                output6rhp << cos(phi2[j1]/180.*M_PI)*sin(theta2[j1]/180.*M_PI) << "\t" << sin(theta2[j1]/180.*M_PI)*sin(phi2[j1]/180.*M_PI) << "\t" << abs(reste[j1]-1)*btdf2[j1] << endl;  // reste sup seuil (hors pic)

                }
                else { // sortie du reste sous seuil

                  output3rss << phi2[j1] << "\t" << theta2[j1] << "\t" << btdf2[j1] << endl;
                  output6rss << cos(phi2[j1]/180.*M_PI)*sin(theta2[j1]/180.*M_PI) << "\t" << sin(theta2[j1]/180.*M_PI)*sin(phi2[j1]/180.*M_PI) << "\t" << btdf2[j1] << endl;

                }

                */

                // recherche du centre de gravité associé aux pics 1 et 2
                max1phiw += btdf2[j1]*abs(pic1[j1]-1)*phi2[j1];
                max1thetaw += btdf2[j1]*abs(pic1[j1]-1)*theta2[j1];
                max2phiw += btdf2[j1]*abs(pic2[j1]-1)*phi2[j1];
                max2thetaw += btdf2[j1]*abs(pic2[j1]-1)*theta2[j1];

                // calcul des coefficients de transmission
                somme1 += btdf2[j1]*abs(pic1[j1]-1);
                somme2 += btdf2[j1]*abs(pic2[j1]-1);
                sommereste += btdf2[j1]*abs(reste[j1]-1);
                sommesousseuil += btdf2[j1]*reste[j1]*pic1[j1]*pic2[j1];

            }

            // division par la somme
            if ( somme1 > 0 ) {
                max1phiw /= somme1;
                max1thetaw /= somme1;
            }
            else {
                max1phiw = 0.;
                max1thetaw = 0.;
            }
            if (somme2 > 0 ) {
                max2phiw /= somme2;
                max2thetaw /= somme2;
            }
            else {
                max2phiw = 0.;
                max2thetaw = 0.;
            }

            // sortie en stdout
            cout.precision(3);
            cout << "\nTransmission:\t\t" << somme << " %\n" << endl;

            //  cout << "Absolute maximum over the domain:\t\t" << max1 << " %" << endl;
            //  cout << "Position in phi: " << max1phi << "°\tin theta: " << max1theta << "°" << endl;
            cout << "Peak 1:\t\t\t" << somme1 << " %" << "\nPosition in (phi,theta): (" << max1phiw << "°," << max1thetaw << "°)\n" << endl;

            //  cout << "Local maximum over a restricted domain:\t\t\t" << max2 << " %" << endl;
            //  cout << "Position in phi:\t\t\t" << max2phi << "°\nPosition in theta:\t\t\t" << max2theta << "°" << endl;
            cout << "Peak 2:\t\t\t" << somme2 << " %" << "\nPosition in (phi,theta): (" << max2phiw << "°," << max2thetaw << "°)" << endl;

            cout << "\nRest:\t\t\t" << sommereste << " %\n" << endl;
            cout << "Under threshold:\t" << sommesousseuil << " %" << endl;
            cout << "(Threshold:\t\t" << seuil << " %)\n" << endl;

            // sortie dans un buffer de type stringstream (test)

            output7b << phi_in << "\t" << theta_in << "\t" << somme1 << "\t" << max1phiw << "\t" << max1thetaw << "\t" << somme2 << "\t" << max2phiw << "\t" << max2thetaw << endl;
            //  output7a << zone << "\t" << somme1/100. << "\t" << max1phi << "\t" << max1theta << "\t" << somme2/100. << "\t" << max2phi << "\t" << max2theta << endl;


            /// TODO: vérifier ici les équations
            // préparation des zones de sorties pour les différentes symmétries du système
            zone2 = zone_tregenza(360.f-(static_cast<float>(phi_in)+1e-1f), static_cast<float>(theta_in)); // symétrie d'axe X
            zone3 = zone_tregenza(180.f-(static_cast<float>(phi_in)+1e-1f), static_cast<float>(theta_in)); // symétrie d'axe Y
            zone4 = zone_tregenza(180.f+(static_cast<float>(phi_in)+1e-1f), static_cast<float>(theta_in)); // symétrie centrale

            cout << "Verification: zone1=" << zone << "\tzone2=" << zone2 << "\tzone3=" << zone3 << "\tzone4=" << zone4 << endl;

            cout << "Input Tregenza zone (reversed): " << zone << "(" << zone_tregenza_reversed(zone) << ") ";

            // in the following, zone \in [1,145], we need \in [0,145[
            coeff1[zone-1] = somme1/100.;
            coeff2[zone-1] = somme2/100.;
            p1[zone-1] = modulo(180-max1phiw,360);
            t1[zone-1] = max1thetaw;
            p2[zone-1] = modulo(180-max2phiw,360);
            t2[zone-1] = max2thetaw;

            // FOR TESTING ONLY
            //Isym = 3;

            if ( (Isym == 4 || Isym == 2) && (zone2 != zone) ) {

                if( !(coeff1[zone2-1] > 0) ) coeff1[zone2-1] = somme1/100.;
                if( !(coeff2[zone2-1] > 0) ) coeff2[zone2-1] = somme2/100.;
                if( !(p1[zone2-1] > 0) ) p1[zone2-1] = modulo((360 - (180-max1phiw)),360);
                if( !(t1[zone2-1] > 0) ) t1[zone2-1] = max1thetaw;
                if( !(p2[zone2-1] > 0) ) p2[zone2-1] = modulo((360 - (180-max2phiw)),360);
                if( !(t2[zone2-1] > 0) ) t2[zone2-1] = max2thetaw;
                // copie des valeurs dans la matrice
                for (unsigned int zoneIndex=0; zoneIndex<145; ++zoneIndex) coeff[zone2-1][zone_tregenza_360minus(zoneIndex+1)-1] = coeff[zone-1][zoneIndex];
                for (unsigned int zoneIndex=0; zoneIndex<1297; ++zoneIndex) coeff5deg[zone2-1][zone_5deg_360minus(zoneIndex+1)-1] = coeff5deg[zone-1][zoneIndex];
                // output de la zone 3 correspondante
                cout << zone2 << "(" << zone_tregenza_reversed(zone2) << ") ";
            }
            if ( (Isym == 4 || Isym == 3) && (zone3 != zone) && (zone3 != zone2) ) {

                if( !(coeff1[zone3-1] > 0) ) coeff1[zone3-1] = somme1/100.;
                if( !(coeff2[zone3-1] > 0) ) coeff2[zone3-1] = somme2/100.;
                if( !(p1[zone3-1] > 0) ) p1[zone3-1] = modulo((180 - (180-max1phiw)),360);
                if( !(t1[zone3-1] > 0) ) t1[zone3-1] = max1thetaw;
                if( !(p2[zone3-1] > 0) ) p2[zone3-1] = modulo((180 - (180-max2phiw)),360);
                if( !(t2[zone3-1] > 0) ) t2[zone3-1] = max2thetaw;
                // copie des valeurs dans la matrice
                for (unsigned int zoneIndex=0; zoneIndex<145; ++zoneIndex) coeff[zone3-1][zone_tregenza_180minus(zoneIndex+1)-1] = coeff[zone-1][zoneIndex];
                for (unsigned int zoneIndex=0; zoneIndex<1297; ++zoneIndex) coeff5deg[zone3-1][zone_5deg_180minus(zoneIndex+1)-1] = coeff5deg[zone-1][zoneIndex];
                // output de la zone 1 correspondante
                cout << zone3 << "(" << zone_tregenza_reversed(zone3) << ") ";
            }
            if ( (Isym == 4) && (zone4 != zone) && (zone4 != zone3) && (zone4 != zone2) ) {

                if( !(coeff1[zone4-1] > 0) ) coeff1[zone4-1] = somme1/100.;
                if( !(coeff2[zone4-1] > 0) ) coeff2[zone4-1] = somme2/100.;
                if( !(p1[zone4-1] > 0) ) p1[zone4-1] = modulo((180 + (180-max1phiw)),360);
                if( !(t1[zone4-1] > 0) ) t1[zone4-1] = max1thetaw;
                if( !(p2[zone4-1] > 0) ) p2[zone4-1] = modulo((180 + (180-max2phiw)),360);
                if( !(t2[zone4-1] > 0) ) t2[zone4-1] = max2thetaw;
                // copie des valeurs dans la matrice
                for (unsigned int zoneIndex=0; zoneIndex<145; ++zoneIndex) coeff[zone4-1][zone_tregenza_180plus(zoneIndex+1)-1] = coeff[zone-1][zoneIndex];
                for (unsigned int zoneIndex=0; zoneIndex<1297; ++zoneIndex) coeff5deg[zone4-1][zone_5deg_180plus(zoneIndex+1)-1] = coeff5deg[zone-1][zoneIndex];
                // output de la zone 2 correspondante
                cout << zone4 << "(" << zone_tregenza_reversed(zone4) << ") ";
            }
            if ( Isym == 1) {

                cerr << "\nRotationnal symmetry not taken into account" << endl;
                return 1;
                /// TODO: implémenter la symétrie rotationnelle, copie des valeurs selon toutes les zones de la meme bande

            }
            else if ( Isym == 0 ) {

                cerr << "\nNo symetry given." << endl;
                return 1;
            }
            cout << "\n" << endl;

            // fermeture des fichiers utilisés

            input.close();

            /*
            output1.close();
            output2.close();
            output3.close();
            output4.close();
            output5.close();
            output6.close();
            */

        } // fin de la boucle sur les arguments de la commande en ligne
    } // fin des conditions de test sur les arguments

    // préparation de la sortie dans un fichier compatible avec script radiance

    //output_filename7 = material;
    //output_filename7 += ".cal";
    output_filename7 = "btdf2prism2.cal";

    //output_filename13 = material;
    //output_filename13 += ".comp";

    fstream output7 (output_filename7.c_str(), ios::out | ios::binary);
    //  fstream output13 (output_filename13.c_str(), ios::out | ios::binary);

    output7a << fixed << setprecision(4);
    output7a << "c1(x)=select(x"; // sortie du coefficient 1
    for (int i1=0;i1<145;i1++) {
    output7a << "," <<coeff1[i1];
    }
    output7a << ");" << endl;

    output7a << "p1(x)=select(x";
    for (int i2=0;i2<145;i2++) {
    output7a << "," <<p1[i2];
    }
    output7a << ");" << endl;

    output7a << "t1(x)=select(x";
    for (int i3=0;i3<145;i3++) {
    output7a << "," <<t1[i3];
    }
    output7a << ");" << endl;

    output7a << "c2(x)=select(x"; // sortie du coefficient 2
    for (int i4=0;i4<145;i4++) {
    output7a << "," <<coeff2[i4];
    }
    output7a << ");" << endl;

    output7a << "p2(x)=select(x";
    for (int i5=0;i5<145;i5++) {
    output7a << "," <<p2[i5];
    }
    output7a << ");" << endl;

    output7a << "t2(x)=select(x";
    for (int i6=0;i6<145;i6++) {
    output7a << "," <<t2[i6];
    }
    output7a << ");" << endl;

    output7 << "\n" << entetecal(material) << "\n" << output7a.str() << "\n" << basdepagecal() << "\n" << exrad(material) << endl;  // passage du buffer dans le fichier avec entete et basdepage
    output7.close();

    // pour comparaison avec donnees provenant de la theorie
    /*
    output13 << "phi_i\ttheta_i\tcoef1\tphi1\ttheta1\tcoef2\tphi2\ttheta2\n" << endl;
    output13 << output7b.str() << endl;
    */

    // vérification de la transmission et normalisation des données
    for (int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
        // for 145 output zones
        double transmission = 0.;
        double normalisation = 0.;
        for (int outputZoneIndex=0; outputZoneIndex<145; ++outputZoneIndex) {
            transmission += coeff[inputZoneIndex][outputZoneIndex]*zone_tregenza_projected_solid_angle(outputZoneIndex+1);
            normalisation += zone_tregenza_projected_solid_angle(outputZoneIndex+1);
        }
        transmission *= (M_PI/normalisation);

        cout << "inputZone: (reversed) " << inputZoneIndex+1 << " (" << zone_tregenza_reversed(inputZoneIndex+1)
             << ")\ttau_hemisph, 145: " << 100.*transmission << "%";

        // for 1297 output zones
        transmission = 0.;
        normalisation = 0.;
        for (int outputZoneIndex=0; outputZoneIndex<1297; ++outputZoneIndex) {
            transmission += coeff5deg[inputZoneIndex][outputZoneIndex]*zone_5deg_projected_solid_angle(outputZoneIndex+1);
            normalisation += zone_5deg_projected_solid_angle(outputZoneIndex+1);
        }
        transmission *= (M_PI/normalisation);

        cout << "\t1297: " << 100.*transmission << "%" << endl;
    }

    // sortie de la matrice  de transmission avec les angles de sorties des 2 pics
    fstream outputdat("tau_145x2peaks.dat", ios::out | ios::binary);
    for (int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
        outputdat << p1[inputZoneIndex] << "\t" << t1[inputZoneIndex] << "\t" << coeff1[inputZoneIndex] << "\t";
        outputdat << p2[inputZoneIndex] << "\t" << t2[inputZoneIndex] << "\t" << coeff2[inputZoneIndex] << endl;
    }
    outputdat << endl;
    outputdat.close();

    // sortie de la matrice de transmission dans un fichier .dat (145x145)
    outputdat.open("tau_145x145.dat", ios::out | ios::binary);
    for (int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
        for (int outputZoneIndex=0; outputZoneIndex<145; ++outputZoneIndex) {
            outputdat << coeff[inputZoneIndex][outputZoneIndex]*zone_tregenza_projected_solid_angle(outputZoneIndex+1) << " ";
        }
        outputdat << endl;
    }
    outputdat << endl;
    outputdat.close();

    // sortie de la matrice de transmission dans un fichier .dat (145x1297)
    outputdat.open("tau_145x1297.dat", ios::out | ios::binary);
    for (int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
        for (int outputZoneIndex=0; outputZoneIndex<1297; ++outputZoneIndex) {
            outputdat << coeff5deg[inputZoneIndex][outputZoneIndex]*zone_5deg_projected_solid_angle(outputZoneIndex+1) << " ";
        }
        outputdat << endl;
    }
    outputdat << endl;
    outputdat.close();

    // sortie de la matrice BTDF dans un fichier .dat (145x145)
    outputdat.open("btdf_145x145.dat", ios::out | ios::binary);
    for (int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
        for (int outputZoneIndex=0; outputZoneIndex<145; ++outputZoneIndex) {
            outputdat << coeff[inputZoneIndex][outputZoneIndex] << " ";
        }
        outputdat << endl;
    }
    outputdat << endl;
    outputdat.close();

    // sortie de la matrice BTDF dans un fichier .dat (145x1297)
    outputdat.open("btdf_145x1297.dat", ios::out | ios::binary);
    for (int inputZoneIndex=0; inputZoneIndex<145; ++inputZoneIndex) {
        for (int outputZoneIndex=0; outputZoneIndex<1297; ++outputZoneIndex) {
            outputdat << coeff5deg[inputZoneIndex][outputZoneIndex] << " ";
        }
        outputdat << endl;
    }
    outputdat << endl;
    outputdat.close();

    // sortie du fichier xml de 145x145
    fstream outputxml("btdf2mkillum145x145.xml", ios::out | ios::binary);
    outputxml << entetexml145x145(material) << "\n";
    for (int reversedOutputZoneIndex=0; reversedOutputZoneIndex<145; ++reversedOutputZoneIndex) {
        for (int reversedInputZoneIndex=0; reversedInputZoneIndex<145; ++reversedInputZoneIndex) {
            // the XML format requires to output on the first line the 145 ingoing values for outgoing zone 1 (weird)
            outputxml << coeff[zone_tregenza(reversedInputZoneIndex+1)-1][zone_tregenza(reversedOutputZoneIndex+1)-1] << " ";
        }
        outputxml << endl;
    }
    outputxml << basdepagexml() << "\n" << endl;
    outputxml.close();

    // sortie du fichier xml de 145x1297
    outputxml.open("btdf2mkillum145x1297.xml", ios::out | ios::binary);
    outputxml << entetexml145x1297(material) << "\n";
    for (int reversedOutputZoneIndex=0; reversedOutputZoneIndex<1297; ++reversedOutputZoneIndex) {
        for (int reversedInputZoneIndex=0; reversedInputZoneIndex<145; ++reversedInputZoneIndex) {
            // the XML format requires to output on the first line the 145 ingoing values for outgoing zone 1 (weird)
            outputxml << coeff5deg[zone_tregenza(reversedInputZoneIndex+1)-1][zone_5deg(reversedOutputZoneIndex+1)-1] << " ";
        }
        outputxml << endl;
    }
    outputxml << basdepagexml() << "\n" << endl;
    outputxml.close();

    // sortie par une réussite
    return EXIT_SUCCESS;

}



void adjacents(int pos, vector<int> &adj, float pasp, float past) {

  if ( (int)(pos/(360/pasp)) == 0 ){

    if ( pos == 0 ) {

      // début du cycle

      adj[pos] = 1;
      adj[pos+1] = 1;
      adj[ 2*( (int)(360/pasp) ) - 1]=1;
      adj[ 2*( (int)(360/pasp) ) - 2]=1;
      adj[ ( (int)(360/pasp) )    ]=1;
      adj[ ( (int)(360/pasp) ) - 1]=1; // OK
    }

    if (pos == ( (int)(360/pasp) -1 )){

      // fin de la première ligne

      adj[pos]=1;
      adj[pos-1]=1;
      adj[pos+1]=1;
      adj[pos+2] = 1;
      adj[0]=1;
      adj[pos+(int)(360/pasp)]=1; // OK
    }
    else {

      adj[pos]=1;
      adj[pos-1]=1;
      adj[pos+1]=1;
      adj[ (int)(360/pasp)*2 - ((int)(360/pasp)-(pos%((int)(360/pasp)))) ]=1;
      adj[ (int)(360/pasp)*2 - ((int)(360/pasp)-(pos%((int)(360/pasp)))) - 1]=1;
      adj[ (int)(360/pasp)*2 - ((int)(360/pasp)-(pos%((int)(360/pasp)))) - 2]=1; // OK

    }
  }
  else if ( (int)(pos/(360/pasp)) == (90/past-2) ) {

    if ( pos == ( ((int)(90/past)-2)*( (int)(360/pasp) ) + 1 ) ) {

      // début du cycle

      adj[pos] = 1;
      adj[pos-1] = 1;
      adj[pos-2] = 1;
      adj[pos+1] = 1;
      adj[pos-(int)(360/pasp)] = 1;
      adj[pos+(int)(360/pasp) -1] = 1;
      if (pos<1153) adj[pos+(int)(360/pasp)] = 1; // Modification JK - 26.10.2012, suite à un bug communiqué par AK

    }

    if ( pos == ( ((int)(90/past)-1)*( (int)(360/pasp) ) ) ){

      // fin de la première ligne

      adj[pos]=1;
      adj[pos+1]=1;
      adj[pos-1]=1;
      adj[pos-(int)(360/pasp)+1]=1;
      adj[pos-(int)(360/pasp)  ]=1;
      adj[ pos - (int)(360/pasp)*2 + 1 ]=1;
      adj[ pos - (int)(360/pasp)*2 + 2 ]=1; // OK

    }
    else {

      adj[pos]=1;
      adj[pos-1]=1;
      adj[pos+1]=1;
      adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp))) ) - (((int)(360/pasp))-(pos%((int)(360/pasp))))    ]=1;
      adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp))) ) - (((int)(360/pasp))-(pos%((int)(360/pasp)))) -1 ]=1;
      adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp))) ) - (((int)(360/pasp))-(pos%((int)(360/pasp)))) -2 ]=1; // OK

    }
  }
  else if ( pos == ((int)(360/pasp)*((int)(90/past)-1) ) ){

    // point maximal phi=0, theta=180... OK!

    adj[pos] = 1; // lui même

    for (int i=0;i<((int)(360/pasp));i++) {

      adj[pos - ((int)(360/pasp)) + i] = 1;

    } // tous les elements autour!! i.e. en (theta - pastheta), qqsoit phi !!

  }
  else if ( (pos%(int)(360/pasp)) == 0 ){

    // pour tous les multiples de (72 si pas de 5°, (int)(360/pasp) autrement!)

    adj[pos]   = 1;
    adj[pos-1] = 1;
    adj[pos-2] = 1;
    adj[pos+1] = 1;
    adj[pos-(int)(360/pasp)] = 1;
    adj[pos+(int)(360/pasp)] = 1;
    adj[pos+(int)(360/pasp)-1] = 1;
    adj[pos+2*((int)(360/pasp))-1] = 1;
    adj[pos+2*((int)(360/pasp))-2] = 1;
  }
  else if ( (pos%((int)(360/pasp))) == (((int)(360/pasp))-1) ){

    // pour tous les 71 + n*72

    adj[pos]   = 1;
    adj[pos-1] = 1;
    adj[pos+2] = 1;
    adj[pos+1] = 1;
    adj[pos-(int)(360/pasp)] = 1;
    adj[pos+(int)(360/pasp)] = 1;
    adj[pos-(int)(360/pasp)+1] = 1;
    adj[pos-2*((int)(360/pasp))+1] = 1;
    adj[pos-2*((int)(360/pasp))+2] = 1;

  }
  else{
  adj[pos]=1;
  adj[pos+1]=1;
  adj[pos-1]=1;
  adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp))) + 2) - (((int)(360/pasp))-(pos%((int)(360/pasp)))) + 1 ]=1;
  adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp))) + 2) - (((int)(360/pasp))-(pos%((int)(360/pasp))))     ]=1;
  adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp))) + 2) - (((int)(360/pasp))-(pos%((int)(360/pasp)))) - 1 ]=1;
  adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp)))    ) - (((int)(360/pasp))-(pos%((int)(360/pasp)))) + 1 ]=1;
  adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp)))    ) - (((int)(360/pasp))-(pos%((int)(360/pasp))))     ]=1;
  adj[ ((int)(360/pasp))*( (int)(pos/((int)(360/pasp)))    ) - (((int)(360/pasp))-(pos%((int)(360/pasp)))) - 1 ]=1;
  }
}

int zone_tregenza(float phi, float theta) {

    // N.B.: le repère utilisé pour les BTDF est par rapport à la normale à la surface (sortante) en coordonnées sphériques (phi et theta)
    int zone = 0;
    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // verification que theta est bien dans le range [0,180]
    assert(theta >= 0.f && theta <= 180.f);

    // conversion en altitude pour le calcul des zones
    float altitude = (90.f-theta);
    if (altitude < 0) { // on the outgoing side (interior)
        // reverse the sign of the altitude to get it positive
        altitude *= -1.f;
    }
    else { // on the incoming side (exterior)
        // add 180 degrees to the phi angle
        phi += 180.f;
    }

    // transformation de phi en positif [0,360[
    while (phi < -0.0001f) phi += 360.f;
    while (phi >= 360.f) phi -=360.f;

    // Boucle pour les autres zones intermédiaires
    //cout << "\naltitude: " << altitude << "\tphi: " << phi << endl;
    for (unsigned int i=0;i<8;++i) {
        if ( altitude < 12.f*(i+1) && altitude >= 12.f*i ) {
            //cout << "band: " << i+1 << endl;
            // computes the zone
            //cout << "floor(phi*divisions[i]/360.f + 0.5f): " << floor(phi*divisions[i]/360.f + 0.5f) << endl;
            zone = premiers[i]+(static_cast<int>(phi*static_cast<float>(divisions[i])/360.f + 0.5f))%divisions[i];
        }
    }

    return zone;

}

int zone_5deg(float phi, float theta) {

    // N.B.: le repère utilisé pour les BTDF est par rapport à la normale à la surface (sortante) en coordonnées sphériques (phi et theta)
    int zone = 0;
    int divisions[19] = { 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 1 };
    int premiers[19] = { 1, 73, 145, 217, 289, 361, 433, 505, 577, 649, 721, 793, 865, 937, 1009, 1081, 1153, 1225, 1297 };

    // verification que theta est bien dans le range [0,180]
    assert(theta >= 0.f && theta <= 180.f);

    // conversion en altitude pour le calcul des zones
    float altitude = (90.f-theta);
    if (altitude < 0) { // on the opposite side
        // reverse the sign of the altitude to get it positive
        altitude *= -1.f;
    }
    else { // on the incoming side (exterior)
        // add 180 degrees to the phi angle
        phi += 180.f;
    }

    // transformation de phi en positif [0,360[
    while (phi < -0.0001f) phi += 360.f;
    while (phi >= 360.f) phi -=360.f;

    // Boucle pour les autres zones intermédiaires
    //cout << "\naltitude: " << altitude << "\tphi: " << phi << endl;
    for (unsigned int i=0;i<19;++i) {
        if ( altitude < (5.f*(i+1)-2.5f) && altitude >= max((5.f*i-2.5f), 0.f) ) {
            //cout << "band: " << i+1 << endl;
            // computes the zone
            //cout << "floor(phi*divisions[i]/360.f + 0.5f): " << floor(phi*divisions[i]/360.f + 0.5f) << endl;
            zone = premiers[i]+(static_cast<int>(phi*divisions[i]/360.f + 0.5f))%divisions[i];
        }
    }

    return zone;

}

int zone_tregenza(int zone_reversed) {

    assert(zone_reversed >= 1 && zone_reversed <= 145);

    // this method returns a normal Tregenza zone identifier from a reversed zone identifier

    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // find the band according to the zone number
    int band;
	for (band = 7; zone_reversed > divisions[band]; --band) zone_reversed -= divisions[band];

	// returns the number accordingly
	return premiers[band]+(divisions[band]+divisions[band]/2-(zone_reversed-1))%divisions[band]; // usage with up=+Z
	//return premiers[band]+(divisions[band]-(zone_reversed-1))%divisions[band]; // usage with up=-Z

}

int zone_tregenza_xyz_prime(int zone_tregenza_xyz_doubleprime) {

    assert(zone_tregenza_xyz_doubleprime >= 1 && zone_tregenza_xyz_doubleprime <= 145);

    // this method returns a normal Tregenza zone identifier from a reversed zone identifier

    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone_tregenza_xyz_doubleprime > divisions[band]; ++band) zone_tregenza_xyz_doubleprime -= divisions[band];

	// returns the number accordingly
	return premiers[band]+(divisions[band]+divisions[band]/2-(zone_tregenza_xyz_doubleprime-1))%divisions[band]; // usage with up=-Z

}

int zone_5deg(int zone_5deg_reversed) {

    assert(zone_5deg_reversed >= 1 && zone_5deg_reversed <= 1297);

    // this method returns a normal Tregenza zone identifier from a reversed zone identifier

    int divisions[19] = { 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 1 };
    int premiers[19] = { 1, 73, 145, 217, 289, 361, 433, 505, 577, 649, 721, 793, 865, 937, 1009, 1081, 1153, 1225, 1297 };

    // find the band according to the zone number
    int band;
	for (band = 18; zone_5deg_reversed > divisions[band]; --band) zone_5deg_reversed -= divisions[band];

	// returns the number accordingly
	return premiers[band]+(divisions[band]+divisions[band]/2-(zone_5deg_reversed-1))%divisions[band]; // usage with up=+Z
	//return premiers[band]+(divisions[band]-(zone_5deg_reversed-1))%divisions[band]; // usage with up=-Z

}

int zone_tregenza_reversed(int zone) {

    assert(zone >= 1 && zone <= 145);

    // this method returns a reversed Tregenza zone identifier that starts from the top zone (145) and ends with the zone 30

    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers_reversed[8] = { 116, 86, 62, 38, 20, 8, 2, 1 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns the number accordingly
	return premiers_reversed[band]+(divisions[band]+divisions[band]/2-(zone-1))%divisions[band]; // usage with up=+Z
	//return premiers_reversed[band]+(divisions[band]-(zone-1))%divisions[band]; // usage with up=-Z

}

int zone_tregenza_180minus(int zone) {

    assert(zone >= 1 && zone <= 145);

    // this method returns a reversed Tregenza zone identifier that starts from the top zone (145) and ends with the zone 30

    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns 360 - phi
	return premiers[band]+(divisions[band]+divisions[band]/2-(zone-1))%divisions[band];

}

int zone_5deg_180minus(int zone) {

    assert(zone >= 1 && zone <= 1297);

    // this method returns a normal Tregenza zone identifier from a reversed zone identifier

    int divisions[19] = { 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 1 };
    int premiers[19] = { 1, 73, 145, 217, 289, 361, 433, 505, 577, 649, 721, 793, 865, 937, 1009, 1081, 1153, 1225, 1297 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns the number accordingly
	return premiers[band]+(divisions[band]+divisions[band]/2-(zone-1))%divisions[band];

}

int zone_tregenza_180plus(int zone) {

    assert(zone >= 1 && zone <= 145);

    // this method returns a reversed Tregenza zone identifier that starts from the top zone (145) and ends with the zone 30

    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns 360 - phi
	return premiers[band]+(divisions[band]/2+(zone-1))%divisions[band];

}

int zone_5deg_180plus(int zone) {

    assert(zone >= 1 && zone <= 1297);

    // this method returns a normal Tregenza zone identifier from a reversed zone identifier

    int divisions[19] = { 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 1 };
    int premiers[19] = { 1, 73, 145, 217, 289, 361, 433, 505, 577, 649, 721, 793, 865, 937, 1009, 1081, 1153, 1225, 1297 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns the number accordingly
	return premiers[band]+(divisions[band]/2+(zone-1))%divisions[band];

}

int zone_tregenza_360minus(int zone) {

    assert(zone >= 1 && zone <= 145);

    // this method returns a reversed Tregenza zone identifier that starts from the top zone (145) and ends with the zone 30

    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns 360 - phi
	return premiers[band]+(divisions[band]-(zone-1))%divisions[band];

}

int zone_5deg_360minus(int zone) {

    assert(zone >= 1 && zone <= 1297);

    // this method returns a normal Tregenza zone identifier from a reversed zone identifier

    int divisions[19] = { 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 1 };
    int premiers[19] = { 1, 73, 145, 217, 289, 361, 433, 505, 577, 649, 721, 793, 865, 937, 1009, 1081, 1153, 1225, 1297 };

    // find the band according to the zone number
    int band;
	for (band = 0; zone > divisions[band]; ++band) zone -= divisions[band];

	// returns the number accordingly
	return premiers[band]+(divisions[band]-(zone-1))%divisions[band];

}

double zone_tregenza_projected_solid_angle(int zone) {

    assert(zone >= 1 && zone <= 145);

    // N.B.: le repère utilisé pour les BTDF est par rapport à la normale à la surface (sortante) en coordonnées sphériques (phi et theta)
    int divisions[8] = { 30, 30, 24, 24, 18, 12, 6, 1 };
    int premiers[8] = { 1, 31, 61, 85, 109, 127, 139, 145 };

    // find the band
    size_t band=0;
    while ( zone >= premiers[band+1] ) { ++band; if (band == 7) break; }

    if ( band < 7 ) {
        //cout << "band: " << band << "\tproj_solid_angle: " << ( sin(12.*(static_cast<double>(band+1))/180.*M_PI) - sin( (12.*static_cast<double>(band))/180.*M_PI ))*sin(12.*(static_cast<double>(band)+0.5)/180.*M_PI)*((2.*M_PI)/static_cast<double>(divisions[band])) << endl;
        return ( sin(12.*(static_cast<double>(band+1))/180.*M_PI) - sin( (12.*static_cast<double>(band))/180.*M_PI ))*sin(12.*(static_cast<double>(band)+0.5)/180.*M_PI)*((2.*M_PI)/static_cast<double>(divisions[band]));
    }
    else {
        //cout << "band: " << band << "\tproj_solid_angle: " << (1.-sin(84./180.*M_PI))*2.*M_PI << endl;
        return (1.-sin(84./180.*M_PI))*2.*M_PI;
    }

}

double zone_5deg_projected_solid_angle(int zone) {

    assert(zone >= 1 && zone <= 1297);

    // N.B.: le repère utilisé pour les BTDF est par rapport à la normale à la surface (sortante) en coordonnées sphériques (phi et theta)
    int divisions[19] = { 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 1 };
    int premiers[19] = { 1, 73, 145, 217, 289, 361, 433, 505, 577, 649, 721, 793, 865, 937, 1009, 1081, 1153, 1225, 1297 };

    // find the band
    size_t band=0;
    while ( zone >= premiers[band+1] ) { ++band; if (band == 18) break; }

    if ( band < 18 ) {
        // find the elevations of the band
        double lowerElevation = max((5.*static_cast<double>(band)-2.5),0.);
        double upperElevation = (5.*(static_cast<double>(band+1))-2.5);
        //cout << "band: " << band << "\tproj_solid_angle: " << ( sin(12.*(static_cast<double>(band+1))/180.*M_PI) - sin( (12.*static_cast<double>(band))/180.*M_PI ))*sin(12.*(static_cast<double>(band)+0.5)/180.*M_PI)*((2.*M_PI)/static_cast<double>(divisions[band])) << endl;
        return (sin(upperElevation/180.*M_PI) - sin(lowerElevation/180.*M_PI))*sin(.5*(upperElevation+lowerElevation)/180.*M_PI)*((2.*M_PI)/static_cast<double>(divisions[band]));
    }
    else {
        //cout << "band: " << band << "\tproj_solid_angle: " << (1.-sin(84./180.*M_PI))*2.*M_PI << endl;
        return (1.-sin(87.5/180.*M_PI))*2.*M_PI;
    }

}

string entetecal(char *material) {

  string lentete;

  lentete =  "{\n\n        ";
  lentete += material;
  lentete += " material\n        prism2\n        v.";
  lentete += version;
  lentete += "\n        ";
  lentete += date;
  lentete += "\n        BTDF2rad, Jerome KAEMPF, LESO, EPFL\n        jerome.kaempf@epfl.ch\n\n        CAL file to compute virtual sources for re-direction with prism2";
  lentete += "\n\n        input -\n                        A1 - x-coordinate of up direction\n                        A2 - y-coordinate\n                        A3 - z-coordinate";
  lentete += "\n\n        output -\n\n        coef1           - transmission coefficient of major contribution\n        dx1, dy1, dz1   - transmission direction for major contribution\n        coef2           - transmission coefficient for minor contribution\n        dx2, dy2, dz2   - transmission direction for minor contribution\n";
  lentete += "        if forward pre-process stage to determine virtual sources\n\n        DxA, DyA, DzA   - direction to the real source\n        if backward ray-tracing stage\n\n}\n";

  return lentete;

}

string basdepagecal() {

  string basdepage;

  basdepage = "{ Compute input angles on surface \n\n        Nx, Ny, Nz                   - surface normal\n        Dx, Dy, Dz                   - direction FROM target light source (badly defined) }";

  basdepage += " \n\n{ Calculation of X' = N x A (N cross UP) }\n\n        Xprime1 = Ny*A3-Nz*A2;\n        Xprime2 = Nz*A1-Nx*A3;\n        Xprime3 = Nx*A2-Ny*A1;";
  basdepage += " \n\n{ Calculation of Y' = X'x N (X' cross N) }\n\n        Yprime1 = Xprime2*Nz-Xprime3*Ny;\n        Yprime2 = Xprime3*Nx-Xprime1*Nz;\n        Yprime3 = Xprime1*Ny-Xprime2*Nx;";
  basdepage += "\n\n{ Normalisation of N and D - normally not necessary }\n\n        NormN = sqrt(Nx*Nx+Ny*Ny+Nz*Nz);\n        NormD = sqrt(Dx*Dx+Dy*Dy+Dz*Dz);";
  basdepage += "\n\n{ Compute spherical angles associated to the refrence frame attached to the panel }\n\n        zz = (Nx*Dx+Ny*Dy+Nz*Dz);\n        theta = acos(zz/(NormN*NormD));";
  basdepage += "\n\n{ The signe constant tells us on which side the ray is }\n\n	signe = if(zz, 1., -1.);";
  basdepage += "\n\n        xx = (Xprime1*Dx+Xprime2*Dy+Xprime3*Dz);\n        yy = (Yprime1*Dx+Yprime2*Dy+Yprime3*Dz);\n        phi = atan2(yy,xx);";
  basdepage += "\n\n{ Conversion in degrees }\n\n        theta2=theta*180/PI;\n        phi2=if(phi,phi,phi+2*PI)*180/PI;";
  basdepage += "\n\n{ Adjacent Tregenza zones }\n\n        div(x)=select(x,30,30,24,24,18,12,6,1);\n\n	thetah = 90-signe*(floor((abs(90-theta2)-6)/12)*12+6);";
  basdepage += "\n	thetab = 90-signe*(floor( (abs(90-theta2)-6)/12 + 1 )*12+6);\n\n	bandeh=floor( if(thetah-90, (thetah-90)/12 + 1, (90-thetah)/12 + 1) );";
  basdepage += "\n	bandeb=floor( if(thetab-90, (thetab-90)/12 + 1, (90-thetab)/12 + 1) );\n\n	deltah=div(abs(bandeh));\n	deltab=div(abs(bandeb));";
  basdepage += "\n\n	phidh = floor(phi2*deltah/360)*(360/deltah);\n	phigh = (floor(phi2*deltah/360)+1)*(360/deltah);\n	phidb = floor(phi2*deltab/360)*(360/deltab);";
  basdepage += "\n	phigb = (floor(phi2*deltab/360)+1)*(360/deltab);\n\n{ Weight factors }";
  basdepage += "\n\n        fdh = acos( sin(thetah/180*PI)*sin(theta2/180*PI)*cos( (phidh-phi2)/180*PI ) + cos(thetah/180*PI)*cos(theta2/180*PI) );";
  basdepage += "\n        fgh = acos( sin(thetah/180*PI)*sin(theta2/180*PI)*cos( (phigh-phi2)/180*PI ) + cos(thetah/180*PI)*cos(theta2/180*PI) );";
  basdepage += "\n        fdb = acos( sin(thetab/180*PI)*sin(theta2/180*PI)*cos( (phidb-phi2)/180*PI ) + cos(thetab/180*PI)*cos(theta2/180*PI) );";
  basdepage += "\n        fgb = acos( sin(thetab/180*PI)*sin(theta2/180*PI)*cos( (phigb-phi2)/180*PI ) + cos(thetab/180*PI)*cos(theta2/180*PI) );";
  basdepage += "\n\n{ A function to determine the Tregenza zone from theta and phi }\n\n";
// previous version
//  basdepage += "\n\n        zonefunc(x,y) =if(x-90, -if(180-x-78,1+floor((mod(180-y,360)*div(1)/360)+0.5), if(180-x-66,31+floor((mod(180-y,360)*div(2)/360)+0.5), if(180-x-54, 61+floor((mod(180-y,360)*div(3)/360)+0.5), if(180-x-42, 85+floor((mod(180-y,360)*div(4)/360)+0.5), if(180-x-30, 109+floor((mod(180-y,360)*div(5)/360)+0.5), if(180-x-18, 127+floor((mod(180-y,360)*div(6)/360)+0.5), if(180-x-6, 139+floor((mod(180-y,360)*div(7)/360)+0.5), 145 ))))))), if(x-78,1+floor((y*30/360)+0.5), if(x-66,31+floor((y*30/360)+0.5), if(x-54, 61+floor((y*24/360)+0.5), if(x-42, 85+floor((y*24/360)+0.5), if(x-30, 109+floor((y*18/360)+0.5), if(x-18, 127+floor((y*12/360)+0.5), if(x-6, 139+floor((y*6/360)+0.5), 145 ))))))));";
  basdepage += "\n\n        zonefunc(x,y) =if(x-90, -if(180-x-78,1+floor((mod(y,360)*div(1)/360)+0.5), if(180-x-66,31+floor((mod(y,360)*div(2)/360)+0.5), if(180-x-54, 61+floor((mod(y,360)*div(3)/360)+0.5), if(180-x-42, 85+floor((mod(y,360)*div(4)/360)+0.5), if(180-x-30, 109+floor((mod(y,360)*div(5)/360)+0.5), if(180-x-18, 127+floor((mod(y,360)*div(6)/360)+0.5), if(180-x-6, 139+floor((mod(y,360)*div(7)/360)+0.5), 145 ))))))), if(x-78,1+floor((mod(180+y,360)*div(1)/360)+0.5), if(x-66,31+floor((mod(180+y,360)*div(2)/360)+0.5), if(x-54, 61+floor((mod(180+y,360)*div(3)/360)+0.5), if(x-42, 85+floor((mod(180+y,360)*div(4)/360)+0.5), if(x-30, 109+floor((mod(180+y,360)*div(5)/360)+0.5), if(x-18, 127+floor((mod(180+y,360)*div(6)/360)+0.5), if(x-6, 139+floor((mod(180+y,360)*div(7)/360)+0.5), 145 ))))))));";
  basdepage += "\n\n{ The 4 neighbouring zones and the usual zone }\n\n	zone = zonefunc(theta2,phi2);\n	zone1 = zonefunc(thetab,phigb);";
  basdepage += "\n	zone2 = zonefunc(thetah,phigh);\n	zone3 = zonefunc(thetah,phidh);\n	zone4 = zonefunc(thetab,phidb);";
  basdepage += "\n\n{ Coefficients associated to the combination of the 4 adjacent zones }\n{ If zone=144 -> there is no difference in phi }";
  basdepage += "\n\n	pd1 = if(abs(zone1)-144,(1/(fgb+FTINY))/2 ,(1/(fgb+FTINY)) );\n	pd2 = if(abs(zone2)-144,(1/(fgh+FTINY))/2 ,(1/(fgh+FTINY)) );";
  basdepage += "\n	pd3 = if(abs(zone3)-144,(1/(fdh+FTINY))/2 ,(1/(fdh+FTINY)) );\n	pd4 = if(abs(zone4)-144,(1/(fdb+FTINY))/2 ,(1/(fdb+FTINY)) );";
  basdepage += "\n\n{ If the coefficient is zero, we don't use it for the weighted angles, but for the weighted transmission coefficient }";
  basdepage += "\n\n        z1p1 = if(c1(abs(zone1)),1.,0);\n        z1p2 = if(c2(abs(zone1)),1.,0);\n        z2p1 = if(c1(abs(zone2)),1.,0);";
  basdepage += "\n        z2p2 = if(c2(abs(zone2)),1.,0);\n        z3p1 = if(c1(abs(zone3)),1.,0);\n        z3p2 = if(c2(abs(zone3)),1.,0);";
  basdepage += "\n        z4p1 = if(c1(abs(zone4)),1.,0);\n        z4p2 = if(c2(abs(zone4)),1.,0);";
  basdepage += "\n\n{ The flag: forward pre-process or backward raytracing\n\n  DxA, DyA, DzA are defined only in the backward raytracing bit, otherwise they are equal to 0 }";
  basdepage += "\n\n        flag = sqrt(DxA*DxA + DyA*DyA + DzA*DzA);\n\n{ Interpolation }\n\n{ Interpolation without bidirectionnality ";
  basdepage += "\n\n	coef1 = if(flag, (pd1*c1(abs(zone1)) +pd2*c1(abs(zone2)) +pd3*c1(abs(zone3)) +pd4*c1(abs(zone4)))/(pd1+pd2+pd3+pd4), if(signe, (pd1*c1(abs(zone1)) +pd2*c1(abs(zone2)) +pd3*c1(abs(zone3)) +pd4*c1(abs(zone4)))/(pd1+pd2+pd3+pd4),0));";
  basdepage += "\n	coef2 = if(flag, (pd1*c2(abs(zone1)) +pd2*c2(abs(zone2)) +pd3*c2(abs(zone3)) +pd4*c2(abs(zone4)))/(pd1+pd2+pd3+pd4), if(signe, (pd1*c2(abs(zone1)) +pd2*c2(abs(zone2)) +pd3*c2(abs(zone3)) +pd4*c2(abs(zone4)))/(pd1+pd2+pd3+pd4),0)); }";

  basdepage += "\n\n{ Output of the transmission coefficients }\n\n	coef1 = (pd1*c1(abs(zone1)) +pd2*c1(abs(zone2)) +pd3*c1(abs(zone3)) +pd4*c1(abs(zone4)))/(pd1+pd2+pd3+pd4);";
  basdepage += "\n	coef2 = (pd1*c2(abs(zone1)) +pd2*c2(abs(zone2)) +pd3*c2(abs(zone3)) +pd4*c2(abs(zone4)))/(pd1+pd2+pd3+pd4);";
  basdepage += "\n\n	post1 = (pd1*t1(abs(zone1))*z1p1 +pd2*t1(abs(zone2))*z2p1 +pd3*t1(abs(zone3))*z3p1 +pd4*t1(abs(zone4))*z4p1)/(pd1*z1p1+pd2*z2p1+pd3*z3p1+pd4*z4p1);";
  basdepage += "\n	post2 = (pd1*t2(abs(zone1))*z1p2 +pd2*t2(abs(zone2))*z2p2 +pd3*t2(abs(zone3))*z3p2 +pd4*t2(abs(zone4))*z4p2)/(pd1*z1p2+pd2*z2p2+pd3*z3p2+pd4*z4p2);";
  basdepage += "\n\n{ Evaluation of interpolation region - p1 }";
  basdepage += "\n\n        sit1=if(p1(abs(zone1))-180 ,if(p1(abs(zone2))-180 ,if(p1(abs(zone3))-180 ,if(p1(abs(zone4))-180, 2, 0), 0), 0) ,if(180-p1(abs(zone2)) ,if(180-p1(abs(zone3)) ,if(180-p1(abs(zone4)),1,0), 0), 0));";
  basdepage += "\n\n        sit3 = if( 90-p1(abs(zone1)), if ( 90-p1(abs(zone2)) , if ( 90 - p1(abs(zone3)), if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), if ( p1(abs(zone3)) - 270, if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), 0 ) ), if ( p1(abs(zone2)) - 270, if ( 90 - p1(abs(zone3)), if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), if ( p1(abs(zone3)) - 270, if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)) ,0 )), 0 ))";
  basdepage += "\n, if ( p1(abs(zone1))-270, if ( 90-p1(abs(zone2)) , if ( 90 - p1(abs(zone3)), if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), if ( p1(abs(zone3)) - 270, if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), 0 ) ), if ( p1(abs(zone2)) - 270, if ( 90 - p1(abs(zone3)), if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), if ( p1(abs(zone3)) - 270, if ( 90 -p1(abs(zone4)), 30, if ( p1(abs(zone4))-270, 30, 0)), 0 )),0 )),0));";
  basdepage += "\n\n        sit4 = if( p1(abs(zone1))-90	,if( 270-p1(abs(zone1)) ,if( p1(abs(zone2))-90	,if(270-p1(abs(zone2)), if(p1(abs(zone3))-90, if(270-p1(abs(zone3)), if(p1(abs(zone4))-90, if(270-p1(abs(zone4)), 4, 0) ,0),0),0),0),0 ), 0 ), 0 );";
  basdepage += "\n\n        sit = sit1+sit3+sit4;\n\n{ Evaluation of interpolation region  - p2 }";
  basdepage += "\n\n        sit1p2=if(p2(abs(zone1))-180 ,if(p2(abs(zone2))-180 ,if(p2(abs(zone3))-180 ,if(p2(abs(zone4))-180, 2, 0), 0), 0),if(180-p2(abs(zone2)) ,if(180-p2(abs(zone3)) ,if(180-p2(abs(zone4)),1,0), 0), 0));";
  basdepage += "\n\n        sit3p2= if( 90-p2(abs(zone1))    , if ( 90-p2(abs(zone2)) , if ( 90 - p2(abs(zone3)), if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), if ( p2(abs(zone3)) - 270, if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), 0 ) ), if ( p2(abs(zone2)) - 270, if ( 90 - p2(abs(zone3)), if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0) ), if ( p2(abs(zone3)) - 270, if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), 0 )),0 ))";
  basdepage += "\n, if ( p2(abs(zone1))-270, if ( 90-p2(abs(zone2)) , if ( 90 - p2(abs(zone3)), if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), if ( p2(abs(zone3)) - 270, if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), 0 ) ), if ( p2(abs(zone2)) - 270, if ( 90 - p2(abs(zone3)), if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), if ( p2(abs(zone3)) - 270, if ( 90 -p2(abs(zone4)), 30, if ( p2(abs(zone4))-270, 30, 0)), 0 ) ),0)) ,0));";
  basdepage += "\n\n        sit4p2= if( p2(abs(zone1))-90	,if( 270-p2(abs(zone1)) ,if( p2(abs(zone2))-90	,if(270-p2(abs(zone2)), if(p2(abs(zone3))-90, if(270-p2(abs(zone3)), if(p2(abs(zone4))-90, if(270-p2(abs(zone4)), 4, 0),0),0),0),0),0 ), 0 ), 0 );";
  basdepage += "\n\n        sitp2 = sit1p2+sit3p2+sit4p2;\n\n{ Evaluation of interpolation (phi) in the regions 1 to 4 }";
  basdepage += "\n\nposp1 = if ( sit - 30, (pd1*p1(abs(zone1))*z1p1 +pd2*p1(abs(zone2))*z2p1 +pd3*p1(abs(zone3))*z3p1 +pd4*p1(abs(zone4))*z4p1)/(pd1*z1p1+pd2*z2p1+pd3*z3p1+pd4*z4p1), if( 30 - sit, if( sit, (pd1*p1(abs(zone1))*z1p1+pd2*p1(abs(zone2))*z2p1 +pd3*p1(abs(zone3))*z3p1 +pd4*p1(abs(zone4))*z4p1)/(pd1*z1p1+pd2*z2p1+pd3*z3p1+pd4*z4p1), p1(abs(zone)) ), mod( (pd1*mod(p1(abs(zone1))+180, 360)*z1p1 +pd2*mod(p1(abs(zone2))+180,360)*z2p1 +pd3*mod(p1(abs(zone3))+180,360)*z3p1 +pd4*mod(p1(abs(zone4))+180,360)*z4p1)/(pd1*z1p1+pd2*z2p1+pd3*z3p1+pd4*z4p1) + 180, 360) ));";
  basdepage += "\n\nposp2 = if ( sitp2 - 30, (pd1*p2(abs(zone1))*z1p2 +pd2*p2(abs(zone2))*z2p2 +pd3*p2(abs(zone3))*z3p2 +pd4*p2(abs(zone4))*z4p2)/(pd1*z1p2+pd2*z2p2+pd3*z3p2+pd4*z4p2), if( 30 - sitp2 , if( sitp2, (pd1*p2(abs(zone1))*z1p2+pd2*p2(abs(zone2))*z2p2 +pd3*p2(abs(zone3))*z3p2 +pd4*p2(abs(zone4))*z4p2)/(pd1*z1p2+pd2*z2p2+pd3*z3p2+pd4*z4p2), p2(abs(zone)) ), mod( (pd1*mod(p2(abs(zone1))+180,360)*z1p2 +pd2*mod(p2(abs(zone2))+180,360)*z2p2 +pd3*mod(p2(abs(zone3))+180,360)*z3p2 +pd4*mod(p2(abs(zone4))+180,360)*z4p2)/(pd1*z1p2+pd2*z2p2+pd3*z3p2+pd4*z4p2) + 180,360) ));";
  basdepage += "\n\n{	posp1 = (pd1*p1(abs(zone1))*z1p1 +pd2*p1(abs(zone2))*z2p1 +pd3*p1(abs(zone3))*z3p1 +pd4*p1(abs(zone4))*z4p1)/(pd1*z1p1+pd2*z2p1+pd3*z3p1+pd4*z4p1); }";
  basdepage += "\n{	posp2 = (pd1*p2(abs(zone1))*z1p2 +pd2*p2(abs(zone2))*z2p2 +pd3*p2(abs(zone3))*z3p2 +pd4*p2(abs(zone4))*z4p2)/(pd1*z1p2+pd2*z2p2+pd3*z3p2+pd4*z4p2); }";
  basdepage += "\n\n{ Transformation of the resulting angles }\n\n        tt1 = (180-post1)*PI/180;\n        pp1 = (360-posp1)*PI/180;";
  basdepage += "\n\n        tt2 = (180-post2)*PI/180;\n        pp2 = (360-posp2)*PI/180;";
  basdepage += "\n\n{ Output of the re-directed components }\n";
  basdepage += "\n        dx1 = if( flag, DxA, signe*(Xprime1*sin(tt1)*cos(pp1)+Yprime1*sin(tt1)*sin(pp1)+Nx*cos(tt1)));";
  basdepage += "\n        dy1 = if( flag, DyA, signe*(Xprime2*sin(tt1)*cos(pp1)+Yprime2*sin(tt1)*sin(pp1)+Ny*cos(tt1)));";
  basdepage += "\n        dz1 = if( flag, DzA, signe*(Xprime3*sin(tt1)*cos(pp1)+Yprime3*sin(tt1)*sin(pp1)+Nz*cos(tt1)));\n";
  basdepage += "\n        dx2 = if( flag, DxA, signe*(Xprime1*sin(tt2)*cos(pp2)+Yprime1*sin(tt2)*sin(pp2)+Nx*cos(tt2)));";
  basdepage += "\n        dy2 = if( flag, DyA, signe*(Xprime2*sin(tt2)*cos(pp2)+Yprime2*sin(tt2)*sin(pp2)+Ny*cos(tt2)));";
  basdepage += "\n        dz2 = if( flag, DzA, signe*(Xprime3*sin(tt2)*cos(pp2)+Yprime3*sin(tt2)*sin(pp2)+Nz*cos(tt2)));\n";

return basdepage;

}

string exrad(char *material) {

  string rad;

  rad =  "{ Sample of .rad file for simulation: }\n\n{\n";
  rad += "void prism2 ";
  rad += "btdf2prism2"; // material
  rad += "\n9 coef1 dx1 dy1 dz1 coef2 dx2 dy2 dz2 ";
  rad += "btdf2prism2";
  rad += ".cal\n0\n3 0 0 1\n}\n{ Last parameters are the up direction vector in Radiance coordinates }\n";

  return rad;

}

float modulo(float nombre, float base) {

   return abs((floor(nombre/base))*base - nombre);

}

// routines added for XML output
string entetexml145x145(char *material) {

    int divisions[8] = { 1, 6, 12, 18, 24, 24, 30, 30 };
    float altitudes[9] = { -6.f, 6.f, 18.f, 30.f, 42.f, 54.f, 66.f, 78.f, 90.f };

    string lentete;

    lentete =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    lentete += "<WindowElement>\n";
    lentete += "\t<Information Material=\"";
    lentete += material;
    lentete += "\" Source=\"BTDF2radiance\"/>\n";
    lentete += "\t<Optical>\n";
    lentete += "\t\t<Layer>\n";
    lentete += "\t\t\t<DataDefinition>\n";
    lentete += "\t\t\t\t<IncidentDataStructure>Columns</IncidentDataStructure>\n";
    lentete += "\t\t\t\t<AngleBasis>\n";
    lentete += "\t\t\t\t\t<AngleBasisName>EPFL/Tregenza Full</AngleBasisName>\n";
    for (unsigned int index=0;index<8;++index) {
        lentete += "\t\t\t\t\t<AngleBasisBlock>\n";
        lentete += "\t\t\t\t\t\t<Theta>";
        lentete += toString((altitudes[index]+altitudes[index+1])/2.f);
        lentete += "</Theta>\n";
        lentete += "\t\t\t\t\t\t<nPhis>";
        lentete += toString(divisions[index]);
        lentete += "</nPhis>\n";
        lentete += "\t\t\t\t\t\t<ThetaBounds>\n";
        lentete += "\t\t\t\t\t\t\t<LowerTheta>";
        lentete += toString(max(altitudes[index],0.f));
        lentete += "</LowerTheta>\n";
        lentete += "\t\t\t\t\t\t\t<UpperTheta>";
        lentete += toString(altitudes[index+1]);
        lentete += "</UpperTheta>\n";
        lentete += "\t\t\t\t\t\t</ThetaBounds>\n";
        lentete += "\t\t\t\t\t</AngleBasisBlock>\n";
    }
    lentete += "\t\t\t\t</AngleBasis>\n";
    lentete += "\t\t\t</DataDefinition>\n";
    lentete += "\t\t\t<WavelengthData>\n";
    lentete += "\t\t\t\t<Wavelength>Visible</Wavelength>\n";
    lentete += "\t\t\t\t<WavelengthDataBlock>\n";
    lentete += "\t\t\t\t\t<WavelengthDataDirection>Transmission Front</WavelengthDataDirection>\n";
    lentete += "\t\t\t\t\t<ColumnAngleBasis>EPFL/Tregenza Full</ColumnAngleBasis>\n";
    lentete += "\t\t\t\t\t<RowAngleBasis>EPFL/Tregenza Full</RowAngleBasis>\n";
    lentete += "\t\t\t\t\t<ScatteringData>";

    return lentete;

}

// routines added for XML output
string entetexml145x1297(char *material) {

    int divisions[8] = { 1, 6, 12, 18, 24, 24, 30, 30 };
    float altitudes[9] = { -6.f, 6.f, 18.f, 30.f, 42.f, 54.f, 66.f, 78.f, 90.f };

	struct { float tmin; int nphis; } latitudes[20]	=
	{   {-2.5, 1},
		{2.5, 72},
		{7.5, 72},
		{12.5, 72},
		{17.5, 72},
		{22.5, 72},
		{27.5, 72},
		{32.5, 72},
		{37.5, 72},
		{42.5, 72},
		{47.5, 72},
		{52.5, 72},
		{57.5, 72},
		{62.5, 72},
		{67.5, 72},
		{72.5, 72},
		{77.5, 72},
		{82.5, 72},
		{87.5, 72},
		{90.0, 0} };

    string lentete;

    lentete =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    lentete += "<WindowElement>\n";
    lentete += "\t<Information Material=\"";
    lentete += material;
    lentete += "\" Source=\"BTDF2radiance\"/>\n";
    lentete += "\t<Optical>\n";
    lentete += "\t\t<Layer>\n";
    lentete += "\t\t\t<DataDefinition>\n";
    lentete += "\t\t\t\t<IncidentDataStructure>Columns</IncidentDataStructure>\n";
    lentete += "\t\t\t\t<AngleBasis>\n";
    lentete += "\t\t\t\t\t<AngleBasisName>EPFL/Tregenza Full</AngleBasisName>\n";
    for (unsigned int index=0;index<8;++index) {
        lentete += "\t\t\t\t\t<AngleBasisBlock>\n";
        lentete += "\t\t\t\t\t\t<Theta>";
        lentete += toString((altitudes[index]+altitudes[index+1])/2.f);
        lentete += "</Theta>\n";
        lentete += "\t\t\t\t\t\t<nPhis>";
        lentete += toString(divisions[index]);
        lentete += "</nPhis>\n";
        lentete += "\t\t\t\t\t\t<ThetaBounds>\n";
        lentete += "\t\t\t\t\t\t\t<LowerTheta>";
        lentete += toString(max(altitudes[index],0.f));
        lentete += "</LowerTheta>\n";
        lentete += "\t\t\t\t\t\t\t<UpperTheta>";
        lentete += toString(altitudes[index+1]);
        lentete += "</UpperTheta>\n";
        lentete += "\t\t\t\t\t\t</ThetaBounds>\n";
        lentete += "\t\t\t\t\t</AngleBasisBlock>\n";
    }
    lentete += "\t\t\t\t</AngleBasis>\n";
    lentete += "\t\t\t\t<AngleBasis>\n";
    lentete += "\t\t\t\t\t<AngleBasisName>EPFL/5deg Full</AngleBasisName>\n";
    for (unsigned int index=0;index<19;++index) {
        lentete += "\t\t\t\t\t<AngleBasisBlock>\n";
        lentete += "\t\t\t\t\t\t<Theta>";
        lentete += toString((latitudes[index].tmin+latitudes[index+1].tmin)/2.f);
        lentete += "</Theta>\n";
        lentete += "\t\t\t\t\t\t<nPhis>";
        lentete += toString(latitudes[index].nphis);
        lentete += "</nPhis>\n";
        lentete += "\t\t\t\t\t\t<ThetaBounds>\n";
        lentete += "\t\t\t\t\t\t\t<LowerTheta>";
        lentete += toString(max(latitudes[index].tmin,0.f));
        lentete += "</LowerTheta>\n";
        lentete += "\t\t\t\t\t\t\t<UpperTheta>";
        lentete += toString(latitudes[index+1].tmin);
        lentete += "</UpperTheta>\n";
        lentete += "\t\t\t\t\t\t</ThetaBounds>\n";
        lentete += "\t\t\t\t\t</AngleBasisBlock>\n";
    }
    lentete += "\t\t\t\t</AngleBasis>\n";
    lentete += "\t\t\t</DataDefinition>\n";
    lentete += "\t\t\t<WavelengthData>\n";
    lentete += "\t\t\t\t<Wavelength>Visible</Wavelength>\n";
    lentete += "\t\t\t\t<WavelengthDataBlock>\n";
    lentete += "\t\t\t\t\t<WavelengthDataDirection>Transmission Front</WavelengthDataDirection>\n";
    lentete += "\t\t\t\t\t<ColumnAngleBasis>EPFL/Tregenza Full</ColumnAngleBasis>\n";
    lentete += "\t\t\t\t\t<RowAngleBasis>EPFL/5deg Full</RowAngleBasis>\n";
    lentete += "\t\t\t\t\t<ScatteringData>";

    return lentete;

}

string basdepagexml() {

    string basdepage;

    basdepage =  "\t\t\t\t\t</ScatteringData>\n";
    basdepage += "\t\t\t\t</WavelengthDataBlock>\n";
    basdepage += "\t\t\t</WavelengthData>\n";
    basdepage += "\t\t</Layer>\n";
    basdepage += "\t</Optical>\n";
    basdepage += "</WindowElement>\n";

    return basdepage;

}
