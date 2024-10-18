/**
 * @file CSVLengthIndicated.cpp
 * @brief Contains functions for converting a CSV file to a length-indicated format and reading length-indicated records.
 * 
 * This file contains implementations of functions that convert a CSV file to a length-indicated file format and
 * read records from a length-indicated binary file.
 * 
 * @author
 * Thomas Hoerger
 * @date
 * October 18 2024
 */

#include "CSVProcessing.h" 
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Converts a CSV file to a length-indicated file format.
 * 
 * This function reads a CSV file and converts each record into a binary format where the length of each record
 * is indicated before the actual record content. Each record's length is written as a binary value, followed
 * by the record itself as a comma-separated string.
 * 
 * @param csvFile The input CSV file path.
 * @param outputFile The output file path where the length-indicated records will be written.
 * 
 * @note The output file is in binary format with each record prefixed by its length (in bytes).
 */
void convertCSVToLengthIndicated(const std::string &csvFile, const std::string &outputFile) {
    std::ifstream inputFile(csvFile);  ///< Input file stream to read the CSV file.
    std::ofstream outputFileStream(outputFile, std::ios::binary);  ///< Output file stream to write the length-indicated data in binary.

    // Check if the input or output files cannot be opened.
    if (!inputFile.is_open() || !outputFileStream.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    // Read each line (record) from the input CSV file.
    while (getline(inputFile, line)) {
        size_t recordLength = line.length();  ///< Determine the length of the current record.
        
        // Write the length of the record first (in binary format).
        outputFileStream.write(reinterpret_cast<const char *>(&recordLength), sizeof(recordLength));
        // Write the actual record data as a comma-separated string.
        outputFileStream.write(line.c_str(), recordLength);
    }

    inputFile.close();  ///< Close the input CSV file.
    outputFileStream.close();  ///< Close the output binary file.
}

/**
 * @brief Reads a length-indicated record from a binary file.
 * 
 * This function reads a single length-indicated record from the provided binary input file stream.
 * It first reads the length of the record, then reads the actual record data as a string.
 * 
 * @param fileStream The input binary file stream from which to read the length-indicated record.
 * @return A string containing the record data.
 * 
 * @note The function assumes the record length is stored as a `size_t` before each record in the binary file.
 */
std::string readLengthIndicatedRecord(std::ifstream &fileStream) {
    size_t recordLength;  ///< Variable to store the length of the record.
    
    // Read the length of the record (stored as a binary value).
    fileStream.read(reinterpret_cast<char *>(&recordLength), sizeof(recordLength));

    std::string record(recordLength, '\0');  ///< Initialize a string to hold the record data.
    // Read the actual record data.
    fileStream.read(&record[0], recordLength);

    return record;  ///< Return the record data as a string.
}
