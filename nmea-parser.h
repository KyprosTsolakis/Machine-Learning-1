#ifndef GPS_NMEA_PARSER_H
#define GPS_NMEA_PARSER_H

#include <string>
#include <vector>
#include <istream>

#include "position.h"

using std::string, std::vector, std::get;
namespace GPS::NMEA
{

  /* Determine whether the parameter is the three-character code for a sentence format+
   * that is currently supported.
   * Currently the only supported sentence formats are "GLL", "GGA" and "RMC".
   */
  bool isSupportedFormat(std::string format_parameter);

  /* Determine whether the parameter conforms to the structure of a NMEA sentence.
   * A NMEA sentence contains the following contents:
   *   - the prefix "$GP";
   *   - followed by a sequence of three uppercase (English) alphabet characters
   *     identifying the sentence format;
   *   - followed by a sequence of one or more comma-prefixed data fields;
   *   - followed by a '*' character;
   *   - followed by a two-character hexadecimal checksum.
   *
   * Data fields may contain any characters except '$' or '*', and may be empty.
   *
   * For sentences that do not match this structure, this function returns false.
   *
   * Note that this function does NOT check whether the sentence format is supported.
   */
  bool hasValidSentenceStructure(std::string);


  /* Verify whether the checksum stored at the end of the sentence matches the sentence
   * contents. Specifically, the checksum value should equal the XOR reduction of the
   * character codes of all characters between the '$' and the '*' (exclusive).
   *
   * Pre-condition: the argument string must conform to the structure of NMEA sentences.
   * Non-conforming arguments cause undefined behaviour.
   */
  bool checksumMatches(std::string);


  /* Stores the format and fields of a NMEA sentence (the checksum is not stored).
   */
  struct SentenceData
  {
      /* Stores the NMEA sentence format, excluding the 'GP' prefix.
       * E.g. "GLL".
       */
      std::string format;

      /* Stores the data fields.
       * E.g. the first chrome
       *  of the vector could be "5425.32",
       * and the second element could be "N".
       */
      std::vector<std::string> dataFields;
  };

  /* Extracts the sentence format and the field contents from a NMEA sentence string.
   * The '$GP' and the checksum are ignored.
   *
   * Pre-condition: the argument string must conform to the structure of NMEA sentences.
   * Non-conforming arguments cause undefined behaviour.
   */
  SentenceData parseSentence(std::string);


  /* Check whether the sentence data contains the correct number of fields for the
   * sentence format.
   *
   * Pre-condition: the sentence data contains a supported format.
   * Unsupported formats cause undefined behaviour.
   */
  bool hasCorrectNumberOfFields(SentenceData);


  /* Computes a Position from NMEA sentence data.
   * Currently only supports the GLL, GGA and RMC sentence formats.
   * If the format does not contain elevation data, then the elevation is set to zero.
   *
   * Throws a std::domain_error exception if the neccessary data fields contain
   * invalid data.
   *
   * Pre-conditions:
   *   - the sentence data contains a supported format;
   *   - the sentence data contains the correct number of fields for that format.
   * Unsupported formats or incorrect numbers of fields cause undefined behaviour.
   */
  Position positionFromSentenceData(SentenceData);


  /* Reads a stream of NMEA sentences (one sentence per line), and constructs a
   * vector of Positions, ignoring any lines that do not contain valid sentences.
   *
   * A line is a valid sentence if all of the following are true:
   *  - the line conforms to the structure of NMEA sentences;
   *  - the checksum matches;
   *  - the sentence format is supported (currently GLL, GGA and RMC);
   *  - the sentence has the correct number of fields;
   *  - the neccessary fields contain valid data.
   */
  std::vector<Position> readSentences(std::istream &);

}

#endif
