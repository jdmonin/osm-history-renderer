/**
 * The database populated by the importer sometimes uses a text[] column to store
 * the OpenStreetMap tags. This column is filled using the quoted external
 * representation as specified in the docs:
 *   https://www.postgresql.org/docs/12/arrays.html#ARRAYS-IO
 *
 * this representation can be used in the COPY FROM STDIN pipe to
 * populate the table. This class provides methods to encode the osm data
 * into that format.
 */


#ifndef IMPORTER_PGARRAY_HPP
#define IMPORTER_PGARRAY_HPP

#include <vector>

#include "importformatter.hpp"

/**
 * Provides methods to encode the osm data into the quoted external
 * postgres array format
 */
class PgArray : ImportFormatter {

public:

    /**
     * format a list of object IDs as external int[] notation
     */
    static std::string format(const std::vector<osm_object_id_t>& ids) {
        // SPEED: instead of stringstream, which does dynamic allocation, use a fixed buffer
        std::stringstream pgarray;

        pgarray << '{';

        for(std::vector<osm_object_id_t>::const_iterator it = ids.begin(); it != ids.end(); ++it) {
            pgarray << *it;

            // if necessary, add a delimiter
            if(it+1 != ids.end()) {
                pgarray << ',';
            }
        }

        pgarray << '}';

        return pgarray.str();
    }

    /**
     * format a list of strings as external text[] notation
     */
    static std::string format(const std::vector<const char*>& strs) {
        // SPEED: instead of stringstream, which does dynamic allocation, use a fixed buffer
        std::stringstream pgarray;

        pgarray << '{';

        for(std::vector<const char*>::const_iterator it = strs.begin(); it != strs.end(); ++it) {
            std::string str = escape(*it);

            pgarray << '"' << str << '"';

            // if necessary, add a delimiter
            if(it+1 != strs.end()) {
                pgarray << ',';
            }
        }

        pgarray << '}';

        return pgarray.str();
    }

    /**
     * format a list of relation members as external text[] notation
     */
    static std::string format(const std::vector<Osmium::OSM::RelationMember>& rels) {
        // SPEED: instead of stringstream, which does dynamic allocation, use a fixed buffer
        std::stringstream pgarray;

        pgarray << '{';

        for(std::vector<Osmium::OSM::RelationMember>::const_iterator it = rels.begin(); it != rels.end(); ++it) {
            pgarray << '"' << it->type() << it->ref()  // "w901965121": type + ref id
                << "\",\"" << it->role() << '"';

            // if necessary, add a delimiter
            if(it+1 != rels.end()) {
                pgarray << ',';
            }
        }

        pgarray << '}';

        return pgarray.str();
    }

    /**
     * format a taglist as external text[] notation
     */
    static std::string format(const Osmium::OSM::TagList& tags) {
        // SPEED: instead of stringstream, which does dynamic allocation, use a fixed buffer
        std::stringstream pgarray;

        pgarray << '{';

        for(Osmium::OSM::TagList::const_iterator it = tags.begin(); it != tags.end(); ++it) {
            // escape key and value
            std::string k = escape(it->key());
            std::string v = escape(it->value());

            // add to string representation
            pgarray << '"' << k << "\",\"" << v << '"';

            // if necessary, add a delimiter
            if(it+1 != tags.end()) {
                pgarray << ',';
            }
        }

        pgarray << '}';

        return pgarray.str();
    }

};

#endif // IMPORTER_PGARRAY_HPP
