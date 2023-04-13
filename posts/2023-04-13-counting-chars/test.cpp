#include <Rcpp.h>
using namespace Rcpp;

#include <vector>
#include <string>
#include <algorithm>

//[[Rcpp::export]]
std::vector<size_t> count_seq_chars_to_ref_cpp(std::vector<std::string> x, const std::string& y) {
  std::vector<size_t> result;
  for (const auto& xi : x) {
    size_t count = 0;
    auto it_x = xi.begin();
    auto it_y = y.begin();
    while (it_x != xi.end() && it_y != y.end() && *it_x == *it_y) {
      ++count;
      ++it_x;
      ++it_y;
    }
    result.push_back(count);
  }
  return result;
}

/*** R
count_seq_chars_to_ref_cpp(c("abcd152", "abcd513", "abde324", "acde324", "abce532", "acde235"
), "abcd123")
*/
