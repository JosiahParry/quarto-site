
// //[[Rcpp::export]]
// std::vector<size_t> count_seq_chars_to_ref_cpp(std::vector<std::string> x, const std::string& y) {
//   std::vector<size_t> result;
//   for (const auto& xi : x) {
//     size_t count = 0;
//     auto it_x = xi.begin();
//     auto it_y = y.begin();
//     while (it_x != xi.end() && it_y != y.end() && *it_x == *it_y) {
//       ++count;
//       ++it_x;
//       ++it_y;
//     }
//     result.push_back(count);
//   }
//   return result;
// }


#include "cpp11.hpp"
using namespace cpp11;

[[cpp11::register]]
integers count_seq_chars_to_ref(strings x, strings y) {
  
  int n = x.size();
  writable::integers counts(n);
  for (int i = 0; i < n; ++i) {
    int count = 0;
    for (size_t j = 0; j < x[i].size() && j < y.size(); ++j) {
      if (x[i][j] == y[j]) {
        ++count;
      } else {
        break;
      }
    }
    counts[i] = count;
  }
  return counts;
}
