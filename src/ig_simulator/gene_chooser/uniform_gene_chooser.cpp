//
// Created by Andrew Bzikadze on 3/17/17.
//

#include "random_index.hpp"
#include "uniform_gene_chooser.hpp"

namespace ig_simulator {

VDJ_GenesIndexTuple UniformVDJGeneChooser::ChooseGenes() const {
    VDJ_GenesIndexTuple result(size_t(-1), size_t(-1), size_t(-1));

    if (v_db_p_ != nullptr)
        std::get<0>(result) = random_index(0, v_db_p_->size());

    if (d_db_p_ != nullptr)
        std::get<1>(result) = random_index(0, d_db_p_->size());

    if (j_db_p_ != nullptr)
        std::get<2>(result) = random_index(0, j_db_p_->size());

    return result;
}

} // End namespace ig_simulator
