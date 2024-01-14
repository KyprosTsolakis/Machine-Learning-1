#define NDEBUG //Comment this line out for debugging
#include <stdexcept>
#include "nmea-parser.h"
#include <map>
#include <cassert>

/*
 * Map of supported formats.
 * To add another format insert:
 * ,{"FORMAT", {NUMBER OF FIELDS, FIELD WITH (First field is 0): latitude, longitude, position(N or S), position(E or W), elevation} }
*/
std::map<const string, std::tuple<const int, const int, const int, const int, const int, const int>> supportedFormats =
{
    {"GLL", {5, 0, 2, 1, 3, NULL} }
    ,{"GGA", {14, 1, 3, 2, 4, 8} }
    ,{"RMC", {11, 2, 4, 3, 5, NULL} }
};

namespace GPS::NMEA
{

bool isSupportedFormat(string format)
{
    //Checking if given format parameter is included in supportedFormats map.
    if (format.size() == 3 && supportedFormats.count(format))
        return true;
    else
        return false;
}

bool hasValidSentenceStructure(string sentence)
{
    //Checking if sentence has any data fields.
    if((int)sentence.size()<=7) return false;
    //Checking if sentence's data fields have valid structure.
    for (int i = 0; i < (int)sentence.size(); i++)
    {
        if(i<=2 && sentence[i]!="$GP"[i]) return false;
        else if(i>=3 && i<6 && !isupper(sentence[i])) return false;
        else if(i==6 && sentence[i]!=',') return false;
        else if(i>6 && sentence[i] == '$') return false;
        else if(sentence[i] == '*' && (int)sentence.size() != i+3) return false;
        else if(i > (int)sentence.size()-3  && !isxdigit(sentence[i])) return false;
    }
    return true;
}

bool checksumMatches(string sentence)
{
    //Pre-condition
    assert (hasValidSentenceStructure(sentence));
    //Splitting NMEA sentence into characters and checksum.
    string characters = sentence.substr(1, sentence.size()-4);
    string checksum = sentence.substr(sentence.size()-2, sentence.size());
    //Calculating Xor Reduction of the all characters in the NMEA sentence.
    int xorReduction = characters[0];
    for (int i = 0 ; i < (int)characters.size(); i++)
        xorReduction = (xorReduction^characters[i+1]);
    //Converting checksum from hexadecimal to decimal number so it can be compared with the Xor Reduction.
    int checksumInDec = strtol(checksum.c_str(),NULL,16);
    //Checking if checksum matches the sentence contents.
    if (checksumInDec == xorReduction)
        return true;
    else
        return false;
}

SentenceData parseSentence(string sentence)
{
    //Pre-condition
    assert (hasValidSentenceStructure(sentence));
    //Splitting NMEA sentence into format and data fields.
    string format = sentence.substr(3, 3);
    string dataField;
    vector<string> dataFields;
    for (int i = 7; i < (int)sentence.size(); i++)
    {
        //Extracting data field inside a string one character at the time.
        if (sentence[i] != ',' && sentence[i] != '*')
            dataField += sentence[i];
        //Saving completed data field in a vector.
        else
        {
            dataFields.push_back(dataField);
            dataField = "";
        }
    }
    return {format,{dataFields}};
}

bool hasCorrectNumberOfFields(SentenceData data)
{
    //Pre-condition
    assert (isSupportedFormat(data.format));
    //Checking if number of fiends is correct for a given GPS format.
    if (isSupportedFormat(data.format) && get<0>(supportedFormats.at(data.format)) == (int)data.dataFields.size())
        return true;
    else return false;
}

Position positionFromSentenceData(SentenceData data)
{
    //Pre-conditions
    assert (isSupportedFormat(data.format));
    assert (hasCorrectNumberOfFields(data));
    //Taking values from the map created at line 13.
    string latitude = data.dataFields [get<1> (supportedFormats.at(data.format))];
    string longitude = data.dataFields [get<2> (supportedFormats.at(data.format))];
    string northOrSouth = data.dataFields [get<3> (supportedFormats.at(data.format))];
    string eastOrWest = data.dataFields [get<4> (supportedFormats.at(data.format))];
    string elevation;
    //Checking if the format contains place for elevation.
    if (get<5> (supportedFormats.at(data.format)))
        elevation = data.dataFields [get<5> (supportedFormats.at(data.format))];
    else elevation = "0";
    //Throwing an exeption if position values are not a single character.
    if (northOrSouth.size() != 1)
        throw std::domain_error("Ill-formed bearing in "+ data.format + " sentence field: " + northOrSouth + ". Bearings must be a single character.");
    if (eastOrWest.size() != 1)
        throw std::domain_error("Ill-formed bearing in "+ data.format + " sentence field: " + eastOrWest + ". Bearings must be a single character.");
    try
    {
        return Position(latitude,northOrSouth[0],longitude,eastOrWest[0],elevation);
    }
    catch (const std::invalid_argument& e)
    {
        throw std::domain_error("Ill-formed "+ data.format + " sentence field: " + e.what());
    }
}

vector<Position> readSentences(std::istream &streamOfSentences)
{
    vector<Position> vectorOfSentences;
    string line;
    //Adding only valid sentences to a vector.
    while(std::getline(streamOfSentences,line))
        try
    {
        if (hasValidSentenceStructure(line) && checksumMatches(line) && hasCorrectNumberOfFields(parseSentence(line)))
                vectorOfSentences.push_back(positionFromSentenceData(parseSentence(line)));
    }
        //Skipping sentences with invalid arguments.
        catch (std::domain_error&)
    {
        continue;
    }
    return {vectorOfSentences};
}

}
