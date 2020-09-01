// Test_XML einlesen.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;
std::vector< vector<string> > Song;
std::vector<string> Genres;

#include <stdio.h>
#include "sqlite3.h" 

sqlite3* db;
char* zErrMsg = 0;
int rc;
string sqlcommand;
string sqlresults;
sqlite3_stmt* m_statement;

int callback(void* NotUsed, int argc, char** argv, char** azColName) {

    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    for (int i = 0; i < argc; i++) {

        // Show column name, value, and newline
        cout << azColName[i] << ": " << argv[i] << endl;

    }

    // Insert a newline
    cout << endl;

    // Return successful
    return 0;
}

int opendb() {
    rc = sqlite3_open("m:\\Engine Library\\m.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database m.db successfully\n");
    }

    return 0;
}

int printdb() {



    sqlite3_open("m:\\Engine Library\\m.db", &db);
    std::string str = "SELECT path FROM Track where id = 1;";
    sqlite3_prepare_v2(db,
        str.c_str(),
        -1,
        &m_statement,
        0);

    sqlite3_step(m_statement);

    string try1 = string(reinterpret_cast<const char*>(sqlite3_column_text(m_statement, 0)));
    string ergebnis =  try1.c_str();

    std::cout << ergebnis << "\n------------------------------------------------------\n\n";


        sqlite3_close(db);


        return 0;
}



std::string rtrim(std::string s, std::string const& delim = " \t\r\n")
{
    std::string::size_type last = s.find_last_not_of(delim.c_str());
    return last == std::string::npos ? "" : s.erase(last + 1);
}

std::string ltrim(std::string s, std::string const& delim = " \t\r\n")
{
    return s.erase(0, s.find_first_not_of(delim.c_str()));
}

std::string trim(std::string s, std::string const& delim = " \t\r\n")
{
    return ltrim(rtrim(s, delim), delim);
}

auto split(const std::string& value, char separator)
-> std::vector<std::string>
{
    std::vector<std::string> result;
    std::string::size_type p = 0;
    std::string::size_type q;
    while ((q = value.find(separator, p)) != std::string::npos) {
        result.emplace_back(value, p, q - p);
        p = q + 1;
    }
    result.emplace_back(value, p);
    return result;
}

int sammle_genre(string GenreString) {
    auto&& tokens = split(GenreString, '.');
    for (auto i : tokens) {
        std::cout << trim(i) << "\n";
        Genres.push_back(trim(i));
    }
    return 0;
}

int Titel_Einlesen() {
    std::locale::global(std::locale("German_germany.UTF-8"));

    ifstream      file;
    string        zeile;


    file.open("database.xml"); //filename);

    if (file.good())
    {

        std::cout << "Virtual DJ database.xml gefunden, lese Titel." << endl;

        // An den Anfang der Datei springen
        file.seekg(ios::beg);

        int i = 0;


        while (!file.eof())
        {
            getline(file, zeile);
            vector<string> row;

            int start = zeile.find("Song FilePath") + 15;
            if (start > 16) {
                int end = zeile.find("\" ") - start;
                row.push_back(zeile);
                std::cout << "Song #: " << i << "\n";
                std::cout << zeile.substr(start, end) << "\n";


                check_next_line:
                getline(file, zeile);
                
                row.push_back(zeile);

                int commentstart = zeile.find("<Comment>") + 9;
                if (commentstart > 9) {
                    int commentend = zeile.find("</Comment>") - commentstart;
                    std::cout << "Kommentar gefunden: " << zeile.substr(commentstart, commentend) << "\n";

                    sammle_genre(zeile.substr(commentstart, commentend));

                }

                int letztezeile = zeile.find("/Song");
                if (letztezeile > 1) {} else { goto check_next_line; }

                Song.push_back(row);

                i++;
            }

        }

        std::cout << "Gefundene Titel öäü: " << Song.size()+1 << endl;



    }
    else
    {
        std::cout << "Datei nicht gefunden." << endl;
    }

    return 0;

}

int ausgabe_songs() {

    for (auto h : Song) {
        for (auto i : h) {
            std::cout << i << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}

int ausgabe_genres() {



    std::sort(Genres.begin(), Genres.end());
    Genres.erase(unique(Genres.begin(), Genres.end()), Genres.end());

    for (auto h : Genres) {
            std::cout << h << "\n";
    }
    return 0;
}

int rekordbox_xml_generieren() {
    
    fstream f;
    f.open("rekordbox.xml", ios::out| ios::trunc);
    std::cout << "rekordbox.xml wird erzeugt." << endl;

    f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    f << "<DJ_PLAYLISTS Version=\"1.0.0\">" << endl;
    f << "  <PRODUCT Name=\"rekordbox\" Version=\"5.8.5\" Company=\"Pioneer DJ\"/>" << endl;
    f << "    <COLLECTION Entries=\"" << Song.size() << "\">" << endl;

/////////   for (int i = 0; i < Song.size(); ++i)
   for (int i = 0; i < 10; ++i)
    {
/////////       std::string s = Song.at(i);
       std::string s = "";
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
    std::cout << "rekordbox.xml wurde geschrieben." << endl;
    return 0;
}

int main()
{

//    Titel_Einlesen();
 //   opendb();
    printdb();
    //    rekordbox_xml_generieren();
//    ausgabe_songs();
//      ausgabe_genres();
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

