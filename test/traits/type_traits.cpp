#include "traits/type_traits.hpp"

#include "../static_assert_same.hpp"

namespace estd {

void test() {

// make_ref_to_const
static_assert_same<make_ref_to_const<int        &>, int const  &>();
static_assert_same<make_ref_to_const<int       &&>, int const &&>();
static_assert_same<make_ref_to_const<int const  &>, int const  &>();
static_assert_same<make_ref_to_const<int const &&>, int const &&>();

// add_*ref
static_assert_same<add_lref<int         >, int       &>();
static_assert_same<add_lref<int        &>, int       &>();
static_assert_same<add_lref<int       &&>, int       &>();
static_assert_same<add_lref<int const  &>, int const &>();
static_assert_same<add_lref<int const &&>, int const &>();

static_assert_same<add_clref<int         >, int const &>();
static_assert_same<add_clref<int        &>, int const &>();
static_assert_same<add_clref<int       &&>, int const &>();
static_assert_same<add_clref<int const  &>, int const &>();
static_assert_same<add_clref<int const &&>, int const &>();

static_assert_same<add_rref<int         >, int       &&>();
static_assert_same<add_rref<int        &>, int        &>();
static_assert_same<add_rref<int       &&>, int       &&>();
static_assert_same<add_rref<int const  &>, int const  &>();
static_assert_same<add_rref<int const &&>, int const &&>();

static_assert_same<add_crref<int         >, int const &&>();
static_assert_same<add_crref<int        &>, int const  &>();
static_assert_same<add_crref<int       &&>, int const &&>();
static_assert_same<add_crref<int const  &>, int const  &>();
static_assert_same<add_crref<int const &&>, int const &&>();

}}
