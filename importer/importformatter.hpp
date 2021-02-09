
#ifndef IMPORTER_IMPORTFORMATTER_HPP
#define IMPORTER_IMPORTFORMATTER_HPP

/**
 * Formatters for postgresql-specific field types: hstore, int and text arrays.
 * To be used in the COPY FROM STDIN pipe to populate the tables.
 * This class and its subclasses provide methods to encode the osm data into those formats.
 */
class ImportFormatter {

protected:
    /**
     * escake a key or a value for using it in the quoted external notation
     */
    static std::string escape(const char* str) {
        // SPEED: instead of stringstream, which does dynamic allocation, use a fixed buffer
        std::stringstream escaped;

        // iterate over all chars, one by one :
        for(int i = 0; ; i++) {
            // the current char
            char c = str[i];

            // look for special cases
            switch(c) {
                case '\\':
                    escaped << "\\\\\\\\";
                    break;
                case '"':
                    escaped << "\\\\\"";
                    break;
                case '\t':
                    escaped << "\\\t";
                    break;
                case '\r':
                    escaped << "\\\r";
                    break;
                case '\n':
                    escaped << "\\\n";
                    break;
                case '\0':
                    return escaped.str();
                default:
                    escaped << c;
                    break;
            }
        }
    }

};

#endif // IMPORTER_IMPORTFORMATTER_HPP
