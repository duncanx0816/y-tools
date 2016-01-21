#pragma once

#include "crs_matrix.hpp"
#include "graph_component_map.hpp"

/*
    class sparse graph in CRS format
    contains upper and lower triangle representation of graph matrix
 */
class SparseGraph {
    CrsMatrixPtr direct_matrix_;
    CrsMatrixPtr trans_matrix_;
    // weights of vertices
    vector<size_t> weight_;
    GraphComponentMap component_map_;

public:
    SparseGraph(size_t N, const vector<GraphEdge> &edges, const vector<size_t>& weight) :
            direct_matrix_(new CrsMatrix(N, edges)), weight_(weight) {
        trans_matrix_ = direct_matrix_->Transpose();
    }

    SparseGraph(size_t N, const vector<GraphEdge> &edges) : SparseGraph(N, edges, vector<size_t>(N, 1)) {}

    size_t N() const { return direct_matrix_->N(); }

    size_t NZ() const { return direct_matrix_->NZ(); }

    size_t Degree(size_t i) const  {
        assert(i < N());
        return direct_matrix_->RowIndex()[i + 1] - direct_matrix_->RowIndex()[i] +
               trans_matrix_->RowIndex()[i + 1] - trans_matrix_->RowIndex()[i];
    }

    const vector<size_t>& RowIndex() const { return direct_matrix_->RowIndex(); }

    const vector<size_t>& RowIndexT() const { return trans_matrix_->RowIndex(); }

    const vector<size_t>& Col() const { return direct_matrix_->Col(); }

    const vector<size_t>& ColT() const { return trans_matrix_->Col(); }

    const vector<size_t>& Dist() const { return direct_matrix_->Dist(); }

    const vector<size_t>& DistT() const { return trans_matrix_->Dist(); }

    const vector<size_t>& Weight() const { return weight_; }

    const CrsMatrixPtr DirectMatrix() const { return direct_matrix_; }

    const CrsMatrixPtr TransposedMatrix() const { return trans_matrix_; }

    std::shared_ptr<SparseGraph> GetSubgraph(size_t subgraph_id, const set<size_t> &vertex_set);

    GraphComponentMap& GetGraphComponentMap() { return component_map_; }

    bool VertexIsIsolated(size_t vertex) const {
        return RowIndex()[vertex + 1] - RowIndex()[vertex] + RowIndexT()[vertex + 1] - RowIndexT()[vertex] == 0;
    }
};

ostream& operator<<(ostream &out, const SparseGraph &graph);

typedef std::shared_ptr<SparseGraph> SparseGraphPtr;