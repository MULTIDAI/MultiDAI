#include <criterion/criterion.h>
#include "KDTree.h"
#include "CKDTree.h"
#include "Helper.h"
#include "ExperimentHelper.h"
#include "DataImporter.h"
#include "StaticKDTree.h"
#include "LazyCKDTree.h"
#include "QUASII.h"
#include "ModularCKDTree.h"
#include "HybridKDTree.h"
#include "DD1CStochasticCrackingStrategy.h"
#include "MDD1RStochasticCrackingStrategy.h"
#include "LazyCrackingStrategy.h"


Test(KDTree, init) {
  pointVec points;
  point_t pt;
  pt = {0.0};
  points.push_back(pt);
  KDTree tree = KDTree(points);
  cr_assert_not_null(tree.root);
  cr_assert_eq(tree.arr.size(), 1);
  cr_assert_eq(tree.arr[0].first[0], 0.0);
}

Test(KDTree, crack_in_two_simple) {
  pointVec points;
  point_t pt;
  pt = {0.0};
  points.push_back(pt);
  pt = {1.0};
  points.push_back(pt);
  KDTree tree = KDTree(points);
  cr_assert_eq(tree.arr.size(), 2);
  KDNodePtrs nodes = tree.crack_in_two(tree.root, 0.5, 0);
  // std::cout << "crack_in_two_simple, values: " << nodes->at(0)->begin->first[0] << ", " << nodes->at(1)->begin->first[0] << "\n";
  cr_assert_eq(nodes->at(0)->begin->first[0], 0);
  cr_assert_eq(nodes->at(1)->begin->first[0], 1);
}

Test(KDTree, crack_in_two_multi_dim) {
  pointVec points;
  point_t pt00 = {0.0, 0.0};
  points.push_back(pt00);
  point_t pt10 = {1.0, 0.0};
  points.push_back(pt10);
  point_t pt01 = {0.0, 1.0};
  points.push_back(pt01);
  point_t pt11 = {1.0, 1.0};
  points.push_back(pt11);
  KDTree tree = KDTree(points);
  cr_assert_eq(tree.arr.size(), 4);
  KDNodePtrs nodes = tree.crack_in_two(tree.root, 0.5, 0);
  cr_assert_eq(nodes->size(), 2);
  KDNodePtrs nodes_first = tree.crack_in_two(nodes->at(0), 0.5, 1);
  KDNodePtrs nodes_second = tree.crack_in_two(nodes->at(1), 0.5, 1);
  cr_assert_eq(nodes_first->at(0)->begin->first, pt00);
  cr_assert_eq(nodes_first->at(1)->begin->first, pt01);
  cr_assert_eq(nodes_second->at(0)->begin->first, pt10);
  cr_assert_eq(std::next(nodes_second->at(0)->begin)->first, pt11);
}

Test(KDTree, final_partition_extract){
  pointVec points;
  point_t pt00 = {0.0, 0.0};
  points.push_back(pt00);
  point_t pt10 = {1.0, 0.0};
  points.push_back(pt10);
  point_t pt01 = {0.0, 1.0};
  points.push_back(pt01);
  point_t pt11 = {1.0, 1.0};
  points.push_back(pt11);
  KDTree tree = KDTree(points);
  pointIndexArr res = tree.final_partition_extract(tree.root, {0, 0}, {1.1, 1});
  cr_assert_eq(res, pointIndexArr({pointIndex(point_t({0,0}), 0),
                                   pointIndex(point_t({1,0}), 1)}));
}

Test(LazyCKDTree, init) {
  pointVec points;
  point_t pt;
  pt = {0.0};
  points.push_back(pt);
  LazyCKDTree tree = LazyCKDTree(points);
  cr_assert_not_null(tree.root);
  cr_assert_eq(tree.arr.size(), 1);
  cr_assert_eq(tree.arr[0].first[0], 0.0);
}


Test(LazyCKDTree, crack_in_two_simple) {
  pointVec points;
  point_t pt0 = {0.0};
  points.push_back(pt0);
  point_t pt1 = {1.0};
  points.push_back(pt1);
  LazyCKDTree tree = LazyCKDTree(points);
  cr_assert_eq(tree.arr.size(), 2);
  pointIndexArr nodes = tree.query({0.5}, {1.5});
  // H::print_arr(nodes); std::cout << " <- is nodes in crack_in_two_simple\n";
  cr_assert_eq(nodes.empty(), false);
  cr_assert_eq(nodes[0].first, pt1);
  cr_assert_eq(tree.root->get_size(), 3);
  // Check that empty node is empty
  cr_assert_eq(std::prev(tree.root->CI.end())->second->begin,
               std::prev(tree.root->CI.end())->second->end);
}


Test(LazyCKDTree, crack_in_two_1_multi_dim) {
  pointVec points;
  point_t pt00 = {0.0, 0.0};
  points.push_back(pt00);
  point_t pt10 = {1.0, 0.0};
  points.push_back(pt10);
  point_t pt01 = {0.0, 1.0};
  points.push_back(pt01);
  point_t pt11 = {1.0, 1.0};
  points.push_back(pt11);
  LazyCKDTree tree = LazyCKDTree(points);
  cr_assert_eq(tree.arr.size(), 4);
  pointIndexArr nodes = tree.query({0.5, 0.0}, {1.5, 1.1});
  cr_assert_eq(nodes.empty(), false);
  cr_assert((nodes[0].first == pt10) ||
            (nodes[0].first == pt11));
  cr_assert(nodes[1].first == pt10 ||
            nodes[1].first == pt11);
  cr_assert_eq(tree.root->get_size(), 3);
  // Check that empty node is empty
  cr_assert_eq(std::prev(tree.root->CI.end())->second->begin,
               std::prev(tree.root->CI.end())->second->end);
}


