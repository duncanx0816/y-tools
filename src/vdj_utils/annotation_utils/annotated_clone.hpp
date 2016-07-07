#pragma once

#include "cdr_labeling_primitives.hpp"
#include "shm_annotation.hpp"
#include <read_archive.hpp>
#include "../alignment_utils/pairwise_alignment.hpp"

namespace annotation_utils {
    enum StructuralRegion { CDR1, CDR2, CDR3 };

    std::ostream& operator<<(std::ostream& out, const StructuralRegion &region);

    class AnnotatedClone {
        core::Read read_;

        std::map<StructuralRegion, seqan::Dna5String> region_string_map_;
        std::map<StructuralRegion, CDRRange> region_range_map_;

        alignment_utils::ImmuneGeneReadAlignment v_alignment_;
        alignment_utils::ImmuneGeneReadAlignment j_alignment_;

        GeneSegmentSHMs v_shms_;
        GeneSegmentSHMs j_shms_;

        seqan::String<seqan::AminoAcid> aa_read_seq_;
        unsigned read_orf_;
        bool has_stop_codon_;
        bool in_frame_;

        void CheckRangeConsistencyFatal(CDRRange range);

        void UpdateStructuralRegion(StructuralRegion region, CDRRange range);

        void Initialize(CDRLabeling cdr_labeling);

        char GetAminoAcidByPos(const seqan::String<seqan::AminoAcid> &aa_seq, size_t pos, unsigned orf) const;

        void InitializeSHMs(germline_utils::SegmentType);

        void InitializeAASeq();

        void ComputeProductiveness();

        void ComputeInFrame();

    public:
        AnnotatedClone(core::Read read,
                       CDRLabeling cdr_labeling,
                       alignment_utils::ImmuneGeneReadAlignment v_alignment,
                       alignment_utils::ImmuneGeneReadAlignment j_alignment) :
                read_(read),
                v_alignment_(v_alignment),
                j_alignment_(j_alignment),
                v_shms_(read, v_alignment.subject()),
                j_shms_(read, j_alignment.subject()) {
            Initialize(cdr_labeling);
            InitializeAASeq();
            InitializeSHMs(germline_utils::SegmentType::VariableSegment);
            InitializeSHMs(germline_utils::SegmentType::JoinSegment);
        }

        bool RegionIsEmpty(StructuralRegion region) const;

        CDRRange GetRangeByRegion(StructuralRegion region) const;

        seqan::Dna5String GetRegionString(StructuralRegion region) const;

        seqan::Dna5String CDR1() const {
            return GetRegionString(StructuralRegion::CDR1);
        }

        seqan::Dna5String CDR2() const {
            return GetRegionString(StructuralRegion::CDR2);
        }

        seqan::Dna5String CDR3() const {
            return GetRegionString(StructuralRegion::CDR3);
        }

        const core::Read& Read() const { return read_; }

        const alignment_utils::ImmuneGeneReadAlignment& VAlignment() const { return v_alignment_; }

        const alignment_utils::ImmuneGeneReadAlignment& JAlignment() const { return j_alignment_; }

        germline_utils::ChainType ChainType() const { return v_alignment_.subject().Chain(); }

        const GeneSegmentSHMs& VSHMs() const { return v_shms_; }

        const GeneSegmentSHMs& JSHMs() const { return j_shms_; }

        bool HasStopCodon() const { return has_stop_codon_; }

        bool InFrame() const { return in_frame_; }

        bool Productive() const { return !HasStopCodon() and InFrame(); }

        seqan::String<seqan::AminoAcid> AA() const { return aa_read_seq_; }

        const germline_utils::ImmuneGene& VGene() const { return v_alignment_.subject(); }

        const germline_utils::ImmuneGene& JGene() const { return j_alignment_.subject(); }
    };

    std::ostream& operator<<(std::ostream& out, const AnnotatedClone &obj);
}