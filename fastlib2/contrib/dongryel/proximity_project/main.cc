#include "fastlib/fastlib.h"
#include "general_spacetree.h"
#include "contrib/dongryel/pca/pca.h"
#include "pca_tree.h"
#include "gen_metric_tree.h"

typedef GeneralBinarySpaceTree<DBallBound < LMetric<2>, Vector>, Matrix, PcaStat > GTree;

int main(int argc, char *argv[]) {
 
  fx_init(argc, argv);
  const char *fname = fx_param_str(NULL, "data", NULL);
  Dataset dataset_;
  dataset_.InitFromFile(fname);
  Matrix data_;
  data_.Own(&(dataset_.matrix()));
  int leaflen = fx_param_int(NULL, "leaflen", 30);

  printf("Constructing the tree...\n");
  fx_timer_start(NULL, "pca tree");

  ArrayList<int> old_from_new;
  GTree *root_ = proximity::MakeGenMetricTree<GTree>
    (data_, leaflen, &old_from_new);

  fx_timer_stop(NULL, "pca tree");
  printf("Finished constructing the tree...\n");

  // recursively computed PCA
  printf("Recursive PCA\n");
  //(root_->stat().eigenvectors_).PrintDebug();
  //(root_->stat().eigenvalues_).PrintDebug();
  printf("Recursive PCA completed!\n");

  // exhaustively compute PCA
  printf("Exhaustive PCA\n");
  fx_timer_start(NULL, "exhaustive pca");
  Vector eigen_values;
  Matrix principal_components;
  Pca::EigenDecomposeCovariance(data_, &eigen_values, &principal_components);
  fx_timer_stop(NULL, "exhaustive pca");

  // Clean up the memory used by the tree...
  delete root_;

  fx_done();
  return 0;
}
