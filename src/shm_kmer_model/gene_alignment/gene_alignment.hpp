//
// Created by Andrew Bzikadze on 5/18/16.
//

#pragma once

#include <string>
#include <vector>

namespace ns_gene_alignment {
// using DnaGapped = seqan::ModifiedAlphabet<seqan::Dna5, seqan::ModExpand<'-'>>;
// using DnaGappedString = seqan::String<DnaGapped>;
// using DnaGappedAlignment = seqan::Align<DnaGappedString, seqan::ArrayGaps>;

// We place germline and read as pure c++ strings here for now.
class ReadGermlineAlignment {
private:
private:
    std::pair<std::string, std::string> read_germline_pair_;
    std::string gene_id_;
    size_t cdr1_start_;
    size_t cdr1_end_;
    size_t cdr2_start_;
    size_t cdr2_end_;

public:
    ReadGermlineAlignment(const std::string &read,
                          const std::string &germline,
                          const std::string &gene_id,
                          const size_t cdr1_start,
                          const size_t cdr1_end,
                          const size_t cdr2_start,
                          const size_t cdr2_end) :
        read_germline_pair_(std::make_pair(read, germline)),
        gene_id_(gene_id),
        cdr1_start_(cdr1_start),
        cdr1_end_(cdr1_end),
        cdr2_start_(cdr2_start),
        cdr2_end_(cdr2_end)
    { }

    const std::string &read() const { return read_germline_pair_.first; }
    const std::string &germline() const { return read_germline_pair_.second; }
    const std::string &gene_id() const { return gene_id_; }

    size_t cdr1_start() const { return cdr1_start_; }
    size_t cdr2_start() const { return cdr2_start_; }

    size_t cdr1_end() const { return cdr1_end_; }
    size_t cdr2_end() const { return cdr2_end_; }

    void set_read(const std::string &read) { read_germline_pair_.first = read; }
    void set_germline(const std::string &germline) { read_germline_pair_.second = germline; }

    void set_read(const std::string::const_iterator &begin,
                  const std::string::const_iterator &end) {
        read_germline_pair_.first.assign<std::string::const_iterator>(begin,
                                                                      end);
    }

    void set_germline(const std::string::const_iterator &begin,
                      const std::string::const_iterator &end) {
        read_germline_pair_.second.assign<std::string::const_iterator>(begin,
                                                                       end);
    }

    size_t size() const { return read_germline_pair_.first.size(); }
};
using VectorReadGermlineAlignments = std::vector<ReadGermlineAlignment>;
}
