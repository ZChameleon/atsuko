#include <type_traits>
#include "list.hpp"


using namespace atsuko;


static_assert(std::is_same_v<sort<List<2,5,4,6,1,3>>, List<1,2,3,4,5,6>>);


int main() {}
