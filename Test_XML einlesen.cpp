// Test_XML einlesen.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
//#include <algorithm>

using namespace std;
std::vector<string> Titel;

int Titel_Einlesen() {
    ifstream      file;
    string        zeile;
    //   string        ergebnis;

    file.open("database.xml"); //filename);

    if (file.good())
    {

        cout << "Virtual DJ database.xml gefunden, lese Titel." << endl;

        // An den Anfang der Datei springen
        file.seekg(ios::beg);

        int i = 0;

        while (!file.eof())
        {
            getline(file, zeile);

            int start = zeile.find("Song FilePath") + 15;
            if (start > 16) {
                int end = zeile.find("\" ") - start;
                //                std::cout << zeile.substr(start, end) << "i=" << i << "\n";
                Titel.push_back(zeile.substr(start, end));
                i++;
            }

        }

        std::cout << "Gefundene Titel: " << Titel.size()+1 << endl;

        /*        for (i = 0; i < Titel.size(); ++i)
                {
                    //Werte.at(i) greift auf das i. Element im Vektor zu.
                    //Die Zählung beginnt bei 0. Wurden also zehn Werte eingelesen,
                    // so läuft i von 0 bis 9, nicht von 1 bis 10.
                    std::cout << "#" << i << ": " << Titel.at(i) << "\n";
                }
        */

    }
    else
    {
        cout << "Datei nicht gefunden." << endl;
    }

    return 0;

}

int rekordbox_xml_generieren() {
    
    fstream f;
    f.open("rekordbox.xml", ios::out| ios::trunc);
    cout << "rekordbox.xml wird erzeugt." << endl;

    f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    f << "<DJ_PLAYLISTS Version=\"1.0.0\">" << endl;
    f << "  <PRODUCT Name=\"rekordbox\" Version=\"5.8.5\" Company=\"Pioneer DJ\"/>" << endl;
    f << "    <COLLECTION Entries=\"" << Titel.size() << "\">" << endl;
//    f << "    <COLLECTION Entries=\"1000\">" << endl;

   for (int i = 0; i < Titel.size(); ++i)
//    for (int i = 0; i < 1000 ; ++i)
    {
        std::string s = Titel.at(i);
        std::replace(s.begin(), s.end(), '\\', '/');
        
        f << "      <TRACK TrackID=\"" << i+1 << "\"" << endl;
        f << "        Location=\"file://localhost/" << s << "\">" << endl;
        f << "      </TRACK>" << endl;
    }

    f << "    </COLLECTION>" << endl;
    f << "    <PLAYLISTS>" << endl;
 
    f << "    <NODE Type=\"0\" Name=\"ROOT\" Count=\"1\">" << endl;
    f << "      <NODE Name=\"GENRE\" Type=\"0\" Count=\"4\">" << endl;
    f << "        <NODE Name=\"70er\" Type=\"1\" KeyType=\"0\" Entries=\"1\">" << endl;
    f << "          <TRACK Key=\"38\"/>" << endl;
    f << "          </NODE>" << endl;
    f << "        <NODE Name=\"80er\" Type=\"1\" KeyType=\"0\" Entries=\"1\">" << endl;
    f << "          <TRACK Key=\"39\"/>" << endl;
    f << "          </NODE>" << endl;
    f << "        <NODE Name=\"90er\" Type=\"1\" KeyType=\"0\" Entries=\"1\">" << endl;
    f << "          <TRACK Key=\"40\"/>" << endl;
    f << "          </NODE>" << endl;
    f << "        <NODE Name=\"2000er\" Type=\"1\" KeyType=\"0\" Entries=\"1\">" << endl;
    f << "          <TRACK Key=\"41\"/>" << endl;
    f << "          </NODE>" << endl;
    f << "      </NODE>" << endl;
    f << "    </NODE>" << endl;

    f << "    </PLAYLISTS>" << endl;
    f << "  </DJ_PLAYLISTS>" << endl;
    
    
    f.close();
    cout << "rekordbox.xml wurde geschrieben." << endl;
    return 0;
}

int main()
{

    Titel_Einlesen();
    rekordbox_xml_generieren();

}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.

