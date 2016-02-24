#pragma once

#include <unordered_map>
#include "raw_pairing_data.hpp"
#include "db_factory.hpp"

class RawPairingDataStorage {
    std::vector<RawPairingDataPtr> raw_pairing_records_;
    std::map<DropletBarcode, size_t> db_index_map_;
    DbFactory db_factory_;

    // temporary stub
    std::unordered_map<std::string, std::pair<std::string, std::string> > align_record_vj_;

    void ExtractMap();

    void UpdateRecord(DropletBarcode db, std::string header, std::string sequence);

public:
    RawPairingDataStorage() {
        ExtractMap();
    }

    void Update(std::string fastq_fname);

    size_t size() const { return raw_pairing_records_.size(); }

    RawPairingDataPtr operator[](size_t index) const;

    RawPairingDataPtr GetRecordByDb(DropletBarcode db);

    typedef std::vector<RawPairingDataPtr>::const_iterator pairing_data_citerator;

    pairing_data_citerator cbegin() const { return raw_pairing_records_.cbegin(); }

    pairing_data_citerator cend() const { return raw_pairing_records_.cend(); }
};