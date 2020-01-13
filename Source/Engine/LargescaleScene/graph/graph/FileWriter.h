/*
 * LargescaleScene: a procedural landscape rendering library.
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_FILEWRITER_H_
#define _LargescaleScene_FILEWRITER_H_

#include <fstream>
#include <iostream>
#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "Soul/core/Object.h"

using namespace std;

namespace LargescaleScene
{

/**
 * FileWriter handles file outputs for graph saving.
 * Handles binary & ascii.
 * @ingroup graph
 * @author Antoine Begault
 */
class LargescaleScene_API FileWriter
{
public:
    /**
     * Creates a new FileWriter.
     *
     * @param file the path/name of the file to write into.
     * @param binary If true, will write in binary mode. Otherwise, in ASCII.
     */
    FileWriter(const string &file, bool binary = true);

    /**
     * Deletes this FileWriter.
     */
    ~FileWriter();

    /**
     * Write method. Allows to write all sorts of data in the file.
     * @param t the data to write.
     */
    template <typename T> void write(T t)
    {
        if (isBinary) {
            out.write((char*) &t, sizeof(T));
        } else {
            out << t << ' ';
        }
    }

    /**
     * Writes the magic Number into the file. Magic Number determines
     * if the file is indexed or not.
     *
     * @param i the magic number. If 0 -> basic saving. If 1, indexed saving.
     */
    void magicNumber(int i)
    {
        out.write((char *)&i,sizeof(int));
    }

    /**
     * Returns the position of the put pointer.
     */
    streampos tellp();

    /**
     * Sets the position of the put pointer.
     *
     * @param off integral value of type streamoff representing the offset
     *      to be applied relative to an absolute position specified in the
     *      dir parameter.
     * @param dir seeking direction. It is an object of type ios_base::seekdir
     *      that specifies an absolute position from where the offset parameter
     *      off is applied. It can take any of the following member constant
     *      values: ios_base::beg | ios_base::cur | ios_base::end.
     */
    void seekp(streamoff off, ios_base::seekdir dir);

    /**
     * Sets a new field width for the output stream.
     */
    void width(streamsize wide) ;

private:
    /**
     * Output filestream.
     */
    ofstream out;

    /**
     * If true, the writer is in binary mode.
     */
    bool isBinary;
};

}

#endif
