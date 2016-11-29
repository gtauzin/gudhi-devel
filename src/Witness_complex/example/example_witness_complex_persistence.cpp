#include <gudhi/Simplex_tree.h>
#include <gudhi/Witness_complex.h>
#include <gudhi/Persistent_cohomology.h>
#include <gudhi/Points_off_io.h>
#include <gudhi/pick_n_random_points.h>

#include <boost/program_options.hpp>

#include <CGAL/Epick_d.h>

#include <string>
#include <vector>
#include <limits>  // infinity

using namespace Gudhi;
using namespace Gudhi::persistent_cohomology;

typedef CGAL::Epick_d<CGAL::Dynamic_dimension_tag> K;
typedef typename K::Point_d Point_d;

typedef typename std::vector<Point_d> Point_vector;
typedef typename Gudhi::witness_complex::Witness_complex<K> Witness_complex;
typedef Gudhi::Simplex_tree<> SimplexTree;

typedef int Vertex_handle;
typedef double Filtration_value;

void program_options(int argc, char * argv[]
                     , int & nbL
                     , std::string & file_name
                     , std::string & filediag
                     , Filtration_value & max_squared_alpha
                     , int & p
                     , int & dim_max
                     , Filtration_value & min_persistence);

int main(int argc, char * argv[]) {
  std::string file_name;
  std::string filediag;
  Filtration_value max_squared_alpha;
  int p, nbL, lim_d;
  Filtration_value min_persistence;
  SimplexTree simplex_tree;

  program_options(argc, argv, nbL, file_name, filediag, max_squared_alpha, p, lim_d, min_persistence);

  // Extract the points from the file file_name
  Point_vector witnesses, landmarks;
  Gudhi::Points_off_reader<Point_d> off_reader(file_name);
  if (!off_reader.is_valid()) {
      std::cerr << "Witness complex - Unable to read file " << file_name << "\n";
      exit(-1);  // ----- >>
  }
  witnesses = Point_vector(off_reader.get_point_cloud());
  std::cout << "Successfully read " << witnesses.size() << " points.\n";
  std::cout << "Ambient dimension is " << witnesses[0].dimension() << ".\n";

  // Choose landmarks from witnesses
  Gudhi::subsampling::pick_n_random_points(witnesses, nbL, std::back_inserter(landmarks));

  // Compute witness complex
  Witness_complex witness_complex(landmarks,
                                  witnesses);
  
  witness_complex.create_complex(simplex_tree, max_squared_alpha, lim_d);
  
  std::cout << "The complex contains " << simplex_tree.num_simplices() << " simplices \n";
  std::cout << "   and has dimension " << simplex_tree.dimension() << " \n";

  // Sort the simplices in the order of the filtration
  simplex_tree.initialize_filtration();

  // Compute the persistence diagram of the complex
  persistent_cohomology::Persistent_cohomology<SimplexTree, Field_Zp > pcoh(simplex_tree);
  // initializes the coefficient field for homology
  pcoh.init_coefficients(p);

  pcoh.compute_persistent_cohomology(min_persistence);

  // Output the diagram in filediag
  if (filediag.empty()) {
    pcoh.output_diagram();
  } else {
    std::ofstream out(filediag);
    pcoh.output_diagram(out);
    out.close();
  }

  return 0;
}


void program_options(int argc, char * argv[]
                     , int & nbL
                     , std::string & file_name
                     , std::string & filediag
                     , Filtration_value & max_squared_alpha
                     , int & p
                     , int & dim_max
                     , Filtration_value & min_persistence) {
  
  namespace po = boost::program_options;
  
  po::options_description hidden("Hidden options");
  hidden.add_options()
      ("input-file", po::value<std::string>(&file_name),
      "Name of file containing a point set in off format.");
  

  po::options_description visible("Allowed options", 100);
  visible.add_options()
      ("help,h", "produce help message")
      ("landmarks,l", po::value<int>(&nbL),
       "Number of landmarks to choose from the point cloud.")
      ("output-file,o", po::value<std::string>(&filediag)->default_value(std::string()),
       "Name of file in which the persistence diagram is written. Default print in std::cout")
      ("max-sq-alpha,a", po::value<Filtration_value>(&max_squared_alpha)->default_value(std::numeric_limits<Filtration_value>::infinity()),
       "Maximal squared relaxation parameter.")
      ("field-charac,p", po::value<int>(&p)->default_value(11),
       "Characteristic p of the coefficient field Z/pZ for computing homology.")
      ("min-persistence,m", po::value<Filtration_value>(&min_persistence)->default_value(0),
       "Minimal lifetime of homology feature to be recorded. Default is 0. Enter a negative value to see zero length intervals")
      ("cpx-dimension,d", po::value<int>(&dim_max)->default_value(std::numeric_limits<int>::max()),
       "Maximal dimension of the weak witness complex we want to compute.");
  
  po::positional_options_description pos;
  pos.add("input-file", 1);

  po::options_description all;
  all.add(visible).add(hidden);
  po::variables_map vm;

  po::store(po::command_line_parser(argc, argv).
            options(all).positional(pos).run(), vm);
  po::notify(vm);
  
  if (vm.count("help") || !vm.count("input-file")) {
    std::cout << std::endl;
    std::cout << "Compute the persistent homology with coefficient field Z/pZ \n";
    std::cout << "of a Weak witness complex defined on a set of input points.\n \n";
    std::cout << "The output diagram contains one bar per line, written with the convention: \n";
    std::cout << "   p   dim b d \n";
    std::cout << "where dim is the dimension of the homological feature,\n";
    std::cout << "b and d are respectively the birth and death of the feature and \n";
    std::cout << "p is the characteristic of the field Z/pZ used for homology coefficients." << std::endl << std::endl;

    std::cout << "Usage: " << argv[0] << " [options] input-file" << std::endl << std::endl;
    std::cout << visible << std::endl;
    std::abort();
  }
}