Test(LazyCKDTree, larger_tree) {
  value_t n = 5;
  pointVec points = H::point_set_2d_grid(n);
  LazyCKDTree tree = LazyCKDTree(points);
  cr_assert_eq(tree.arr.size(), n * n);
  pointIndexArr nodes = tree.query({1.5, 1.5}, {6.5, 6.5});
  cr_assert_eq(nodes.empty(), false);
  pointIndexArr nodes2 = tree.query({-1,-1}, {4.5, 4.5});
  cr_assert_eq(nodes2.empty(), false);
  // cr_assert_eq(tree.root->get_size(), 3);
  // // Check that empty node is empty
  // cr_assert_eq(std::prev(tree.root->CI.end())->second->begin,
  //              std::prev(tree.root->CI.end())->second->end);
}

Test(LazyCKDTree, verify_query){
  // std::cout << "Verifying query \n";
  pointVec points;
  point_t pt00 = {0.0, 0.0};
  points.push_back(pt00);
  point_t pt10 = {1.0, 0.0};
  points.push_back(pt10);
  point_t pt01 = {0.0, 1.0};
  points.push_back(pt01);
  point_t pt11 = {1.0, 1.0};
  points.push_back(pt11);
  LazyCKDTree tree = LazyCKDTree(points);
  point_t low = {0.0, 0.0};
  point_t high = {1.0, 1.0};
  pointIndexArr query_res = tree.query(low, high);
  bool res = E::verify_query_res(low, high, points, query_res);
  // std::cout << "Verification of query is: " << res << "\n"; 
  cr_assert_eq(res, true);
}

Test(LazyCKDTree, verify_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 100);
  KDTree* tree = new LazyCKDTree(points);
  // E::exec_queries(tree, queries);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    bool res = E::verify_query_res(q.first, q.second, points, query_res);
    cr_assert_eq(res, true);
  }
}


Test(LazyCKDTree, verify_queries_partition_size_10){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 100);
  KDTree* tree = new LazyCKDTree(points, 10);
  // E::exec_queries(tree, queries);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    bool res = E::verify_query_res(q.first, q.second, points, query_res);
    cr_assert_eq(res, true);
  }
  cr_assert(H::verify_final_partition_size(tree->root, 10));
}


// make this test but with original array, since statickkdtree has empty array
Test(StaticKDTree, verify_static_KD_Tree_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 100);
  KDTree* tree = new StaticKDTree(points);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    cr_assert_eq(E::verify_query_res(q.first, q.second, points, query_res), true);
  }
}

Test(CKDTree, verify_KD_Tree_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 10);
  KDTree* tree = new CKDTree(points);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    cr_assert_eq(E::verify_query_res(q.first, q.second, points, query_res), true);
  }
}

Test(QUASII, verify_QUASII_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 10);
  KDTree* tree = new QUASII(points, 2);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    cr_assert_eq(E::verify_query_res(q.first, q.second, points, query_res), true);
  }
}

Test(ModularCKDTree, verify_lazy_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 10);
  CrackingStrategy* C = new LazyCrackingStrategy();
  KDTree* tree = new ModularCKDTree(points, C);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    cr_assert_eq(E::verify_query_res(q.first, q.second, points, query_res), true);
  }
  cr_assert(H::verify_final_partition_size(tree->root, 10));
}

Test(ModularCKDTree, init_dd1c) {
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 10);
  CrackingStrategy* C = new DD1CStochasticCrackingStrategy();
  KDTree* tree = new ModularCKDTree(points, C, 10);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    bool res = E::verify_query_res(q.first, q.second, points, query_res);
    cr_assert_eq(res, true);
  }
  cr_assert(H::verify_final_partition_size(tree->root, 10));
}

Test(ModularCKDTree, verify_dd1c_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_100_queries.data", 100);
  CrackingStrategy* C = new DD1CStochasticCrackingStrategy();
  KDTree* tree = new ModularCKDTree(points, C);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    cr_assert_eq(E::verify_query_res(q.first, q.second, points, query_res), true);
  }
}

Test(ModularCKDTree, verify_mdd1r_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_1k_queries.data", 100);
  CrackingStrategy* C = new MDD1RStochasticCrackingStrategy();
  KDTree* tree = new ModularCKDTree(points, C);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    bool t = E::verify_query_res(q.first, q.second, points, query_res);
    if (!t) {
      std::cout << "query not satisfied\n";
      std::cout << "low: "; H::print_vec(q.first); std::cout << "\n";
      std::cout << "high: "; H::print_vec(q.second); std::cout << "\n";
    }
    cr_assert_eq(t, true);
  }
}

Test(HybridKDTree, verify_hmdd1r_queries){
  DataImporter importer;
  pointVec points = importer.loadData("data/random_60k_points.data", 6000);
  queryList queries = importer.loadQueries("data/random_1k_queries.data", 100);
  CrackingStrategy* C = new MDD1RStochasticCrackingStrategy();
  KDTree* tree = new HybridKDTree(points, C, 100, 200);
  for (query_t q : queries){
    pointIndexArr query_res = E::exec_query(tree, q);
    bool t = E::verify_query_res(q.first, q.second, points, query_res);
    if (!t) {
      std::cout << "query not satisfied\n";
      std::cout << "low: "; H::print_vec(q.first); std::cout << "\n";
      std::cout << "high: "; H::print_vec(q.second); std::cout << "\n";
    }
    cr_assert_eq(t, true);
  }
}


