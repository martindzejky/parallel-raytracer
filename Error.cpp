#include "Error.hpp"

#include <sstream>


Error::Error(std::string error, std::string file, unsigned int line) :
    mError(error), mFile(file), mLine(line) {}

std::string Error::GetMessage() const {
    std::stringstream ss;
    ss
        << "Error raised in file "
        << mFile
        << " on line "
        << mLine
        << ": "
        << mError;

    return ss.str();
}

const std::string &Error::GetError() const {
    return mError;
}

const std::string &Error::GetFile() const {
    return mFile;
}

const unsigned int &Error::GetLine() const {
    return mLine;
}
